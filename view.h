#ifndef VIEW_H
#define VIEW_H
#define GLM_FORCE_RADIANS

#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "GL/glew.h"
#include <QGLWidget>
class OpenGLShape;
#include "ParticleManager.h"
#include <QTime>
#include <QTimer>
#include <memory>

class View : public QGLWidget
{
    Q_OBJECT

public:
    View(QWidget *parent);
    ~View();

private:
    QTime time;
    QTimer timer;

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void drawParticles();//ie opengl stuffs
    void updateParticleManagers();//updates all existing particle managers
    void paintParticles();
    void loadTex(int i);
    void squareData(float scale);
    void rebuildMatrices();
    void setInitialPosition(glm::vec3 initialposition);//sets intiial position of particle emitter
    void paintParticlesGL();
    void initializeParticlesGL();
    void createParticleManager(glm::vec3 initialpos, unsigned int maxp,float scale,std::string texpath,glm::vec3 color, glm::vec3 velocity, float speed,float fuzziness, glm::vec3 force);
    std::unique_ptr<OpenGLShape> m_square;



    GLuint m_textureProgramID;//texture mapping particle shader

    GLuint m_textureId;//texture mapping program
    glm::mat4 m_model, m_view, m_projection;

    /** For mouse interaction. */
    float m_angleX, m_angleY, m_zoom;
    QPoint m_prevMousePos;
    std::vector<std::unique_ptr<ParticleManager>> m_particlemanagers;
    int m_numManagers;
private slots:
    void tick();
};

#endif // VIEW_H

