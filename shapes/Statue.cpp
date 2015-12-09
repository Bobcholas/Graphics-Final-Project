#include "Statue.h"

Statue::Statue()
{
    genStatue(glm::mat4());
}

Statue::Statue(glm::mat4 anchorMatrix)
{
    genStatue(anchorMatrix);
}

Statue::~Statue(){
}

void Statue::genStatue(glm::mat4 anchorMatrix){
    m_anchor = anchorMatrix;
    addBody();
    while (!attachPoints.empty()){
        addLimb(attachPoints.front());
        attachPoints.pop();
    }
}

std::vector<TransPrimitive>* Statue::getObjects(){
    return &objects;
}

void Statue::addBody(){
    std::uniform_int_distribution<int> dist(0, 1);
    std::uniform_real_distribution<float> dist2(0, 1);
    glm::mat4 scale = glm::scale(glm::vec3(dist2(generator) * 2 + 1, dist2(generator) * 2 + 1, dist2(generator) * 2 + 1));

    switch(dist(generator)){
    case 0:
    case 1:
        TransPrimitive tp = TransPrimitive();
        tp.trans = m_anchor * scale;
        tp.primitive.type = PRIMITIVE_SPHERE;
        tp.primitive.material = randMaterial();
        objects.push_back(tp);

        attachPoints.push(TripleVec{scale * glm::vec4(-.5f, dist2(generator)-.5f, dist2(generator)-.5f, 1),
                                    glm::vec4(-1, 0, 0, 0),
                                    glm::vec4(0, -1, 0, 0)});
        attachPoints.push(TripleVec{scale * glm::vec4(.5f, dist2(generator)-.5f, dist2(generator)-.5f, 1),
                                    glm::vec4(1, 0, 0, 0),
                                    glm::vec4(0, -1, 0, 0)});
    }
}

void Statue::addLimb(TripleVec tv){
    std::uniform_int_distribution<int> dist(0, 2);
    std::uniform_real_distribution<float> ezrand(0, 1);
    switch(dist(generator)){
    case 0:
    case 1:
    case 2:
        TransPrimitive tp = TransPrimitive();
        tp.trans = m_anchor * glm::translate(glm::vec3(tv.point));
        tp.primitive.type = PRIMITIVE_SPHERE;
        tp.primitive.material = randMaterial();
        objects.push_back(tp);
    }

    float length = (ezrand(generator) + 1) * 4;

    switch(dist(generator)){
    case 0:
    case 1:
    case 2:
        TransPrimitive tp = TransPrimitive();
        tp.trans = m_anchor * glm::translate(glm::vec3(tv.point))
                * glm::scale(glm::vec3(1, length, 1)) * glm::translate(glm::vec3(0, .5, 0));
        //move base to 0, 0; scale by length; move 0, 0 to joint; move into worldspace

        tp.primitive.type = PRIMITIVE_SPHERE;
        tp.primitive.material = randMaterial();
        objects.push_back(tp);

        if (ezrand(generator) < .3) { //maybe add another branch
            float dirMix = ezrand(generator);
            glm::vec4 newDir = glm::normalize(tv.dir * (1-dirMix) + tv.bendMax * dirMix);
            glm::vec3 cprod = glm::cross(glm::vec3(tv.dir), glm::vec3(tv.bendMax));
            glm::vec4 newDirMax = glm::normalize(glm::rotate(static_cast<float>(ezrand(generator) * M_PI/2 + .1f), cprod)
                                       * newDir);


            attachPoints.push(TripleVec{tv.dir * length + tv.point,
                                        newDir,
                                        newDirMax});
        }
    }
    return;
}

CS123SceneMaterial Statue::randMaterial(){
    CS123SceneMaterial gives;
    gives.blend = 0;
    gives.cAmbient = CS123SceneColor{100, 100, 100, 255};
    gives.cDiffuse = CS123SceneColor{100, 50, 50, 255};
    gives.cDiffuse = CS123SceneColor{50, 100, 50, 255};
    gives.cReflective = CS123SceneColor{50, 50, 100, 255};
    gives.cSpecular = CS123SceneColor{50, 50, 50, 255};
    gives.cTransparent = CS123SceneColor{50, 50, 50, 255};
    gives.ior = 1;
    gives.shininess = .5;
    gives.textureMap = nullptr;
}
