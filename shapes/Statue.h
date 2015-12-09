#ifndef STATUE_H
#define STATUE_H
#include "CS123SceneData.h"
#include <queue>
#include <random>

class Statue
{
public:
    Statue();
    ~Statue();
    Statue(glm::mat4 anchorMatrix);

    std::vector<TransPrimitive> * getObjects();

private:
    struct TripleVec{
        glm::vec4 point; //where object starts
        glm::vec4 dir; //what direction it points
        glm::vec4 bendMax; //maximum bend direction
    };

    void genStatue(glm::mat4 anchorMatrix);

    glm::mat4 m_anchor;
    std::vector<TransPrimitive> objects;
    std::queue<TripleVec> attachPoints;

    std::default_random_engine generator;

    void addBody();
    void addLimb(TripleVec tv);

    CS123SceneMaterial randMaterial();
};

#endif // STATUE_H
