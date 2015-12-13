#ifndef TERRAIN_H
#define TERRAIN_H

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr
#include <vector>

#include "openglshape.h"

class Terrain
{

    struct dmSq
    {
        /*
         * Diamonds: 1
         *         /   \
         *        2  5  3
         *         \   /
         *           4
         *
         * Squares: 1---2
         *          | 5 |
         *          3---4
         */
        dmSq() : v_1(0), v_2(0), v_3(0), v_4(0), v_5(0) {}
        dmSq(int top_tl, int left_tr, int right_bl, int bot_br, int center)
            : v_1(top_tl), v_2(left_tr), v_3(right_bl), v_4(bot_br), v_5(center) {}
        union {
            struct { int v_1, v_2, v_3, v_4, v_5; };
            int channels[5];
        };
    };
public:
    Terrain();

    void init();

    void draw();

private:
    void initHeights();

    float randValue(int row, int col);

    glm::vec3 getPosition(int row, int col);

    glm::vec3 getNormal(int row, int col);

    OpenGLShape m_shape;

    const float m_numRows, m_numCols;

    std::vector<float> m_heights;

    static int ix(int x, int y, int w);

    float heightVal(int ix);

    bool validCol(int col);

    bool validRow(int row);

    int getRow(int ix, int w);

    int getCol(int ix, int w);

    void addVec(float *data, int idx, glm::vec3 ins);

    GLuint m_textureID;

    GLuint m_programID;
};

#endif // TERRAIN_H
