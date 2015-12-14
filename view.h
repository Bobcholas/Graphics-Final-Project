#ifndef VIEW_H
#define VIEW_H
#define GLM_FORCE_RADIANS

#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "GL/glew.h"
#include <QGLWidget>

#include "ParticleManager.h"
#include "terrain.h"
#include <QTime>
#include <QTimer>
#include <memory>
#include "shapes/Shape.h"
#include "shapes/Statue.h"

class OpenGLShape;


class View : public QGLWidget
{
    Q_OBJECT

public:
    View(QWidget *parent);
    ~View();

private:
    QTime m_time;
    QTimer timer;

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *e);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void drawParticles();//ie opengl stuffs
    void drawTerrain();
    void updateParticleManagers();//updates all existing particle managers
    void paintParticles();
    void loadTex(int i);
    void squareData(float scale);
    void rebuildMatrices();
    void setInitialPosition(glm::vec3 initialposition);//sets intiial position of particle emitter
    void paintParticlesGL();
    void initializeParticlesGL();
    void createParticleManager(glm::vec3 initialpos, unsigned int maxp,float scale,std::string texpath,glm::vec3 color, glm::vec3 velocity, float speed,float fuzziness, glm::vec3 force);
    void createParticleManagerSimple(glm::vec3 initialpos, glm::vec3 force);
    void loadParticleTex();
    GLuint loadSkyBoxTex();
    std::unique_ptr<OpenGLShape> m_square;

    void initStatue();
    void paintStatues();
    void drawStatues();
    void setLight(const CS123SceneLightData &light);
    void addLight(const CS123SceneLightData &sceneLight){
        m_lights.push_back(sceneLight); //this is a copy
    }
    void setLights(const glm::mat4 viewMatrix)
    {
        //set view matrix
        glUniformMatrix4fv(glGetUniformLocation(m_statueShaderProgramID, "v"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
        for (std::list<CS123SceneLightData>::const_iterator iter = m_lights.begin();
            iter != m_lights.end(); iter++){
            setLight(*iter);
        }
    }
    void applyMaterial(const CS123SceneMaterial &material);

    GLuint m_textureProgramID;//texture mapping particle shader
    GLuint m_terrainProgramID;
    GLuint m_terrainNoTexProgramID;

    GLuint m_textureId;//texture mapping program

    GLuint m_statueShaderProgramID;

    glm::mat4 m_model, m_view, m_projection;

    /** For mouse interaction. */
    float m_angleX, m_angleY, m_zoom;
    QPoint m_prevMousePos;
    std::vector<std::unique_ptr<ParticleManager>> m_particlemanagers;
    int m_numManagers;

    std::vector<Statue*> m_statues;

    std::map<std::string, GLint> m_uniformLocs;
    Shape *primitives[5];
    CS123SceneGlobalData m_global;
    std::list<TransPrimitive> m_transPrims;
    std::list<CS123SceneLightData> m_lights;

    Terrain m_terrain;
;
    std::unique_ptr<OpenGLShape> m_quad;
    //skybox stuff
    void initializeSkyBoxGL();
    void paintSkyBoxGL();
    GLuint m_skyBoxTex;
    GLuint* m_pmtex;
    bool m_useTerrainTex;

private slots:
    void tick();

};

#endif // VIEW_H

