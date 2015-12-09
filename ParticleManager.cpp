#include "ParticleManager.h"
ParticleManager::ParticleManager(unsigned int maxparticles,std::string texpath): m_maxParticles(10000),
  m_speed(50.0f/10000.f),
  m_fuzziness(50.0f),
  m_scale(0.3f),
  m_color(glm::vec3(1.0f,0.5f,0.2f)),
  m_velocity(glm::vec3(0.0f,0.0001f,0.0f)),
  m_force(glm::vec3(0.f,0.0001f,0.0f)),
  m_position(glm::vec3(0.f)),m_tex(nullptr),m_textureId(0)
{
    m_maxParticles = maxparticles;
    m_particles = std::vector<Particle>(m_maxParticles);
    resetParticles();
    //load texmap
    QString texname = QString(&texpath[0]);
    m_tex = QImage(texname);
}
ParticleManager::~ParticleManager(){}
void ParticleManager::resetParticle(int i){
    m_particles.at(i).pos = m_position;//probably am going to need to have some other default position in world space?
    m_particles.at(i).life = 1.0f;
    m_particles.at(i).decay = randFloat(0.0025f,0.15f);
    m_particles.at(i).color = m_color;
    m_particles.at(i).force = glm::vec3(randFloat(-m_fuzziness*0.01f,(m_fuzziness*0.01f))+m_force.x,randFloat(-m_fuzziness*0.01f,(m_fuzziness*0.01f))+m_force.y,randFloat(-m_fuzziness*0.01f,(m_fuzziness*0.01f))+m_force.z);
    m_particles.at(i).dir = glm::vec3(randFloat(-m_fuzziness,m_fuzziness)+m_velocity.x,randFloat(-m_fuzziness,m_fuzziness)+m_velocity.y,randFloat(-m_fuzziness,m_fuzziness)+m_velocity.z);
}
void ParticleManager::updateParticles(){
    for(int i=0;i<m_particles.size();i++){
        m_particles.at(i).pos+= m_particles.at(i).dir*m_speed;
        m_particles.at(i).dir +=m_particles.at(i).force;
        m_particles.at(i).life-=m_particles.at(i).decay;
        if(m_particles.at(i).life<0){
            resetParticle(i);
        }
    }
}
void ParticleManager::setInitialPosition(glm::vec3 initialposition){
    m_position = initialposition;
}
float ParticleManager::randFloat(float lo, float hi){
    return lo + static_cast<float>(rand()) /( static_cast<float>(RAND_MAX/(hi-lo)));
}

void ParticleManager::resetParticles(){
    for(int i=0;i<m_particles.size();i++){
        resetParticle(i);
    }
}
