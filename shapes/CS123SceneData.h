/**
 * @file CS123SceneData.h
 *
 * Header file containing scene data structures.
 */

#ifndef __CS123_SCENE_DATA__
#define __CS123_SCENE_DATA__

#include <vector>
#include <string>
#include "GL/glew.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

// Enumeration for light types.
enum LightType {
    LIGHT_POINT, LIGHT_DIRECTIONAL, LIGHT_SPOT, LIGHT_AREA
};

enum ShapeType
{
    SHAPE_CUBE,
    SHAPE_CONE,
    SHAPE_SPHERE,
    SHAPE_CYLINDER,
    SHAPE_TORUS,
    SHAPE_SPECIAL_1,
    SHAPE_SPECIAL_2,
    SHAPE_SPECIAL_3,
    NUM_SHAPE_TYPES
};

// Enumeration for types of primitives that can be stored in a scene file.
enum PrimitiveType {
    PRIMITIVE_CUBE,
    PRIMITIVE_CONE,
    PRIMITIVE_CYLINDER,
    PRIMITIVE_TORUS,
    PRIMITIVE_SPHERE,
    PRIMITIVE_MESH
};

// Enumeration for types of transformations that can be applied to objects, lights, and cameras.
enum TransformationType {
    TRANSFORMATION_TRANSLATE, TRANSFORMATION_SCALE, TRANSFORMATION_ROTATE, TRANSFORMATION_MATRIX
};

struct BGRA
{
    BGRA() : b(0), g(0), r(0), a(0) {}
    BGRA(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255)
        : b(blue), g(green), r(red), a(alpha) {}

    // C++ TIP:
    // A union struct. Essentially, this makes b <==> channels[0],
    // g <==> channels[1], etc. In other words: b, g, r, and a are
    // stored at the same memory location as channels[4]. Note that
    // sizeof(b)+sizeof(g)+sizeof(r)+sizeof(a) = sizeof(channels)
    // so the memory overlaps **exactly**.
    //
    // You might want to take advantage of union structs in later
    // assignments, although we don't require or expect you to do so.
    //
    union {
        struct { unsigned char b, g, r, a; };
        unsigned char channels[4];
    };

    // @TODO: [OPTIONAL] You can implement some operators here for color arithmetic.

};

const float FLOAT_EPSILON = 1e-8;



// Struct to store a RGBA color in floats [0,1]
struct CS123SceneColor 
{
    union {
        struct {
            float r;
            float g;
            float b;
            float a;
        };
        float channels[4]; // points to the same four floats above...
    };

    // @TODO: [OPTIONAL] You can implement some operators here for color arithmetic.
    CS123SceneColor& operator*=(float y){
        r *= y;
        g *= y;
        b *= y;
        return *this;
    }
    const CS123SceneColor operator*(float f) const{
        return CS123SceneColor{this->r*f, this->g*f, this->b*f, this->a};
    }
    CS123SceneColor& operator*=(int y){
        r *= y;
        g *= y;
        b *= y;
        return *this;
    }
    const CS123SceneColor operator*(int f) const{
        return CS123SceneColor{this->r*f, this->g*f, this->b*f, this->a};
    }
    CS123SceneColor& operator*=(const CS123SceneColor &c)
    {
        r *= c.r;
        g *= c.g;
        b *= c.b;
        return *this;
    }
    const CS123SceneColor operator*(const CS123SceneColor &c)
    {
        return CS123SceneColor{this->r*c.r, this->g*c.g, this->b*c.b, this->a};
    }
    CS123SceneColor& operator+=(const CS123SceneColor &c)
    {
        r += c.r;
        g += c.g;
        b += c.b;
        return *this;
    }
    const CS123SceneColor operator+(const CS123SceneColor &c)
    {
        return CS123SceneColor{this->r+c.r, this->g+c.g, this->b+c.b, this->a};
    }
    const BGRA toBGRA(){
        return BGRA{static_cast<unsigned char>(std::min(255.f, r*255)),
                    static_cast<unsigned char>(std::min(255.f, g*255)),
                    static_cast<unsigned char>(std::min(255.f, b*255)),
                    static_cast<unsigned char>(std::min(255.f, a*255))};
    }

    const CS123SceneColor nonNeg(){
        return CS123SceneColor{std::max(0.f, r), std::max(0.f, g), std::max(0.f, b), std::max(0.f, a)};
    }

};

// Scene global color coefficients
struct CS123SceneGlobalData 
{
    float ka;  // global ambient coefficient
    float kd;  // global diffuse coefficient
    float ks;  // global specular coefficient
    float kt;  // global transparency coefficient
};

// Data for a single light
struct CS123SceneLightData 
{
    int id;
    LightType type;

    CS123SceneColor color;
    glm::vec3 function;  // Attenuation function

    glm::vec4 pos;       // Not applicable to directional lights
    glm::vec4 dir;       // Not applicable to point lights

    float radius;        // Only applicable to spot lights
    float penumbra;      // Only applicable to spot lights
    float angle;         // Only applicable to spot lights

    float width, height; // Only applicable to area lights
};

// Data for scene camera
struct CS123SceneCameraData
{
    glm::vec4 pos;
    glm::vec4 look;
    glm::vec4 up;

    float heightAngle;
    float aspectRatio;

    float aperture;      // Only applicable for depth of field
    float focalLength;   // Only applicable for depth of field
};

// Data for file maps (ie: texture maps)
struct CS123SceneFileMap
{
    CS123SceneFileMap() : texid(0) {}
    bool isUsed;
    std::string filename;
    float repeatU;
    float repeatV;
    GLuint texid;
};

// Data for scene materials
struct CS123SceneMaterial 
{
    // This field specifies the diffuse color of the object. This is the color you need to use for
    // the object in sceneview. You can get away with ignoring the other color values until
    // intersect and ray.
    CS123SceneColor cDiffuse;

    CS123SceneColor cAmbient;
    CS123SceneColor cReflective;
    CS123SceneColor cSpecular;
    CS123SceneColor cTransparent;
    CS123SceneColor cEmissive;

    CS123SceneFileMap* textureMap;
    float blend;

    CS123SceneFileMap* bumpMap;

    float shininess;

    float ior; // index of refraction
};

// Data for a single primitive.
struct CS123ScenePrimitive
{
    PrimitiveType type;
    std::string meshfile;     // Only applicable to meshes
    CS123SceneMaterial material;
};

// Data for transforming a scene object. Aside from the TransformationType, the remaining of the
// data in the struct is mutually exclusive.
struct CS123SceneTransformation
{
    // Whether the transformation is a rotation, translation, scale, or custom matrix.
    TransformationType type;

    glm::vec3 translate; // The translation vector. Only valid if transformation is a translation.
    glm::vec3 scale;     // The scale vector. Only valid if transformation is a scale.
    glm::vec3 rotate;    // The axis of rotation. Only valid if the transformation is a rotation.
    float angle;         // The rotation angle in RADIANS. Only valid if transformation is a
    // rotation.

    glm::mat4x4 matrix;  // The matrix for the transformation. Only valid if the transformation is
    // a custom matrix.
};

// Structure for non-primitive scene objects
struct CS123SceneNode
{
    // Transformation at this node
    std::vector<CS123SceneTransformation*> transformations;

    // Primitives at this node
    std::vector<CS123ScenePrimitive*> primitives;

    // Children of this node
    std::vector<CS123SceneNode*> children;
};

struct TransPrimitive {
    CS123ScenePrimitive primitive;
    glm::mat4 trans;
};

#endif

