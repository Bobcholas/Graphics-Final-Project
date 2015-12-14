#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include <vector>
#include <QImage>
#include "GL/glew.h"
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
class ParticleManager
{

public:
    ParticleManager(unsigned int maxparticles,std::string texpath);
    ~ParticleManager();
    inline std::vector<Particle> getParticles(){return m_particles;}
    void resetParticles();//resets all particles
    void updateParticles();//one step of physicals calculations and life maintenance for each particle
    void resetParticle(int i);//resets particle at given index
    void setInitialPosition(glm::vec3 initialposition);
    float randFloat(float lo, float hi);
    inline void setScale(float scale){m_scale = scale;}
    inline float getScale(){return m_scale;}
    inline QImage getTex(){return m_tex;}
    inline void setColor(glm::vec3 newcolor){m_color = newcolor;}
    inline void setVelocity(glm::vec3 newvelocity){m_velocity = newvelocity;}
    inline void setForce(glm::vec3 newforce){m_force = newforce;}
    inline void setSpeed(float newspeed){m_speed = newspeed;}
    inline void setFuzziness(float newfuzziness){m_fuzziness = newfuzziness;}
    inline void setTexID(GLuint newid){m_textureId=newid;}
    inline GLuint getTexID(){return m_textureId;}
    inline std::string getTexPath(){return m_texpath;}
    inline float getSpeed(){return m_speed;}
    inline float getFuzziness(){return m_fuzziness;}
private:
    std::vector<Particle> m_particles;
    unsigned int m_maxParticles;//particle limit
    float m_speed;//speed particles move in the simulation
    float m_fuzziness;//randomness of particle orginal direction
    float m_scale;//size of each particle
    glm::vec3 m_color; //default particle color from this emitter
    glm::vec3 m_velocity;//initial particle velocity
    glm::vec3 m_force;//force applied on each particle in the scene
    glm::vec3 m_position;//initial position of emitter (and particles)
    QImage m_tex;//path to texmap image
    GLuint m_textureId;//texid
    std::string m_texpath;
};
#endif // PARTICLEMANAGER_H
