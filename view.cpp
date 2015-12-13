#include "view.h"

#include <QApplication>
#include <QKeyEvent>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr

#include "openglshape.h"
#include "particlesystem/cs123_lib/resourceloader.h"
#include "iostream"
#include "ParticleManager.h"
View::View(QWidget *parent) : QGLWidget(parent),
    m_square(nullptr),
    m_textureProgramID(0),
    m_textureId(0),
    m_angleX(0), m_angleY(0.5f), m_zoom(10.f),m_numManagers(0),m_skyBoxTex(0)
{
    // View needs all mouse move events, not just mouse drag events
    //setMouseTracking(true);

    // Hide the cursor since this is a fullscreen app
    //setCursor(Qt::BlankCursor);

    // View needs keyboard focus
    setFocusPolicy(Qt::StrongFocus);

    // The game loop is implemented using a timer
    connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));
    //m_particles = std::vector<Particle>(m_maxParticles); resetParticles(); now handled in particle managers
    createParticleManager(glm::vec3(0.f),100,0.5f,":/images/particle3.jpg",glm::vec3(1.0f,0.5f,0.2f),glm::vec3(0.0f,0.0001f,0.0f),(25.0f/10000.f),50.0f,glm::vec3(0.f,0.001f,0.0f));
    createParticleManager(glm::vec3(3.f),300,0.1f,":/images/particle2.bmp",glm::vec3(1.0f,0.5f,0.2f),glm::vec3(0.0f,0.0001f,0.0f),(80.0f/100000.f),25.0f,glm::vec3(0.f,0.0001f,0.0f));
    //createParticleManager(glm::vec3(10.f),20,0.1f,":/images/particle1.bmp",glm::vec3(1.0f,0.5f,0.2f),glm::vec3(0.0f,20.0001f,20.0f),(80.0f/100000.f),25.0f,glm::vec3(0.f,0.0001f,0.0f));
}

View::~View()
{
    // TODO: Clean up GPU memory.
    glDeleteTextures(m_particlemanagers.size(),m_pmtex);
    delete[] m_pmtex;
    glDeleteTextures(1,&m_textureId);
}

void View::initializeGL()
{
    // All OpenGL initialization *MUST* be done during or after this
    // method. Before this method is called, there is no active OpenGL
    // context and all OpenGL calls have no effect.

    // Start a timer that will try to get 60 frames per second (the actual
    // frame rate depends on the operating system and other running programs)
    time.start();
    timer.start(1000 / 60);

    // Center the mouse, which is explained more in mouseMoveEvent() below.
    // This needs to be done here because the mouse may be initially outside
    // the fullscreen window and will not automatically receive mouse move
    // events. This occurs if there are two monitors and the mouse is on the
    // secondary monitor.
    QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
    ResourceLoader::initializeGlew();
    initializeSkyBoxGL();
    initializeParticlesGL();

}

void View::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO: Implement the demo rendering here

    paintSkyBoxGL();
    paintParticlesGL();
}

void View::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    rebuildMatrices();
}

void View::mousePressEvent(QMouseEvent *event)
{
    m_prevMousePos = event->pos();
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    // This starter code implements mouse capture, which gives the change in
    // mouse position since the last mouse movement. The mouse needs to be
    // recentered after every movement because it might otherwise run into
    // the edge of the screen, which would stop the user from moving further
    // in that direction. Note that it is important to check that deltaX and
    // deltaY are not zero before recentering the mouse, otherwise there will
    // be an infinite loop of mouse move events.
    /**
    int deltaX = event->x() - width() / 2;
    int deltaY = event->y() - height() / 2;
    if (!deltaX && !deltaY) return;
    **/
    //QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));

    // TODO: Handle mouse movements here
    m_angleX += 10 * (event->x() - m_prevMousePos.x()) / (float) width();
    m_angleY += 10 * (event->y() - m_prevMousePos.y()) / (float) height();
    m_prevMousePos = event->pos();
    rebuildMatrices();
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
}

void View::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) QApplication::quit();

    // TODO: Handle keyboard presses here
}
void View::wheelEvent(QWheelEvent *event)
{
    m_zoom -= event->delta() / 100.f;
    rebuildMatrices();
}

void View::keyReleaseEvent(QKeyEvent *event)
{
}

void View::tick()
{void createParticleManager(glm::vec3 initialpos, unsigned int maxp);
    // Get the number of seconds since the last tick (variable update rate)
    float seconds = time.restart() * 0.001f;

    // TODO: Implement the demo update here

    // Flag this view for repainting (Qt will call paintGL() soon after)
    update();
}

void View::loadTex(int i){
    QImage image=m_particlemanagers.at(i)->getTex();
    GLuint pmid = m_particlemanagers.at(i)->getTexID();
    //std::cout << pmid <<std::endl;
    glGenTextures(1,&pmid);
    glBindTexture(GL_TEXTURE_2D,pmid);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,image.width(),image.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,image.bits());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);//param?
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
}
void View::squareData(float scale){
    //probably using code similar to quad in lab3?

    // Smart pointer!
    m_square.reset(new OpenGLShape());

    // TODO (Task 1): Initialize m_square.
    float staticFanArray[36] = {-scale,scale,0.0f,//top left
                                1.0f,0.0f,0.0f, //red
                                0.0f,0.0f, //u1,v1

                                -scale,-scale,0.0f,// bottom left
                                0.0f,1.0f,0.0f,//green
                                0.0f,1.0f, //u2,v2


                                scale,scale,0.0f,// top right
                                1.0f,1.0f,0.0f,//yellow
                                1.0f,0.0f, //u5,v5

                                scale,-scale,0.0f,//bottom right
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
void View::drawParticles(){

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Defines the color the screen will be cleared to.
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glm::mat4 translatemodel(1.f);
    glUseProgram(m_textureProgramID);

    // Sets projection and view matrix uniforms.
    glUniformMatrix4fv(glGetUniformLocation(m_textureProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
    glUniformMatrix4fv(glGetUniformLocation(m_textureProgramID, "view"), 1, GL_FALSE, glm::value_ptr(m_view));
    //get model, etc. into the vertex shader, then translate by positiion
    std::vector<Particle> curparticles;
    for(int j = 0;j<m_particlemanagers.size();j++){
        curparticles = m_particlemanagers.at(j)->getParticles();
        squareData(m_particlemanagers.at(j)->getScale());
        //bind tex + other methods
        glBindTexture(GL_TEXTURE_2D,m_pmtex[j]);
        for(int i = 0;i<curparticles.size();i++){
            //create translation transformation
            translatemodel = glm::translate(glm::mat4(1.f),curparticles.at(i).pos);
            glUniformMatrix4fv(glGetUniformLocation(m_textureProgramID, "model"), 1, GL_FALSE, glm::value_ptr(translatemodel *m_model));
            //drawing particle
            m_square->draw();
        }
    }
    glUseProgram(0);//stop using texmapping program
}

void View::paintParticles(){
    //possibly clear some buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    updateParticleManagers();
    drawParticles();
}


void View::rebuildMatrices()
{
    m_model = glm::mat4(1.f);
    m_view = glm::translate(glm::vec3(0, 0, -m_zoom)) *
             glm::rotate(m_angleY, glm::vec3(1,0,0)) *
             glm::rotate(m_angleX, glm::vec3(0,1,0));
    m_projection = glm::perspective(0.8f, (float)width()/height(), 0.1f, 100.f);
    update();
}

void View::initializeParticlesGL()
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Defines the color the screen will be cleared to.

    // Creates the three shader programs.
    m_textureProgramID = ResourceLoader::createShaderProgram(":/shaders/texture.vert", ":/shaders/texture.frag");
    loadParticleTex();
    //squareData(); handled on case by case basis



}

void View::paintParticlesGL()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);//could use GL_SRC_ALPHA, experiment
    updateParticleManagers();
    drawParticles();
    glDisable(GL_BLEND);
}
/**
void View::resetParticles(){
    for(int i=0;i<m_particles.size();i++){
        resetParticle(i);
    }
}
**/
void View::updateParticleManagers(){
    for(int i = 0;i<m_particlemanagers.size();i++){
        m_particlemanagers.at(i)->updateParticles();
    }
}
/**
 * @brief View::createParticleManager Creates particle manager with different parameters depending on the arguments
 * @param initialpos: initial position of the particle manager
 * @param maxp: max particles
 * @param scale: particle scale.
 * @param texpath: path to texture map
 * @param color: color of particles
 * @param velocity: particle velocity
 * @param speed: speed of particle simulation
 * @param fuzziness: randomness of particle orginal direction
 * @param force: force applied on each particle in the scene
 */
void View::createParticleManager(glm::vec3 initialpos, unsigned int maxp,float scale,std::string texpath,glm::vec3 color, glm::vec3 velocity, float speed,float fuzziness, glm::vec3 force){
    std::unique_ptr<ParticleManager> newpm(new ParticleManager(maxp,texpath));
    newpm->setInitialPosition(initialpos);
    newpm->setScale(scale);
    newpm->setColor(color);
    newpm->setVelocity(velocity);
    newpm->setSpeed(speed);
    newpm->setFuzziness(fuzziness);
    newpm->setForce(force);
    newpm->setTexID(m_numManagers);
    m_particlemanagers.push_back(std::move(newpm));
    m_numManagers+=1;

}
void View::initializeSkyBoxGL(){
    m_skyBoxTex = ResourceLoader::createShaderProgram(":/shaders/textureVanilla.vert", ":/shaders/texture1.frag");
    //input cube data
    m_quad.reset(new OpenGLShape());
    GLfloat skyboxVertices[] = {
            // Positions
            -20.0f,  20.0f, -20.0f,
            -20.0f, -20.0f, -20.0f,
             20.0f, -20.0f, -20.0f,
             20.0f, -20.0f, -20.0f,
             20.0f,  20.0f, -20.0f,
            -20.0f,  20.0f, -20.0f,

            -20.0f, -20.0f,  20.0f,
            -20.0f, -20.0f, -20.0f,
            -20.0f,  20.0f, -20.0f,
            -20.0f,  20.0f, -20.0f,
            -20.0f,  20.0f,  20.0f,
            -20.0f, -20.0f,  20.0f,

             20.0f, -20.0f, -20.0f,
             20.0f, -20.0f,  20.0f,
             20.0f,  20.0f,  20.0f,
             20.0f,  20.0f,  20.0f,
             20.0f,  20.0f, -20.0f,
             20.0f, -20.0f, -20.0f,

            -20.0f, -20.0f,  20.0f,
            -20.0f,  20.0f,  20.0f,
             20.0f,  20.0f,  20.0f,
             20.0f,  20.0f,  20.0f,
             20.0f, -20.0f,  20.0f,
            -20.0f, -20.0f,  20.0f,

            -20.0f,  20.0f, -20.0f,
             20.0f,  20.0f, -20.0f,
             20.0f,  20.0f,  20.0f,
             20.0f,  20.0f,  20.0f,
            -20.0f,  20.0f,  20.0f,
            -20.0f,  20.0f, -20.0f,

            -20.0f, -20.0f, -20.0f,
            -20.0f, -20.0f,  20.0f,
             20.0f, -20.0f, -20.0f,
             20.0f, -20.0f, -20.0f,
            -20.0f, -20.0f,  20.0f,
             20.0f, -20.0f,  20.0f
        };
    m_quad->setVertexData(skyboxVertices,18*6*sizeof(float),GL_TRIANGLES,18*6*2);
    m_quad->setAttribute(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),0);
    //m_quad->setAttribute(1,2,GL_FLOAT,GL_FALSE,5*sizeof(GLfloat),3*sizeof(GLfloat));
    loadSkyBoxTex();
}
//skybox methods derived from http://learnopengl.com/#!Advanced-OpenGL/Cubemaps
void View::paintSkyBoxGL(){
    glEnable(GL_DEPTH_TEST);

    glUseProgram(m_skyBoxTex);
    glm::mat4x4 scalemodel(1.0f);
    scalemodel = glm::scale(glm::mat4(1.f),glm::vec3(10,10,10.f));;
    glUniformMatrix4fv(glGetUniformLocation(m_skyBoxTex, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
    glUniformMatrix4fv(glGetUniformLocation(m_skyBoxTex, "view"), 1, GL_FALSE, glm::value_ptr(m_view));
    glUniformMatrix4fv(glGetUniformLocation(m_skyBoxTex, "model"), 1, GL_FALSE, glm::value_ptr(m_model));

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);
    m_quad->draw();
    glUseProgram(0);
    glDisable(GL_DEPTH_TEST);
}
GLuint View::loadSkyBoxTex(){

    //right left top bottom back front
    std::vector<QImage> boximages;
    boximages.push_back(QImage(":/images/right.jpg"));
    boximages.push_back(QImage(":/images/left.jpg"));
    boximages.push_back(QImage(":/images/top.jpg"));
    boximages.push_back(QImage(":/images/bottom.jpg"));
    boximages.push_back(QImage(":/images/back.jpg"));
    boximages.push_back(QImage(":/images/front.jpg"));
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);
    QImage image;
    for(GLuint i = 0; i < 6; i++)
    {
        image = boximages.at(i);
        //glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,0,GL_RGBA,image.width(),image.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,image.bits());

    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return m_textureId;
}
//create particlemanager first, then load all textures, store in array of GLuint
//inspired by http://www.gamasutra.com/view/feature/131768/understanding_and_using_opengl_.php?page=5
void View::loadParticleTex(){\
    m_pmtex = new GLuint[m_particlemanagers.size()];
    glGenTextures(m_particlemanagers.size(),m_pmtex);
    GLuint pmid;
    QImage image;
    for(int i = 0;i<m_particlemanagers.size();i++){
        image=m_particlemanagers.at(i)->getTex();
        pmid = m_particlemanagers.at(i)->getTexID();
        //std::cout << pmid <<std::endl;

        glBindTexture(GL_TEXTURE_2D,m_pmtex[i]);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,image.width(),image.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,image.bits());
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);//param?
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    }
}
