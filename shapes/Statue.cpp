#include "Statue.h"
#include <iostream>

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
    m_mat = randMaterial();
    addBody();
}

std::vector<TransPrimitive>* Statue::getObjects(){
    return &objects;
}

int Statue::intRange(int lo, int hi){
    return rand() % (hi - lo + 1) + lo;
}

float Statue::floatRange(float lo, float hi){
    return  lo + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(hi-lo)));
}

glm::vec4 Statue::vecAround(glm::vec4 dir){
    glm::vec3 rotator = glm::normalize(glm::vec3(floatRange(-1, 1), floatRange(-1, 1), floatRange(-1, 1)));
    return glm::rotate(static_cast<float>(std::pow(floatRange(0, 1), 1.4)), rotator) * dir;
}

glm::mat4 Statue::rotateTo(glm::vec4 from, glm::vec4 to){
    glm::vec3 cprod = glm::cross(glm::normalize(glm::vec3(from)), glm::normalize(glm::vec3(to)));
    float mag = std::asin(glm::length(cprod));
    if (mag == 0){
        to.x += .1; //if not wholly x, this will find a cross product
        cprod = glm::cross(glm::normalize(glm::vec3(from)), glm::normalize(glm::vec3(to)));
        mag = std::asin(glm::length(cprod));
        if (mag == 0){ //this will find if wholly x
            to.y += .1;
            mag = std::asin(glm::length(cprod));
        }
        return glm::rotate(static_cast<float>(M_PI), cprod);
    }else{
        return glm::rotate(mag, cprod);
    }
}

Statue::TripleVec Statue::limbStarter(TripleVec base){
    TripleVec tv;
    tv.point = base.point;
    glm::vec4 pointDir = vecAround(base.dir);
    glm::vec4 maxDir = rotateTo(base.dir, pointDir) * base.bendMax;
    glm::mat4 rotator = glm::rotate(static_cast<float>(M_PI * 2 * floatRange(0, 1)), glm::vec3(base.dir));
    tv.dir = rotator*pointDir;
    tv.bendMax = rotator * maxDir;
    return tv;
}

void Statue::addBody(){
    glm::mat4 scale = glm::scale(glm::vec3(floatRange(2, 3), floatRange(2, 4), floatRange(1, 2)));

    switch(intRange(0, 1)){
    case 0:
    case 1:
        TransPrimitive tp = TransPrimitive();
        tp.trans = m_anchor * scale;
        tp.primitive.type = PRIMITIVE_CUBE;
        tp.primitive.material = m_mat;
        objects.push_back(tp);

        int armJoints = intRange(2, 4);
        int legJoints = 1;
        addLimb(limbStarter(TripleVec{scale * glm::vec4(-.5f, floatRange(-.5f, .5f), floatRange(-.5f, .5f), 1),
                                      glm::vec4(-1, 0, 0, 0),
                                      glm::vec4(0, 1, 0, 0)}), armJoints);
        addLimb(limbStarter(TripleVec{scale * glm::vec4(.5f, floatRange(-.5f, .5f), floatRange(-.5f, .5f), 1),
                                      glm::vec4(1, 0, 0, 0),
                                      glm::vec4(0, 1, 0, 0)}), armJoints);

        addLimb(TripleVec{scale * glm::vec4(floatRange(0, .5f), -.5f, floatRange(-.5f, .5f), 1),
                          glm::vec4(0, -1, 0, 0),
                          glm::vec4(-1, 0, 0, 0)}, legJoints);
        addLimb(TripleVec{scale * glm::vec4(floatRange(-.5f, 0), -.5f, floatRange(-.5f, .5f), 1),
                          glm::vec4(0, -1, 0, 0),
                          glm::vec4(-1, 0, 0, 0)}, legJoints);

        TripleVec starter = limbStarter(TripleVec{scale * glm::vec4(floatRange(-.5f, .5f), .5f, floatRange(-.5f, .5f), 1),
                                                  glm::vec4(0, 1, 0, 0),
                                                  glm::vec4(0, 0, 1, 0)});
        addHead(starter.point, starter.dir);
    }
}

void Statue::addHead(glm::vec4 point, glm::vec4 up){
    TransPrimitive tp;
    float size = floatRange(1, 3);
    tp.trans = m_anchor *
            rotateTo(glm::vec4(0, 1, 0, 0), up) *
            glm::translate(glm::vec3(point)) *
            glm::scale(glm::vec3(size*.75, size, size*.8)) *
            glm::translate(glm::vec3(0, .35, 0));
    switch(intRange(0, 2)){
    case 0:
        tp.primitive.type = PRIMITIVE_SPHERE;
        break;
    case 1:
        tp.primitive.type = PRIMITIVE_CUBE;
        break;
    case 2:
        tp.primitive.type = PRIMITIVE_CYLINDER;
        break;
    }
    tp.primitive.material = m_mat;
    objects.push_back(tp);
}

void Statue::addLimb(TripleVec tv, int joints){
    std::cout << joints << std::endl;
    std::cout << glm::to_string(tv.point) << std::endl;
    if (joints == 0){
        return;
    }
    {
        TransPrimitive tp = TransPrimitive();
        tp.trans = m_anchor * glm::translate(glm::vec3(tv.point));
        tp.primitive.material = m_mat;
        switch(0){//intRange(0, 2)){
        case 0:
            tp.primitive.type = PRIMITIVE_SPHERE;
            break;
        case 1:
            tp.primitive.type = PRIMITIVE_CUBE;
            break;
        case 2:
            tp.primitive.type = PRIMITIVE_CYLINDER;
            break;
        }
        objects.push_back(tp);
    }

    float length = floatRange(1.9, 2);

    float dirMix = floatRange(0, 1);

    TransPrimitive tp = TransPrimitive();
    tp.trans = m_anchor;
    tp.trans *= glm::translate(glm::vec3(tv.point));
    glm::mat4 rotator = rotateTo(glm::vec4(0, 1, 0, 0), tv.dir);
    tp.trans *= rotator;
    tp.trans *= glm::scale(glm::vec3(1, length, 1));
    tp.trans *= glm::translate(glm::vec3(0, .5, 0));
    //move base to 0, 0; scale by length; move 0, 0 to joint; move into worldspace

    switch(0){//intRange(0, 2)){
    case 0:
        tp.primitive.type = PRIMITIVE_CYLINDER;
        break;
    case 1:
        tp.primitive.type = PRIMITIVE_CUBE;
        break;
    case 2:
        tp.primitive.type = PRIMITIVE_SPHERE;
        break;
    }

    tp.primitive.material = m_mat;
    objects.push_back(tp);

    if (joints > 0) { //maybe add another branch
        glm::vec4 newDir = glm::normalize(tv.dir * (1-dirMix) + tv.bendMax * dirMix);
        glm::mat4 dirRotate = rotateTo(tv.dir, newDir);
        glm::vec4 newDirMax = glm::normalize(dirRotate * tv.bendMax);


        addLimb(TripleVec{tp.trans * glm::vec4(0, .5, 0, 1),
                          newDir,
                          newDirMax}, joints - 1);
        return;
    }
}

CS123SceneMaterial Statue::randMaterial(){
    CS123SceneMaterial gives;
    gives.blend = 0;
    gives.cAmbient = CS123SceneColor{floatRange(.25, 1), floatRange(.25, 1), floatRange(.25, 1), 1};
    gives.cDiffuse = CS123SceneColor{floatRange(.25, 1), floatRange(.25, 1), floatRange(.25, 1), 1};
    gives.cDiffuse = CS123SceneColor{floatRange(.25, 1), floatRange(.25, 1), floatRange(.25, 1), 1};
    gives.cReflective = CS123SceneColor{floatRange(.25, 1), floatRange(.25, 1), floatRange(.25, 1), 1};
    gives.cSpecular = CS123SceneColor{floatRange(.25, 1), floatRange(.25, 1), floatRange(.25, 1), 1};
    gives.cTransparent = CS123SceneColor{floatRange(.25, 1), floatRange(.25, 1), floatRange(.25, 1), 1};
    gives.ior = 1;
    gives.shininess = .5;
    gives.textureMap = nullptr;
    return gives;
}
