#include "ShapeCube.h"
#include "Shape.h"
#include "glm.hpp"
#include <iostream>

ShapeCube::ShapeCube(int t1, int t2, int t3)
    : Shape(t1, t2, t3)
{
    rebuildVerts();

}

void ShapeCube::setT1(int t1){
    if (t1 != m_t1){
        Shape::setT1(t1);
        rebuildVerts();
    }
}

ShapeType ShapeCube::getShapeType(){
    return SHAPE_CUBE;
}


void ShapeCube::rebuildVerts(){

    if (m_t1 < 1){
        m_t1 = 1;
    }

    m_vertices.clear();
    //2 * t1 * t1 triangles per face
    for (int face = 0; face < 6; face++){ //do same generation for each face, just rotate
        glm::mat4 faceMat;
        switch (face){
        case 0:
            faceMat = glm::mat4(); //identity
            break;
        case 1:
            faceMat = glm::rotate(static_cast<float>(M_PI), glm::vec3(0.f, 1.f, 0.f));
            break;
        case 2:
            faceMat = glm::rotate(static_cast<float>(M_PI/2.f), glm::vec3(0, 1, 0));
            break;
        case 3:
            faceMat = glm::rotate(static_cast<float>(-M_PI/2.f), glm::vec3(0, 1, 0));
            break;
        case 4:
            faceMat = glm::rotate(static_cast<float>(M_PI/2.f), glm::vec3(1, 0, 0));
            break;
        case 5:
            faceMat = glm::rotate(static_cast<float>(-M_PI/2.f), glm::vec3(1, 0, 0));
            break;
        }

        for (float ind1 = 0; ind1 < m_t1; ind1++){
            for (float ind2 = 0; ind2 < m_t1; ind2++){
                float i = -.5 + ind1 * 1.f/m_t1; //x/y values
                float j = -.5 + ind2 * 1.f/m_t1;

                glm::vec4 normal = faceMat * glm::vec4(0, 0, 1, 1);
                glm::vec4 botleft = faceMat * glm::vec4(i, j, .5f, 1);
                glm::vec4 botright = faceMat * glm::vec4(i + 1.f/m_t1, j, .5f, 1);
                glm::vec4 topright = faceMat * glm::vec4(i + 1.f/m_t1, j + 1.f/m_t1, .5f, 1);
                glm::vec4 topleft = faceMat * glm::vec4(i, j + 1.f/m_t1, .5f, 1);

                pushVertex(botleft, normal);
                pushVertex(botright, normal);
                pushVertex(topleft, normal);

                pushVertex(topleft, normal);
                pushVertex(botright, normal);
                pushVertex(topright, normal);

            }
        }
    }
}


DistNorm ShapeCube::implicitIntersect(glm::vec4 pos, glm::vec4 direction, glm::mat4 shapeTransform, glm::vec2 *storeUV){
    glm::mat4 inverse = glm::inverse(shapeTransform); //to transform world coordinates into shape coordinates
    glm::vec4 shape_pos = inverse * pos;
    glm::vec4 shape_dir = inverse * direction;
    DistNorm gives;
    gives.dist = -1;
    glm::vec4 faces[] = {glm::vec4(.5, 0, 0, 1),
                         glm::vec4(-.5, 0, 0, 1),
                         glm::vec4(0, .5, 0, 1),
                         glm::vec4(0, -.5, 0, 1),
                         glm::vec4(0, 0, .5, 1),
                         glm::vec4(0, 0, -.5, 1)};
    for (int i = 0; i < 6; i++){
        glm::vec4 facePt = faces[i];
        glm::vec4 fNorm = glm::normalize(glm::vec4(glm::vec3(facePt), 0));
        float planehit = planeIntersect(shape_pos, shape_dir, facePt, fNorm);
        glm::vec4 intersect = shape_pos + shape_dir*planehit;
        if ((std::abs(intersect.x) - .5 < .001) && //idea: if the point is less than .5 from origin in each dimension, good;
                (std::abs(intersect.y) - .5 < .001) && //coordinate corresponding to face will always be .5 so don't worry
                (std::abs(intersect.z) - .5 < .001)){ //so, get magnitude, subtract .5, check < epsilon
            //ex: .25, .5, -.4 -> -.25, 0, -.1 all < epsilon

            if ((gives.dist > planehit && planehit >= 0) || gives.dist < 0){
                gives.dist = planehit;
                gives.norm = fNorm;
                if (storeUV){ //not null
                    float u, v;
                    switch (i){
                    case 0:
                        u = -intersect.z;
                        v = intersect.y;
                        break;
                    case 1:
                        u = intersect.z;
                        v = intersect.y;
                        break;
                    case 2:
                        u = intersect.x;
                        v = -intersect.z;
                        break;
                    case 3:
                        u = intersect.x;
                        v = intersect.z;
                        break;
                    case 4:
                        u = intersect.x;
                        v = intersect.y;
                        break;
                    case 5:
                        u = -intersect.x;
                        v = intersect.y;
                        break;
                    }
                    *storeUV = glm::vec2(u + .5f, v+.5f);
                }
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
