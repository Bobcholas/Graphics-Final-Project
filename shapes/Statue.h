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
    Statue(glm::mat4 anchor);

    std::vector<TransPrimitive> * getObjects();

    struct DoubleVec{
        glm::vec4 point; //where object starts
        glm::vec4 dir; //what direction it points
    };
    inline std::vector<DoubleVec> *getParticles()
    {
        return &m_particles;
    }

    static glm::mat4 rotateTo(glm::vec4 from, glm::vec4 to);
    static int intRange(int lo, int hi);
    static float floatRange(float lo, float hi);

private:
    struct TripleVec{
        glm::vec4 point; //where object starts
        glm::vec4 dir; //what direction it points
        glm::vec4 bendMax; //maximum bend direction
    };

    void genStatue(glm::mat4 anchor);

    CS123SceneMaterial m_mat;

    glm::mat4 m_anchor;
    std::vector<TransPrimitive> objects;
    std::vector<DoubleVec> m_particles;
    std::deque<TripleVec> attachPoints;

    void addBody();
    void addLimb(TripleVec tv, int joints, bool useParticles);
    void addHead(glm::vec4 point, glm::vec4 up);

    static glm::vec4 vecAround(glm::vec4 dir);

    TripleVec limbStarter(TripleVec base);

    CS123SceneMaterial randMaterial();

    CS123SceneMaterial slightModMat();
};

#endif // STATUE_H
