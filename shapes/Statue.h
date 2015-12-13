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

    struct DoubleVec{
        glm::vec4 point; //where object starts
        glm::vec4 dir; //what direction it points
    };
    inline std::vector<DoubleVec> *getParticles()
    {
        return &m_particles;
    }

private:
    struct TripleVec{
        glm::vec4 point; //where object starts
        glm::vec4 dir; //what direction it points
        glm::vec4 bendMax; //maximum bend direction
    };


    void genStatue(glm::mat4 anchorMatrix);

    CS123SceneMaterial m_mat;

    glm::mat4 m_anchor;
    std::vector<TransPrimitive> objects;
    std::vector<DoubleVec> m_particles;
    std::deque<TripleVec> attachPoints;

    void addBody();
    void addLimb(TripleVec tv, int joints, bool useParticles);
    void addHead(glm::vec4 point, glm::vec4 up);

    int intRange(int lo, int hi);
    float floatRange(float lo, float hi);
    glm::vec4 vecAround(glm::vec4 dir);
    glm::mat4 rotateTo(glm::vec4 from, glm::vec4 to);

    TripleVec limbStarter(TripleVec base);

    CS123SceneMaterial randMaterial();

    CS123SceneMaterial slightModMat();
};

#endif // STATUE_H
