#include "glwidget.h"
#include "settings.h"

#include "cs123_lib/resourceloader.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr

#include "openglshape.h"
#include <iostream>

GLWidget::GLWidget(QGLFormat format, QWidget *parent)
    : QGLWidget(format, parent),
      m_square(nullptr),
      m_solidProgramID(0),
      m_gradientProgramID(0),
      m_textureProgramID(0),
      m_textureID(0),
    m_maxParticles(10000),
    m_speed(50.0f/10000.f),
    m_fuzziness(50.0f),
    m_scale(0.1f),
    m_color(glm::vec3(1.0f,0.5f,0.2f)),
    m_velocity(glm::vec3(0.0f,0.0001f,0.0f)),
    m_force(glm::vec3(0.f,0.0001f,0.0f)),
    m_position(glm::vec3(0.f))
{m_particles = std::vector<Particle>(m_maxParticles); resetParticles();}

GLWidget::~GLWidget()
{
    // TODO: Clean up GPU memory.
    glDeleteTextures(1,&m_textureID);
}


void GLWidget::initializeGL()
{
    ResourceLoader::initializeGlew();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Defines the color the screen will be cleared to.

    // Creates the three shader programs.
    m_solidProgramID = ResourceLoader::createShaderProgram(":/shaders/solid.vert", ":/shaders/solid.frag");
    m_gradientProgramID = ResourceLoader::createShaderProgram(":/shaders/gradient.vert", ":/shaders/gradient.frag");
    m_textureProgramID = ResourceLoader::createShaderProgram(":/shaders/texture.vert", ":/shaders/texture.frag");

    squareData();
    // TODO (Task 6): Initialize texture map.
    QImage image(":/images/particle3.jpg");
    glGenTextures(1,&m_textureID);
    glBindTexture(GL_TEXTURE_2D,m_textureID);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,image.width(),image.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,image.bits());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);//param?
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE,GL_ONE);

}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        updateParticles();
        drawParticles();

}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}
void GLWidget::resetParticles(){
    for(int i=0;i<m_particles.size();i++){
        resetParticle(i);
    }
}
void GLWidget::resetParticle(int i){
    //std::cout << "resetting particle" << i << std::endl;
    Particle curparticle = m_particles.at(i);
    curparticle.pos = glm::vec3(0.f,0.f,0.f);//probably am going to need to have some other default position in world space?
    curparticle.life = 1.0f;
    curparticle.decay = randFloat(0.0025f,0.15f);
    curparticle.color = m_color;
    curparticle.force = glm::vec3(randFloat(-m_fuzziness*0.01f,(m_fuzziness*0.01f))+m_force.x,randFloat(-m_fuzziness*0.01f,(m_fuzziness*0.01f))+m_force.y,randFloat(-m_fuzziness*0.01f,(m_fuzziness*0.01f))+m_force.z);
    curparticle.dir = glm::vec3(randFloat(-m_fuzziness,m_fuzziness)+m_velocity.x,randFloat(-m_fuzziness,m_fuzziness)+m_velocity.y,randFloat(-m_fuzziness,m_fuzziness)+m_velocity.z);
    m_particles[i] = curparticle;
}
void GLWidget::updateParticles(){
    Particle curparticle;
    for(int i=0;i<m_particles.size();i++){
        curparticle = m_particles.at(i);
        //std::cout << curparticle.pos.x <<" "<< curparticle.pos.y << " "<<curparticle.pos.z <<std::endl;
        //std::cout << "dir "<<curparticle.dir.x <<" "<< curparticle.dir.y <<" "<< curparticle.dir.z <<std::endl;
        curparticle.pos+= m_particles.at(i).dir*m_speed;
        //std::cout << "after "<<curparticle.pos.x <<" "<< curparticle.pos.y << curparticle.pos.z <<std::endl;
        curparticle.dir +=m_particles.at(i).force;
        curparticle.life-=m_particles.at(i).decay;
        if(curparticle.life<0){
            resetParticle(i);
        }
        m_particles[i] = curparticle;
    }
}
void GLWidget::loadTex(){
    QImage image(":/images/particle3.jpg");//could be set later, just using one of the examples
    glGenTextures(1,&m_textureId);//texture id is something else from the shader program name
    glBindTexture(GL_TEXTURE_2D,m_textureId);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,image.width(),image.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,image.bits());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);//param?
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
}
void GLWidget::squareData(){
    //probably using code similar to quad in lab3?

    // Smart pointer!
    m_square.reset(new OpenGLShape());

    // TODO (Task 1): Initialize m_square.
    float staticFanArray[36] = {-m_scale,m_scale,0.0f,//top left
                                1.0f,0.0f,0.0f, //red
                                0.0f,0.0f, //u1,v1

                                -m_scale,-m_scale,0.0f,// bottom left
                                0.0f,1.0f,0.0f,//green
                                0.0f,1.0f, //u2,v2


                                m_scale,m_scale,0.0f,// top right
                                1.0f,1.0f,0.0f,//yellow
                                1.0f,0.0f, //u5,v5

                                m_scale,-m_scale,0.0f,//bottom right
                                0.0f,0.0f,1.0f,// also blue
                                1.0f,1.0f };// u6,v6
    m_square->setVertexData(staticFanArray,sizeof(staticFanArray),GL_TRIANGLE_STRIP,4);
    m_square->setAttribute(0,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),0);
    // TODO (Task 3): Interleave positions and colors in the array used to intialize m_square
    m_square->setAttribute(1,3,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),3*sizeof(GLfloat));
    // TODO (Task 7): Interleave UV-coordinates along with positions and colors in your VBO
    m_square->setAttribute(2,2,GL_FLOAT,GL_FALSE,8*sizeof(GLfloat),(3+3)*sizeof(GLfloat));
}
//this probably gets called in some kind of paint method
void GLWidget::drawParticles(){

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Defines the color the screen will be cleared to.
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //generally in some type of paintgl function, could separqate out later
    glm::vec3 eye = glm::vec3(0.f, 1.f, 4.f);
    glm::vec3 center = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);

    glm::mat4 proj = glm::perspective(0.8f, (float)width()/height(), 0.1f, 100.f);
    glm::mat4 view = glm::lookAt(eye, center, up);
    glm::mat4 model(1.f);
    glUseProgram(m_textureProgramID);

    // Sets projection and view matrix uniforms.
    glUniformMatrix4fv(glGetUniformLocation(m_textureProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(glGetUniformLocation(m_textureProgramID, "view"), 1, GL_FALSE, glm::value_ptr(view));

    //get model, etc. into the vertex shader, then translate by positiion
    for(int i = 0;i<m_particles.size();i++){
        //glUniform3fv(glGetUniformLocation(m_textureProgramID, "particlepos"),1,glm::value_ptr(m_particles.at(i).pos));
        model = glm::translate(glm::mat4(1.f),m_particles.at(i).pos);
        glUniformMatrix4fv(glGetUniformLocation(m_textureProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        //std::cout << "drawing squares" << std::endl;
        m_square->draw();
    }
    glUseProgram(0);//stop using texmapping program
}
float GLWidget::randFloat(float lo, float hi){
    return lo + static_cast<float>(rand()) /( static_cast<float>(RAND_MAX/(hi-lo)));
}
void GLWidget::paintParticles(){
    //possibly clear some buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    updateParticles();
    drawParticles();
}
