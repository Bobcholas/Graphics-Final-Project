#include "openglshape.h"

OpenGLShape::OpenGLShape() :
    m_vboID(0),
    m_vaoID(0),
    m_numVertices(0),
    m_drawMode(GL_TRIANGLES)
{
    glGenBuffers(1, &m_vboID);
    glGenVertexArrays(1, &m_vaoID);
}

OpenGLShape::~OpenGLShape()
{
    glDeleteBuffers(1, &m_vboID);
    glDeleteVertexArrays(1, &m_vaoID);
}


/**
 * @param data - Array of floats containing the vertex data.
 * @param size - Size of the data array, in bytes.
 * @param drawMode - Drawing mode. (e.g. GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN)
 * @param numVertices - Number of vertices to be rendered.
 */
void OpenGLShape::setVertexData(GLfloat *data, GLsizeiptr size, GLenum drawMode, int numVertices)
{
    // Stores the draw mode and number of vertices, which will be used for drawing later.
    m_drawMode = drawMode;
    m_numVertices = numVertices;

    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


/**
 * @param index - Index of the vertex attribute.
 * @param size - Number of components for the attribute. (e.g. 3 for a vec3)
 * @param type - Data type of the components. (e.g. GL_FLOAT)
 * @param normalized - Whether integers should be converted to floats (just use GL_FALSE for this)
 * @param stride - Byte offset between each vertex in the data array.
 * @param pointer - Byte offset of this attribute from the beginning of the data array.
 */
void OpenGLShape::setAttribute(
        GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, size_t pointer)
{
    glBindVertexArray(m_vaoID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, type, normalized, stride, (void*) pointer);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void OpenGLShape::draw()
{
    glBindVertexArray(m_vaoID);
    glDrawArrays(m_drawMode, 0, m_numVertices);
    glBindVertexArray(0);
}
