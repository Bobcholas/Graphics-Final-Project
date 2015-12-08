#ifndef GLWIDGET_H
#define GLWIDGET_H

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr

#include <memory>
#include <QGLWidget>
#include <QTimer>

class OpenGLShape;

class GLWidget : public QGLWidget
{
    Q_OBJECT
    struct Particle{
        // particle struct containing necessary attributes
        bool active;//determines whether or not particle is active. If active, it should not be draw or modified by update particles()
        float life;//initially 1.0, decreases by decay, on 0, resets
        float decay;// see life
        glm::vec3 color;//particle color
        glm::vec3 pos; //particle's current location in 3D worldspace? updated each step based on velocity
        glm::vec3 dir;// direction particle is currently moving
        glm::vec3 force;//force acting on particle (gravity), this affects direction by making dir +=force
    };
public:
    GLWidget(QGLFormat format, QWidget *parent = 0);
    ~GLWidget();
    void drawParticles();//ie opengl stuffs
    void resetParticle(int i);//resets particle at given index
    void resetParticles();//resets all particles
    void updateParticles();//one step of physicals calculations and life maintenance for each particle
    static float randFloat(float lo, float hi);
    void paintParticles();
    void loadTex();
    void squareData();
protected:
    /** This is called once, before any calls to paintGL. */
    void initializeGL();

    /** This is called every time the window needs to be repainted. */
    void paintGL();

    void resizeGL(int w, int h);
    std::vector<Particle> m_particles;
    unsigned int m_maxParticles;
    GLuint m_textureId;
    float m_speed;
    float m_fuzziness;//randomness of particle orginal direction
    float m_scale;//size of each particle
    glm::vec3 m_color; //default partilce color from this emitter
    glm::vec3 m_velocity;//initial particle velosity
    glm::vec3 m_force;//force applied on each particle in the scene
    glm::vec3 m_position;//initial position of emitter (and particles)
private:
    std::unique_ptr<OpenGLShape> m_square;

    GLuint m_solidProgramID;
    GLuint m_gradientProgramID;
    GLuint m_textureProgramID;

    GLuint m_textureID;
};

#endif // GLWIDGET_H
