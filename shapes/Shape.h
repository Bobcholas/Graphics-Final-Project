#ifndef SHAPE_H
#define SHAPE_H

#include "glm.hpp"
#include "CS123SceneData.h"

struct DistNorm{
    float dist;
    glm::vec4 norm;
};

class Shape
{
public:
    //Constructor: takes 3 tesselation params
    Shape(int t1, int t2, int t3);


    virtual ~Shape();

    //set tesselation params
    virtual void setT1(int t1);

    virtual void setT2(int t2);

    virtual void setT3(int t3);

    virtual void rebuildVerts() = 0; //create your vertices
    virtual ShapeType getShapeType() = 0;
    virtual int getNumVerts();


    //Buffer vertices into the provided VAO
    void bufferVerts(GLuint shader);
    void draw();
    void init();

    virtual DistNorm implicitIntersect(glm::vec4 pos, glm::vec4 direction, glm::mat4 shapeTransform, glm::vec2 *foundUV) = 0;

protected:
    std::vector<float> m_vertices;
    int m_t1, m_t2, m_t3;
    GLuint m_vboID, m_vaoID;

    void pushVertex(glm::vec4 vert, glm::vec4 norm);

    static float planeIntersect(glm::vec4 pos, glm::vec4 dir, glm::vec4 pt, glm::vec4 norm){
        return ((norm.x*pt.x + norm.y*pt.y + norm.z*pt.z) - (norm.x*pos.x + norm.y*pos.y + norm.z*pos.z)) /
                    (norm.x*dir.x + norm.y*dir.y + norm.z*dir.z);
    };


};

#endif // SHAPE_H
