#include "Shape.h"
#include "glm.hpp"
#include <iostream>

Shape::Shape(int t1, int t2, int t3)
    :m_t1(t1), m_t2(t2), m_t3(t3)
{
}

Shape::~Shape(){
    std::cout << "deleting shape, nothing done" << std::endl;


}

void Shape::init(){
    // Initialize the vertex array object.
    glGenVertexArrays(1, &m_vaoID);
    // Initialize the vertex buffer object.
    glGenBuffers(1, &m_vboID);
}

void Shape::setT1(int t1){
    m_t1 = t1;
}

void Shape::setT2(int t2){
    m_t2 = t2;
}

void Shape::setT3(int t3){
    m_t3 = t3;
}

void Shape::bufferVerts(GLuint shader){

    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
    glBindVertexArray(m_vaoID);
    glBufferData(GL_ARRAY_BUFFER, getNumVerts() * 6 * sizeof(GLfloat), &m_vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(glGetAttribLocation(shader, "position"));
    glEnableVertexAttribArray(glGetAttribLocation(shader, "normal"));
    glVertexAttribPointer(
        glGetAttribLocation(shader, "position"),
        3,                   // Num coordinates per position
        GL_FLOAT,            // Type
        GL_FALSE,            // Normalized
        sizeof(GLfloat) * 6, // Stride
        (void*) 0            // Array buffer offset
    );
    glVertexAttribPointer(
        glGetAttribLocation(shader, "normal"),
        3,           // Num coordinates per normal
        GL_FLOAT,    // Type
        GL_TRUE,     // Normalized
        sizeof(GLfloat) * 6,           // Stride
        (void*) (sizeof(GLfloat) * 3)    // Array buffer offset
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Shape::draw(){
    glBindVertexArray(m_vaoID);
    glDrawArrays(GL_TRIANGLES, 0, getNumVerts() /* Number of vertices to draw */);
    glBindVertexArray(0);

}

int Shape::getNumVerts(){
    return m_vertices.size() / 6;
}

void Shape::pushVertex(glm::vec4 vert, glm::vec4 norm){
    m_vertices.push_back(vert.x);
    m_vertices.push_back(vert.y);
    m_vertices.push_back(vert.z);
    m_vertices.push_back(norm.x);
    m_vertices.push_back(norm.y);
    m_vertices.push_back(norm.z);
}
