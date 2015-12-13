#include "terrain.h"
#include "QQueue"
#include "QImage"
#include "particlesystem/cs123_lib/resourceloader.h"

#include <math.h>
#include <iostream>
#include <vector>

int Terrain::ix(int x, int y, int w) {
    //Get the 1D (row major) index for (x,y) in a 2D array of width w
    return x + w*y;
}

Terrain::Terrain() : m_numRows(1025), m_numCols(m_numRows), m_heights(std::vector<float>(m_numRows*m_numCols, -100)), m_shape(), m_programID(0), m_textureID(0)
{
    m_heights[0] = randValue(0, 0);
    m_heights[ix(0, m_numCols-1, m_numRows)] = randValue(0, m_numCols-1);
    m_heights[ix(m_numRows - 1, 0, m_numRows)] = randValue(m_numRows-1, 0);
    m_heights[ix(m_numRows-1, m_numCols-1, m_numRows)] = randValue(m_numRows-1, m_numCols-1);
}



/**
 * Returns a pseudo-random value between -1.0 and 1.0 for the given row and column.
 */
float Terrain::randValue(int row, int col)
{
    return -1.0 + 2.0 * glm::fract(sin(row * 127.1f + col * 311.7f) * 43758.5453123f);
}


/**
 * Returns the object-space position for the terrain vertex at the given row and column.
 */
glm::vec3 Terrain::getPosition(int row, int col)
{
    /*glm::vec3 position;
    position.x = 10 * row/m_numRows - 5;
    position.y = 0;
    position.z = 10 * col/m_numCols - 5;

    // TODO: Adjust position.y using value noise.
    //position.y = randValue(row, col);
    float freqBase = 5.0;
    for(int k = 1; k < 5; k++)
    {
        float level = freqBase*pow(2, k-1);
        float newRow = row/(100.f/level);
        float newCol = col/(100.f/level);
        float cornerA = randValue(floor(newRow), floor(newCol));
        float cornerB = randValue(floor(newRow), ceil(newCol));
        float cornerC = randValue(ceil(newRow), floor(newCol));
        float cornerD = randValue(ceil(newRow), ceil(newCol));

        float colFract = glm::fract(newCol);
        float rowFract = glm::fract(newRow);
        float ABlin = glm::mix(cornerA, cornerB, colFract*colFract*(3-2*colFract));
        float CDlin = glm::mix(cornerC, cornerD, colFract*colFract*(3-2*colFract));

        position.y += glm::mix(ABlin, CDlin, rowFract*rowFract*(3-2*rowFract))/(pow(2, k-1));

    }

    return position;*/
    return glm::vec3(10*row/m_numRows - 5, m_heights[ix(row, col, m_numCols)], 10*col/m_numCols-5);
}


/**
 * Returns the normal vector for the terrain vertex at the given row and column.
 */
glm::vec3 Terrain::getNormal(int row, int col)
{
    // TODO: Compute the normal at the given row and column using the positions of the
    //       neighboring vertices.
    glm::vec3 P = getPosition(row, col);
    std::vector<glm::vec3> pts;

    pts.push_back(getPosition(row+1, col+1));
    pts.push_back(getPosition(row+1, col));
    pts.push_back(getPosition(row+1, col-1));
    pts.push_back(getPosition(row, col-1));
    pts.push_back(getPosition(row-1, col-1));
    pts.push_back(getPosition(row-1, col));
    pts.push_back(getPosition(row-1, col+1));
    pts.push_back(getPosition(row, col+1));

    glm::vec3 normal = glm::vec3(0);
    for(int n = 0; n < 8; n++)
    {
        normal += (1.f/8.f)*glm::normalize(glm::cross((pts[n] - P), (pts[(n+1)%8] - P)));
    }
    return normal;
}

float Terrain::heightVal(int ix)
{
    if(ix < 0 || ix >= m_numCols*m_numRows)
    {
        return 0.f;
    }else
    {
        return m_heights[ix];
    }
}

bool Terrain::validRow(int row)
{
    return row >= 0 && row < m_numRows;
}
bool Terrain::validCol(int col)
{
    return col >= 0 && col < m_numCols;
}

int Terrain::getRow(int ix, int w)
{
    if(ix >= 0)
    {
        return ix/w;//This relies on integer division

    }else
    {
        return -1;
    }
}

int Terrain::getCol(int ix, int w)
{
    if(ix >= 0)
    {
        return ix%w;
    }else
    {
        return -1;
    }
}

void Terrain::initHeights()
{
    /*
     * TODO: diamond-square algorithm
     * -make a diamond queue and a square queue, start with 1 in diamond queue
     *
     * diamonds will be listed as: 1---2
     *                             | 5 |
     *                             3---4
     * squares will be listed as:
     *                              /1\
     *                             2 5 3
     *                              \4/
     *
     *
     * while there are diamonds:
     *      for each diamond in queue:
     *          compute diamond
     *          add the squares in 4 directions
     *      for each square in queue:
     *          compute square
     *          add the diamonds in 4 possible directions
     *
     */
    QQueue<dmSq> squares = QQueue<dmSq>();
    QQueue<dmSq> diamonds = QQueue<dmSq>();

    float startNoise = 3.5f;

    std::vector<int> inits = std::vector<int>();

    dmSq init = dmSq(0,
                     ix(m_numCols - 1, 0, m_numCols),
                     ix(0, m_numRows - 1, m_numCols),
                     ix(m_numRows-1, m_numCols-1, m_numCols),
                     ix((m_numRows-1)/2, (m_numCols-1)/2, m_numCols));
    diamonds.enqueue(init);
    while(diamonds.length() > 0)
    {
        while(diamonds.length() > 0)
        {
            dmSq curDiam = diamonds.dequeue();
            if(m_heights[curDiam.v_5] == -100)
            {
                float avg = (heightVal(curDiam.v_1)+
                            heightVal(curDiam.v_2)+
                            heightVal(curDiam.v_3)+
                            heightVal(curDiam.v_4))/4.0;
                m_heights[curDiam.v_5] = avg + (((double) rand() / (RAND_MAX)) - .5)*startNoise;
                //Check each of the 4 diamonds

                //First, make sure the midpoint is a whole number
                //Add diamond one, up
                int top;
                //Make sure the new top is in the square
                int uDist = getRow(curDiam.v_5, m_numCols) - getRow(curDiam.v_1, m_numCols);
                if(getRow(curDiam.v_1, m_numCols) - uDist >= 0)
                {
                    top = ix(getCol(curDiam.v_5, m_numCols), getRow(curDiam.v_1, m_numCols) - uDist, m_numCols);
                }else
                {
                    top = -1;
                }
                int target = ix(getCol(curDiam.v_5, m_numCols), getRow(curDiam.v_1, m_numCols), m_numCols);
                squares.enqueue(dmSq(top, curDiam.v_1, curDiam.v_2, curDiam.v_5, target));
                //Otherwise, we're done!
                if(target == 174)
                {
                    //Yee
                    int x = 1;
                }
                //Add diamond two, down
                int down;
                //Make sure the new top is in the square
                int dDist = getRow(curDiam.v_3, m_numCols) - getRow(curDiam.v_5, m_numCols);
                if(getRow(curDiam.v_3, m_numCols) + dDist < m_numRows)
                {
                    down = ix(getCol(curDiam.v_5, m_numCols), getRow(curDiam.v_3, m_numCols) + dDist, m_numCols);
                }else
                {
                    down = -1;
                }
                target = ix(getCol(curDiam.v_5, m_numCols), getRow(curDiam.v_3, m_numCols), m_numCols);
                squares.enqueue(dmSq(curDiam.v_5, curDiam.v_3, curDiam.v_4, down, target));
                //Otherwise, we're done!
                if(target == 174)
                {
                    //Yee
                    int x = 1;
                }
                //Add diamond three, left
                int left;
                //Make sure the new top is in the square
                int lDist = getCol(curDiam.v_5, m_numCols) - getCol(curDiam.v_1, m_numCols);
                if(getCol(curDiam.v_1, m_numCols) - lDist >= 0)
                {
                    left = ix(getCol(curDiam.v_1, m_numCols) - lDist, getRow(curDiam.v_5, m_numCols),m_numCols);
                }else
                {
                    left = -1;
                }
                target = ix(getCol(curDiam.v_1, m_numCols), getRow(curDiam.v_5, m_numCols), m_numCols);
                squares.enqueue(dmSq(curDiam.v_1, left, curDiam.v_5, curDiam.v_3, target));
                //Otherwise, we're done!
                if(target == 174)
                {
                    //Yee
                    int x = 1;
                }
                //Add diamond four, right
                int right;
                //Make sure the new top is in the square
                int rDist = getCol(curDiam.v_4, m_numCols) - getCol(curDiam.v_5, m_numCols);
                if(getCol(curDiam.v_4, m_numCols) + rDist < m_numCols)
                {
                    right = ix(getCol(curDiam.v_4, m_numCols) + rDist, getRow(curDiam.v_5, m_numCols), m_numCols);
                }else
                {
                    right = -1;
                }
                target = ix(getCol(curDiam.v_4, m_numCols), getRow(curDiam.v_5, m_numCols), m_numCols);
                squares.enqueue(dmSq(curDiam.v_2, curDiam.v_5, right, curDiam.v_4, target));

                //Otherwise, we're done!
                if(target == 174)
                {
                    //Yee
                    int x = 1;
                }

            }
        }

        while(squares.length() > 0)
        {
            dmSq curSq = squares.dequeue();
            //Make sure there's still iterations to do
            if(m_heights[curSq.v_5] == -100)
            {
                float avg = (heightVal(curSq.v_1)+
                heightVal(curSq.v_2)+
                heightVal(curSq.v_3)+
                heightVal(curSq.v_4))/4.0;
                m_heights[curSq.v_5] = avg + (((double) rand() / (RAND_MAX)) -.5)*startNoise;

                if(getCol(curSq.v_5, m_numCols) - getCol(curSq.v_2, m_numCols) > 1 || getRow(curSq.v_5, m_numCols) - getRow(curSq.v_1, m_numCols) > 1)
                {
                    //Now, add those diamonds!

                    int upRow = getRow(curSq.v_1, m_numCols);
                    int downRow = getRow(curSq.v_4, m_numCols);
                    int leftCol = getCol(curSq.v_2, m_numCols);
                    int rightCol = getCol(curSq.v_3, m_numCols);

                    int tupRow = (getRow(curSq.v_5, m_numCols) + getRow(curSq.v_1, m_numCols))/2;
                    int tdownRow = (getRow(curSq.v_5, m_numCols) + getRow(curSq.v_4, m_numCols))/2;
                    int tleftCol = (getCol(curSq.v_5, m_numCols) + getCol(curSq.v_2, m_numCols))/2;
                    int trightCol = (getCol(curSq.v_5, m_numCols) + getCol(curSq.v_3, m_numCols))/2;

                    //Upper left diamond
                    if(validRow(upRow) && validCol(leftCol))
                    {
                        int p1 = ix(leftCol, upRow, m_numCols);
                        int target = ix(tleftCol, tupRow, m_numCols);
                        diamonds.enqueue(dmSq(p1, curSq.v_1, curSq.v_2, curSq.v_5, target));
                        if(target == 174)
                        {
                            //Yee
                            int x = 1;
                        }
                    }

                    //Upper right diamond
                    if(validRow(upRow) && validCol(rightCol))
                    {
                        int p2 = ix(rightCol, upRow, m_numCols);
                        int target = ix(trightCol, tupRow, m_numCols);
                        diamonds.enqueue(dmSq(curSq.v_1, p2, curSq.v_5, curSq.v_3, target));
                        if(target == 174)
                        {
                            //Yee
                            int x = 1;
                        }
                    }

                    //Lower left diamond
                    if(validRow(downRow) && validCol(leftCol))
                    {
                        int p3 = ix(leftCol, downRow, m_numCols);
                        int target = ix(tleftCol, tdownRow, m_numCols);
                        diamonds.enqueue(dmSq(curSq.v_2, curSq.v_5, p3, curSq.v_4, target));
                        if(target == 174)
                        {
                            //Yee
                            int x = 1;
                        }
                    }

                    //Lower right diamond
                    if(validRow(downRow) && validCol(rightCol))
                    {
                        int p4 = ix(rightCol, downRow, m_numCols);
                        int target = ix(trightCol, tdownRow, m_numCols);
                        diamonds.enqueue(dmSq(curSq.v_5, curSq.v_3, curSq.v_4, p4, target));
                        if(target == 174)
                        {
                            //Yee
                            int x = 1;
                        }
                    }
                }
            }
        }
        startNoise *= .4;
    }
}

/**
 * Initializes the terrain by storing positions and normals in a vertex buffer.
 */
void Terrain::init()
{
    m_shape.create();
    // TODO: Change from GL_LINE to GL_FILL in order to render full triangles instead of wireframe.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    std::cout<<(m_numRows - 1) * (2*m_numCols + 2)<<"\n";

    initHeights();

    // Initializes a grid of vertices using triangle strips.
    int numVertices = (m_numRows - 1) * (2*m_numCols + 2);
    std::vector<glm::vec3> data(3 * numVertices);
    int index = 0;
    for (int row = 0; row < m_numRows - 1; row++)
    {
        for (int col = m_numCols - 1; col >= 0; col--)
        {
            data[index++] = getPosition(row, col);
            data[index++] = getNormal  (row, col);
            data[index++] = glm::vec3(static_cast<float>(col)/static_cast<float>(m_numCols),
                                      static_cast<float>(row)/static_cast<float>(m_numRows),
                                      0.f);

            data[index++] = getPosition(row + 1, col);
            data[index++] = getNormal  (row + 1, col);
            data[index++] = glm::vec3(static_cast<float>(col)/static_cast<float>(m_numCols),
                                      static_cast<float>(row+1)/static_cast<float>(m_numRows),
                                      0.f);
        }
        data[index++] = getPosition(row + 1, 0);
        data[index++] = getNormal  (row + 1, 0);
        data[index++] = glm::vec3(static_cast<float>(0)/static_cast<float>(m_numCols),
                                  static_cast<float>(row+1)/static_cast<float>(m_numRows),
                                  0.f);

        data[index++] = getPosition(row + 1, m_numCols - 1);
        data[index++] = getNormal  (row + 1, m_numCols - 1);
        data[index++] = glm::vec3(static_cast<float>(m_numCols-1)/static_cast<float>(m_numCols),
                                  static_cast<float>(row+1)/static_cast<float>(m_numRows),
                                  0.f);
    }

    // Initialize OpenGLShape.
    m_shape.setVertexData(&data[0][0], 3 * sizeof(glm::vec3) * numVertices, GL_TRIANGLE_STRIP, numVertices);
    m_shape.setAttribute(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), 0);
    m_shape.setAttribute(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), sizeof(glm::vec3));
    m_shape.setAttribute(2, 3, GL_FLOAT, GL_FALSE, 3*sizeof(glm::vec3), 2*sizeof(glm::vec3));

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    QImage image(":/images/grass.jpg");

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
}






/**
 * Draws the terrain.
 */
void Terrain::draw()
{
    //TODO: make sure I'm using the right shaders
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    m_shape.draw();
    glBindTexture(GL_TEXTURE_2D, 0);
}
