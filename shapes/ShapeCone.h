#ifndef SHAPECONE_H
#define SHAPECONE_H

#include "Shape.h"

class ShapeCone :
        public Shape
{
public:
    ShapeCone(int t1, int t2, int t3);

    void setT1(int t1) override;
    void setT2(int t2) override;

    ShapeType getShapeType() override;

    void rebuildVerts() override;
    DistNorm implicitIntersect(glm::vec4 pos, glm::vec4 direction, glm::mat4 shapeTransform, glm::vec2 *storeUV) override;

};


#endif // SHAPECONE_H
