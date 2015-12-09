#include "ShapeCylinder.h"
#include "Shape.h"
#include "glm.hpp"

ShapeCylinder::ShapeCylinder(int t1, int t2, int t3)
    : Shape(t1, t2, t3)
{
    rebuildVerts();

}

void ShapeCylinder::setT1(int t1){
    if (t1 != m_t1){
        Shape::setT1(t1);
        rebuildVerts();
    }
}
void ShapeCylinder::setT2(int t2){
    if (t2 != m_t2){
        Shape::setT2(t2);
        rebuildVerts();
    }
}


ShapeType ShapeCylinder::getShapeType(){
    return SHAPE_CYLINDER;
}


void ShapeCylinder::rebuildVerts(){

    if (m_t2 < 3){
        m_t2 = 3;
    }
    if (m_t1 < 1){
        m_t1 = 1;
    }

    m_vertices.clear();

    for (int side = -1; side <= 1; side += 2){
        glm::vec4 center = glm::vec4(0, side * .5, 0, 1);
        glm::vec4 norm = glm::vec4(0, side, 0, 1); //flat shading on top/bottom
        glm::vec4 previous = glm::vec4(.5f, side * .5, 0, 1);
        //these are the corners; lerp between them


        for (int p = 0; p < m_t2; p++){
            glm::vec4 next = glm::vec4(std::cos((side * (p + 1) * M_PI * 2.f)/m_t2) * .5f,
                                       side * .5,
                                       std::sin((side * (p + 1) * M_PI * 2.f)/m_t2) * .5f,
                                       1);

            glm::vec4 in_lerp_prev = center; //starting point is the center
            glm::vec4 in_lerp_next;

            for (int i = 0; i < m_t1; i++){
                float lerp = 1.f / m_t1 * (i+1);

                glm::vec4 out_lerp_prev = glm::mix(center, previous, lerp);
                glm::vec4 out_lerp_next = glm::mix(center, next, lerp);

                pushVertex(in_lerp_prev, norm);
                pushVertex(out_lerp_next, norm);
                pushVertex(out_lerp_prev, norm);

                if (i != 0){
                    pushVertex(out_lerp_next, norm);
                    pushVertex(in_lerp_prev, norm);
                    pushVertex(in_lerp_next, norm);
                }

                in_lerp_prev = out_lerp_prev;
                in_lerp_next = out_lerp_next;

            }
            previous = next;

        }
    }

    for (int lat = 0; lat < m_t1; lat++){
        float y = 1.f / m_t1 * lat - .5f;
        glm::vec4 botprev = glm::vec4(.5, y, 0, 1);
        glm::vec4 topprev = glm::vec4(.5, y + 1.f/m_t1, 0, 1);
        glm::vec4 pnorm = glm::vec4(1, 0, 0, 1); //same for vertical line along side

        for (int lon = 0; lon < m_t2; lon++){
            float rad = (M_PI * 2.f * (lon+1)) / m_t2;
            glm::vec4 botnext = glm::vec4(std::cos(rad) * .5,
                                          y,
                                          std::sin(rad) * .5,
                                          1);
            glm::vec4 topnext = glm::vec4(std::cos(rad) * .5,
                                          y + 1.f/m_t1,
                                          std::sin(rad) * .5,
                                          1);
            glm::vec4 nnorm = glm::vec4(std::cos(rad), 0, std::sin(rad), 1);


            pushVertex(topprev, pnorm);
            pushVertex(topnext, nnorm);
            pushVertex(botprev, pnorm);

            pushVertex(topnext, nnorm);
            pushVertex(botnext, nnorm);
            pushVertex(botprev, pnorm);

            botprev = botnext;
            topprev = topnext;
            pnorm = nnorm;
        }
    }

}


DistNorm ShapeCylinder::implicitIntersect(glm::vec4 pos, glm::vec4 direction, glm::mat4 shapeTransform, glm::vec2 *storeUV){
    glm::mat4 inverse = glm::inverse(shapeTransform); //to transform world coordinates into shape coordinates
    glm::vec4 shape_pos = inverse * pos;
    glm::vec4 shape_dir = inverse * direction;
    DistNorm gives;
    gives.dist = -1;

    //Body:
    float a = std::pow(shape_dir.x, 2) + std::pow(shape_dir.z, 2);
    float b = 2*shape_dir.x*shape_pos.x + 2*shape_dir.z*shape_pos.z;
    float c = std::pow(shape_pos.x, 2) + std::pow(shape_pos.z, 2) - .25;

    if (b*b - 4*a*c >= 0){
        float sqr = std::sqrt(b*b - 4*a*c);
        float plus = (-b + sqr)/(2.0*a);
        float minus = (-b - sqr)/(2.0*a);
        gives.dist = plus;
        if ((gives.dist > minus && minus >= 0) || gives.dist < 0){
            gives.dist = minus;
        }
        glm::vec4 intersect = shape_pos + shape_dir * gives.dist;
        if (intersect.y < -.5 - FLOAT_EPSILON || intersect.y > .5 + FLOAT_EPSILON){ //out of bounds
            //note that for something on the surface of infinite cylinder to be out of bounds and closer than the other,
            //the cap will intersect also
            gives.dist = -1;
        }
        if (gives.dist >= 0){
            glm::vec4 xz = glm::vec4(intersect.x, 0, intersect.z, 0);
            gives.norm = glm::normalize(xz);
            if (storeUV){
                storeUV->y = intersect.y + .5f;
                float arctan = std::atan2(intersect.z, intersect.x);
                if (arctan < 0){
                    storeUV->x = -arctan/(2*M_PI);
                }else{
                    storeUV->x = 1 - arctan/(2*M_PI);
                }
            }
        }
    }

    //Cap:
    float top = planeIntersect(shape_pos, shape_dir, glm::vec4(0, .5, 0, 1), glm::vec4(0, 1.0, 0, 0));
    float bot = planeIntersect(shape_pos, shape_dir, glm::vec4(0, -.5, 0, 1), glm::vec4(0, -1.0, 0, 0));
    //again, if one is closer than the other, closer oob and farther in bounds implies
    //that curved surface is also in bounds and closer than farther cap plane
    //so, only need to check closer in bounds
    float best = std::min(top, bot);
    glm::vec4 intersect = shape_pos + shape_dir*best;
    if (intersect.x*intersect.x + intersect.z*intersect.z < .25 + FLOAT_EPSILON){ //on circle
        if ((gives.dist > best && best >= 0) || gives.dist < 0){
            gives.dist = best;
            gives.norm = glm::normalize(glm::vec4(0, intersect.y, 0, 0)); //just y-component for normal of cap
            if (storeUV){
                storeUV->x = intersect.x;
                if (intersect.y < 0){
                    storeUV->y = intersect.z;
                }else{
                    storeUV->y = -intersect.z;
                }
                *storeUV += (glm::vec2(.5f, .5f)); //shift from [-.5, .5] to [0, 1]
            }
        }
    }

    if (gives.dist >= 0){
        gives.norm = glm::vec4(glm::normalize(glm::transpose(glm::inverse(glm::mat3(shapeTransform)))*glm::vec3(gives.norm)), 0); //put into world coords
        return gives;
    }else{
        return DistNorm{-1.f, glm::vec4(0, 0, 0, 0)}; //no intersect in front found
    }
}
