#ifndef SHAPECUBE_H
#define SHAPECUBE_H

#include "Shape.h"

class ShapeCube :
        public Shape
{
public:
    ShapeCube(int t1, int t2, int t3);

    //Only this one matters
    void setT1(int t1) override;

    ShapeType getShapeType() override;

    void rebuildVerts() override;
    DistNorm implicitIntersect(glm::vec4 pos, glm::vec4 direction, glm::mat4 shapeTransform, glm::vec2 *storeUV) override;

};

#endif // SHAPECUBE_H
