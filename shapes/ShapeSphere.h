#ifndef SHAPESPHERE_H
#define SHAPESPHERE_H

#include "Shape.h"

class ShapeSphere :
        public Shape
{
public:
    ShapeSphere(int t1, int t2, int t3);
    ShapeSphere(int t1, int t2);

    void setT1(int t1) override;
    void setT2(int t2) override;

    ShapeType getShapeType() override;

    void rebuildVerts() override;
    DistNorm implicitIntersect(glm::vec4 pos, glm::vec4 direction, glm::mat4 shapeTransform, glm::vec2 *storeUV) override;

};


#endif // SHAPESPHERE_H
