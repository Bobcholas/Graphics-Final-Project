#include "ShapeSphere.h"
#include "Shape.h"
#include "glm.hpp"

ShapeSphere::ShapeSphere(int t1, int t2)
    : ShapeSphere(t1, t2, 1)
{
}

ShapeSphere::ShapeSphere(int t1, int t2, int t3)
    : Shape(t1, t2, t3)
{
    rebuildVerts();

}

void ShapeSphere::setT1(int t1){
    if (t1 != m_t1){
        Shape::setT1(t1);
        rebuildVerts();
    }
}
void ShapeSphere::setT2(int t2){
    if (t2 != m_t2){
        Shape::setT2(t2);
        rebuildVerts();
    }
}


ShapeType ShapeSphere::getShapeType(){
    return SHAPE_SPHERE;
}


void ShapeSphere::rebuildVerts(){

    if (m_t2 < 3){
        m_t2 = 3;
    }
    if (m_t1 < 2){
        m_t1 = 2;
    }

    m_vertices.clear();

    for (int lat = 0; lat < m_t1; lat++){
        //let's evenly space around the sphere
        float angle = M_PI * (1.f / m_t1 * lat - .5f);
        float angleplus = M_PI * (1.f / m_t1 * (lat + 1) - .5);
        float y = std::sin(angle) * .5;
        float yplus = std::sin(angleplus) * .5;
        float botscale = .25f - y*y;
        botscale = (botscale < 0) ? 0 : std::sqrt(botscale); //in case of floating point errors (infinitesimally below 0);
        float topscale = .25f - yplus * yplus;
        topscale = (topscale < 0) ? 0 : std::sqrt(topscale);
        glm::vec4 botprev = glm::vec4(botscale, y, 0, 1);
        glm::vec4 topprev = glm::vec4(topscale, yplus, 0, 1);
        glm::vec4 bp_norm = glm::vec4(botscale * 2, y*2, 0, 1);
        glm::vec4 tp_norm = glm::vec4(topprev.x * 2, topprev.y * 2, 0, 1);

        for (int lon = 0; lon < m_t2; lon++){
            float rad = (M_PI * 2.f * (lon+1)) / m_t2;
            glm::vec4 botnext = glm::vec4(std::cos(rad) * botscale,
                                          y,
                                          std::sin(rad) * botscale,
                                          1);
            glm::vec4 topnext = glm::vec4(std::cos(rad) * topscale,
                                          yplus,
                                          std::sin(rad) * topscale,
                                          1);

            glm::vec4 bn_norm = glm::vec4(2 * botnext.x, 2 * botnext.y, 2 * botnext.z, 1);
            glm::vec4 tn_norm = glm::vec4(2 * topnext.x, 2 * topnext.y, 2 * topnext.z, 1);

            if (lat != m_t1 - 1){ //this is degenerate at top of sphere
                pushVertex(topprev, tp_norm);
                pushVertex(topnext, tn_norm);
                pushVertex(botprev, bp_norm);
            }

            if (lat != 0){ //and this at bottom
                pushVertex(topnext, tn_norm);
                pushVertex(botnext, bn_norm);
                pushVertex(botprev, bp_norm);
            }

            botprev = botnext;
            topprev = topnext;
            bp_norm = bn_norm;
            tp_norm = tn_norm;
        }
    }

}


DistNorm ShapeSphere::implicitIntersect(glm::vec4 pos, glm::vec4 direction, glm::mat4 shapeTransform, glm::vec2 *storeUV){
    glm::mat4 inverse = glm::inverse(shapeTransform); //to transform world coordinates into shape coordinates
    glm::vec4 shape_pos = inverse * pos;
    glm::vec4 shape_dir = inverse * direction;
    DistNorm gives;
    gives.dist = -1;

    float a = std::pow(shape_dir.x, 2) + std::pow(shape_dir.y, 2) + std::pow(shape_dir.z, 2);
    float b = 2*shape_dir.x*shape_pos.x + 2*shape_dir.y*shape_pos.y + 2*shape_dir.z*shape_pos.z;
    float c = std::pow(shape_pos.x, 2) + std::pow(shape_pos.y, 2) + std::pow(shape_pos.z, 2) - .25;

    if (b*b - 4*a*c >= 0){
        float sqr = std::sqrt(b*b - 4*a*c);
        float plus = (-b + sqr)/(2.0*a);
        float minus = (-b - sqr)/(2.0*a);
        gives.dist = plus;
        if ((gives.dist > minus && minus >= 0) || gives.dist < 0){
            gives.dist = minus;
        }
        glm::vec4 intersect = shape_pos + shape_dir * gives.dist;
        if (gives.dist >= 0){
            gives.norm = glm::normalize(intersect);
            gives.norm.w = 0;
            if (storeUV){
                float arctan = std::atan2(intersect.z, intersect.x);
                if (arctan < 0){
                    storeUV->x = -arctan/(2*M_PI);
                }else{
                    storeUV->x = 1 - arctan/(2*M_PI);
                }

                if(std::abs(intersect.x) < .0001 && std::abs(intersect.z) < .0001){
                    storeUV->y = 0;
                }else{
                    float radius = std::sqrt(std::pow(intersect.x, 2) + std::pow(intersect.z, 2));
                    float arcsin = std::asin(intersect.y / .5f);
                    storeUV->y = arcsin / M_PI;
                }

                *storeUV += glm::vec2(.5f, .5f);
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
