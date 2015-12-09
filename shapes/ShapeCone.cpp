#include "Shape.h"
#include "ShapeCone.h"
#include "glm.hpp"
#include <limits>

ShapeCone::ShapeCone(int t1, int t2, int t3)
    : Shape(t1, t2, t3)
{
    rebuildVerts();

}

void ShapeCone::setT1(int t1){
    if (t1 != m_t1){
        Shape::setT1(t1);
        rebuildVerts();
    }
}
void ShapeCone::setT2(int t2){
    if (t2 != m_t2){
        Shape::setT2(t2);
        rebuildVerts();
    }
}


ShapeType ShapeCone::getShapeType(){
    return SHAPE_CONE;
}


void ShapeCone::rebuildVerts(){

    if (m_t2 < 3){
        m_t2 = 3;
    }
    if (m_t1 < 1){
        m_t1 = 1;
    }

    m_vertices.clear();

    glm::vec4 center = glm::vec4(0, -.5, 0, 1);
    glm::vec4 norm = glm::vec4(0, -1, 0, 1); //flat shading on bottom
    glm::vec4 previous = glm::vec4(.5f, -.5, 0, 1);
    for (int p = 0; p < m_t2; p++){
        glm::vec4 next = glm::vec4(std::cos((-1 * (p + 1) * M_PI * 2.f)/m_t2) * .5f,
                                   -.5,
                                   std::sin((-1 * (p + 1) * M_PI * 2.f)/m_t2) * .5f,
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

    const float norm_xz_scale = 2.f * std::sqrt(5.f) / 5.f; //from the algo, except divided by 5 to normalize vector
    const float norm_y = std::sqrt(5.f) / 5.f;


    for (int lat = 0; lat < m_t1; lat++){
        float y = 1.f / m_t1 * lat - .5f;
        float botscale = .5f * (1.f - 1.f/m_t1 * lat);
        float topscale = .5f * (1.f - 1.f/m_t1 * (lat + 1));
        glm::vec4 botprev = glm::vec4(botscale, y, 0, 1);
        glm::vec4 topprev = glm::vec4(topscale, y + 1.f/m_t1, 0, 1);
        glm::vec4 pnorm = glm::vec4(norm_xz_scale, norm_y, 0, 1); //same for vertical line along side

        for (int lon = 0; lon < m_t2; lon++){
            float rad = (M_PI * 2.f * (lon+1)) / m_t2;
            glm::vec4 botnext = glm::vec4(std::cos(rad) * botscale,
                                          y,
                                          std::sin(rad) * botscale,
                                          1);
            glm::vec4 topnext = glm::vec4(std::cos(rad) * topscale,
                                          y + 1.f/m_t1,
                                          std::sin(rad) * topscale,
                                          1);
            glm::vec4 nnorm = glm::vec4(std::cos(rad) * norm_xz_scale, norm_y, std::sin(rad) * norm_xz_scale, 1);

            if (lat != m_t1 - 1){ //this is degenerate at tip of cone
                pushVertex(topprev, pnorm);
                pushVertex(topnext, nnorm);
                pushVertex(botprev, pnorm);
            }

            pushVertex(topnext, nnorm);
            pushVertex(botnext, nnorm);
            pushVertex(botprev, pnorm);

            botprev = botnext;
            topprev = topnext;
            pnorm = nnorm;
        }
    }

}


DistNorm ShapeCone::implicitIntersect(glm::vec4 pos, glm::vec4 direction, glm::mat4 shapeTransform, glm::vec2 *storeUV){
    glm::mat4 inverse = glm::inverse(shapeTransform); //to transform world coordinates into shape coordinates
    glm::vec4 shape_pos = inverse * pos;
    glm::vec4 shape_dir = inverse * direction;
    DistNorm gives;
    gives.dist = -1;

    //Body:
    float a = std::pow(shape_dir.x, 2) + std::pow(shape_dir.z, 2) - std::pow(shape_dir.y, 2)/4.0;
    float b = 2*shape_dir.x*shape_pos.x + 2*shape_dir.z*shape_pos.z - .5*shape_pos.y*shape_dir.y + .25*shape_dir.y;
    float c = std::pow(shape_pos.x, 2) + std::pow(shape_pos.z, 2) - std::pow(shape_pos.y, 2)/4.0 + shape_pos.y/4.0 - 1.0/16.0;

    if (b*b - 4*a*c >= 0){
        float sqr = std::sqrt(b*b - 4*a*c);
        float plus = (-b + sqr)/(2.0*a);
        float minus = (-b - sqr)/(2.0*a);
        glm::vec4 plusInter = shape_pos + shape_dir * plus;
        if (plusInter.y >= -.501 && plusInter.y <= .501){
            gives.dist = plus;
        }
        glm::vec4 minusInter = shape_pos + shape_dir * minus;
        if (minusInter.y >= -.501 && minusInter.y <= .501){
            if ((gives.dist > minus && minus >= 0) || gives.dist < 0){
                gives.dist = minus;
            }
        }
        glm::vec4 intersect = shape_pos + shape_dir * gives.dist;
        if (intersect.y < -.5 || intersect.y > .5){ //out of bounds
            gives.dist = -1;
        }
        if (gives.dist >= 0){
            glm::vec4 xz = glm::vec4(intersect.x, 0, intersect.z, 0);
            if (xz.x == 0 && xz.z == 0){ //point of the cone, can't normalize 0 vector
                gives.norm = glm::vec4(0, 1, 0, 0);
            }else{ //find normal
                xz = glm::normalize(xz);
                float rt5 = std::sqrt(5.0);
                glm::vec4 surfNorm = glm::vec4(xz.x * 2.0 / rt5, //from shapes algo
                                               1.0/rt5,
                                               xz.z * 2.0 / rt5,
                                               0);
                gives.norm = surfNorm;
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
    }

    //Cap:
    float cap = Shape::planeIntersect(shape_pos, shape_dir,
                                      glm::vec4(0, -.5, 0, 1), glm::vec4(0, -1.0, 0, 0));
    glm::vec4 intersect = shape_pos + shape_dir * cap;
    if (intersect.x*intersect.x + intersect.z*intersect.z <= .25 + .0001){ //plane intersect is on the circle
        if ((gives.dist > cap && cap >= 0) || gives.dist < 0){
            gives.dist = cap;
            gives.norm = glm::vec4(0, -1.0, 0, 0);
            if (storeUV){
                storeUV->x = intersect.x;
                storeUV->y = intersect.z;
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
