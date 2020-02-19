#ifndef HELPER_H
#define HELPER_H
#include <vector>
#include <iostream>

#define SNOWMAN 1
#define GLASS 2
#define WALL 3
#define LIGHT 4
#define CYLINDER 5
#define BALL 6

template<typename T>
class Vec3{
    public:
    T x, y, z;
    Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
    Vec3(T xx) : x(xx), y(xx), z(xx) {}
    Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
    Vec3& normalize()
    {
        T nor2 = length2();
        if (nor2 > 0) {
            T invNor = 1 / sqrt(nor2);
            x *= invNor, y *= invNor, z *= invNor;
        }
        return *this;
    }
    Vec3<T> operator * (const T &f) const { return Vec3<T>(x * f, y * f, z * f); }
    Vec3<T> operator * (const Vec3<T> &v) const { return Vec3<T>(x * v.x, y * v.y, z * v.z); }
    T dot(const Vec3<T> &v) const { return x * v.x + y * v.y + z * v.z; }
    Vec3<T> operator - (const Vec3<T> &v) const { return Vec3<T>(x - v.x, y - v.y, z - v.z); }
    Vec3<T> operator + (const Vec3<T> &v) const { return Vec3<T>(x + v.x, y + v.y, z + v.z); }
    Vec3<T>& operator += (const Vec3<T> &v) { x += v.x, y += v.y, z += v.z; return *this; }
    Vec3<T>& operator *= (const Vec3<T> &v) { x *= v.x, y *= v.y, z *= v.z; return *this; }
    Vec3<T> operator - () const { return Vec3<T>(-x, -y, -z); }
    T length2() const { return x * x + y * y + z * z; }
    T length() const { return sqrt(length2()); }
    // friend std::ostream & operator << (std::ostream &os, const Vec3<T> &v)
    // {
    //     os << "[" << v.x << " " << v.y << " " << v.z << "]";
    //     return os;
    // }
};

typedef Vec3<float> ColorRGB;

class Ray{
    public:
    Vec3<float> origin;
    Vec3<float> direction;

    Ray();

    Ray(Vec3<float> o,Vec3<float> d);

};

class Object{

    public:
    int type;
    int id;
    float reflection;
    float refraction;
    Vec3<float> color;//RGB format
    float diffuse;
    float specular;
    Vec3<float> emissionColor;
    bool isGlass=false;
    bool isSnowman= false;
    int belong=0;

    void set_properties(float refl,float refr,Vec3<float> colr,float diff,float spec);

    virtual bool intersect(Ray r,float& t){return false;};

    virtual Ray getNormal(Vec3<float> intersection){return Ray();};

    virtual void update_position(float angle){ return ;};
};

class Camera{
    public:
    Vec3<float> position;

    Camera(){}

    Camera(Vec3<float> p){
        position = p;
    }
};

class Scene;

class LightPoint{

    
    
    public:
    Vec3<float> origin;
    ColorRGB color;
    Vec3<float> initial_origin;

    LightPoint(Vec3<float> o);

    float DistanceDrop(Vec3<float> intersection);

    Ray getL(Vec3<float> intersection);

    float ShadowDrop(Scene* s,Vec3<float> intersection);

    void update_position(float angle);

};



class Sphere;
class Cylinder;

class Scene{

    public:
    std::vector<Object> scene_objects;
    std::vector<Object *> scene_objects_ptr;
    std::vector<Object *> snow_man;
    std::vector<Sphere> scene_spheres;
    std::vector<Cylinder> scene_cylinders;
    std::vector<Object *> tubelights;
    std::vector<LightPoint> light_objects;
    std::vector<LightPoint *> lights;
    int toggle=2;
    bool anti_aliasing=false;


   Camera camera;

    Scene();

    void make_scene();

    void place_objects();

    void place_lights();

    void place_all_objects();

    void place_snowman();

    void update_camera(int type,int forward);

    void update_spheres_position(int type);

    void update_light_positions(int type);

    void trace_prep();

    void toggle_aliasing(){
        anti_aliasing = !anti_aliasing;
    }

    ColorRGB trace(int x,int y,int widht,int height);

    ColorRGB trace_ray(Ray ray,int depth,Object* exclude);

    ColorRGB shade(Object* obj,Ray normal,Vec3<float> intersection,Ray incident);

};

#endif