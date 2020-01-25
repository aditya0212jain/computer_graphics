#include <vector>
#include <cmath>
#include "helper.h"

typedef Vec3<float> ColorRGB;

/*
types
spheres = 1
cylinder = 2
walls = 3
*/

float R=3;
float angle_x=0;
float angle_y=0;
float angle_sphere=0;
float angle_lights=0;

Ray::Ray(){
    origin = Vec3<float>(0,0,0);
    direction = Vec3<float>(0,0,0);
}

Ray::Ray(Vec3<float> o,Vec3<float> d){
        origin = o;
        direction = d;
}


Vec3<float> rotate_point_yconstant(Vec3<float> point,float angle){
    float xnew = point.x * cos(M_PI*angle/180.0) - point.z * sin(M_PI*angle/180.0);
    float znew = point.x * sin(M_PI*angle/180.0) + point.z * cos(M_PI*angle/180.0);
    point.x = xnew;
    point.z = znew;
    return point;
}

Vec3<float> rotate_point_xconstant(Vec3<float> point,float angle){
    float ynew = point.y * cos(M_PI*angle/180.0) - point.z * sin(M_PI*angle/180.0);
    float znew = point.y * sin(M_PI*angle/180.0) + point.z * cos(M_PI*angle/180.0);
    point.y = ynew;
    point.z = znew;
    return point;
}


void Object::set_properties(float refl,float refr,Vec3<float> colr,float diff,float spec){
        reflection = refl;
        refraction = refr;
        color = colr;
        diffuse = diff;
        specular = spec;
}

class Sphere: public Object{

    float radius;
    Vec3<float> center;
    Vec3<float> initial_center;

    public:
    
    Sphere(float r,Vec3<float> c){
        // std::cout<<"Sphere radius: "<<radius<<"\n";
        type = 1;
        radius = r;
        center = c; 
        initial_center = c;
        emissionColor = ColorRGB(0,0,0);
    }

    Sphere(float r,Vec3<float> c,int glass){
        type = 1;
        radius = r;
        center = c; 
        initial_center = c;
        emissionColor = ColorRGB(0,0,0);
        if(glass==0){
            isGlass = true;
        }else{
            isGlass = false;
        }
    }

    bool intersect(Ray r,float& t){
        Vec3<float> ce = center - r.origin;// c - e
        float through = ce.dot(r.direction);// (c-e).l
        if(through<0){
            return false;
        }
        float di = ce.dot(ce) - through*through;//d = (c-e)^2 - ((c-e).l)^2
        di = radius*radius - di;// r^2 - d^2
        if(di<0){
            return false;
        }
        float st = sqrt(di);
        float t0 = through - st;
        float t1 = through +st;
        //TODO : Check this part again
        if (t0<0){
            t = t1;
        }else{
            t = t0;
        }
        return true;
    }

    Ray getNormal(Vec3<float> intersection){
        Vec3<float> normal = intersection - center;
        normal = normal.normalize();
        Ray r(intersection,normal);
        return r;
    }

    void update_position(float angle){
        // center = initial_center;
        center = rotate_point_yconstant(center,angle);
    }
};

class Cylinder: public Object{

    float radius;
    float length;
    Vec3<float> bottom_center;
    Vec3<float> initial_center;
    Vec3<float> direction;
    public:
    Cylinder(float r,float c,Vec3<float> b_c){
        type = 2;
        radius = r;
        length = c;
        bottom_center = b_c;
        initial_center = b_c;
        emissionColor = ColorRGB(1,1,1);
        direction = Vec3<float>(0,1,0);
    }

    bool intersect(Ray r,float& t){
        Vec3<float> new_c = r.origin - bottom_center;
        float b = 2*(new_c.z*r.direction.z + new_c.x*r.direction.x);
        float c = pow(new_c.z,2) + pow(new_c.x,2) - pow(radius,2);
        float a = pow(r.direction.x,2) + pow(r.direction.z,2);
        float d = pow(b,2) - (4*a*c);
        if(d<0){
            return false;
        }
        d = sqrt(d);
        float t0 = (-b-d)/(2*a);
        float t1 = (-b+d)/(2*a);
        if (t0<0){
            t = t1;
        }else{
            t = t0;
        }
        Vec3<float> intersection = r.origin + r.direction*t;
        Vec3<float> cd(0,1,0);
        float len = cd.dot(intersection - bottom_center);
        if(len<0||len>length){
            return false;
        }
        return true;
    }


    Ray getNormal(Vec3<float> intersection){
        Vec3<float> c(0,1,0);
        Vec3<float> normal = intersection - (bottom_center + (c*(c.dot(intersection - bottom_center))));
        normal = normal.normalize();
        Ray r(intersection,normal);
        return r;
    }

    void update_position(float angle){
        // bottom_center = initial_center;
        bottom_center = rotate_point_yconstant(bottom_center,angle);
    }

};

LightPoint::LightPoint(Vec3<float> o){
    origin = o;
    initial_origin = o;
    color = ColorRGB(1,1,1);
}

float LightPoint::DistanceDrop(Vec3<float> intersection){
    Vec3<float> t = origin - intersection;
    float dis2 = t.dot(t);
    float dis = sqrt(dis2);
    float ka=1,kb=1,kc=2;
    // this was very important
    float disinc = 20/((dis2*kc)+(dis*kb)+(ka));
    // disinc = 1;
    return disinc;
}

Ray LightPoint::getL(Vec3<float> intersection){
    Vec3<float> t = origin - intersection;
    t = t.normalize();
    Ray r(intersection,t);
    return r;
}

float LightPoint::ShadowDrop(Scene* s,Vec3<float> intersection){
    Vec3<float> po= origin - intersection;
    float t_light = po.dot(po);
    po = po.normalize();
    Ray ray(intersection,po);
    float minD = INFINITY;
    float t;
    for(int i=0;i<s->scene_objects_ptr.size();i++){
        if(s->scene_objects_ptr[i]->intersect(ray,t)&&s->scene_objects_ptr[i]->type<2){
            if(t<minD){
                minD = t;
            }
        }
    }
    if(minD<t_light){
        return 0;
    }else
    {
        return 1;
    }
    
}

void LightPoint::update_position(float angle){
    // origin = initial_origin;
    origin = rotate_point_yconstant(origin,angle);
}

class Wall: public Object{

    public:
    Vec3<float> center;
    Vec3<float> normal;
    
    Wall(Vec3<float> cl,Vec3<float> n=Vec3<float>(0.0,0.0,0.0)){
        type = 3;
        color = cl;
        normal = n;
    }

    bool intersect(Ray r,float& t){
        double directions_dot_prod = normal.dot(r.direction);
		if (directions_dot_prod == 0) {// the plane and ray are parallel
			// t = FLT_MAX; // no intersection, at 'infinity'
			return false;
		}
		t = normal.dot(center - r.origin) / directions_dot_prod;

		if (t < 1e-2) { // the plane is behind the ray
			// t = ;
			return false;
		}
		// Vec3<float> intersection = r.origin + r.direction*t;

        // if(intersection.x>2*R||intersection.y>2*R||intersection.z>2*R){
        //     return false;
        // }

    
        // if(intersection.x>2*R||intersection.y>2*R||intersection.z>2*R){
        //     return false;
        // }
        return true;


    }

    Ray getNormal(Vec3<float> intersection){
        return Ray(intersection,normal);
    }

};

Scene::Scene(){}

void Scene::make_scene(){
    // camera.position = Vec3<float>(60*R,60*R,60*R);
    place_lights();
    place_all_objects();
}

void Scene::update_camera(int type,int forward){
    
    if(type==0){
        // horizontally angle_y
        if(forward==1){
            angle_y +=15;
        }else{
            angle_y -=15;
        }
        return;
    }else{
        if(forward==1){
            angle_x -=15;
        }else{
            angle_x +=15;
        }
    }
    return;
    // float xnew = camera.position.x * cos(M_PI*angle/180) - camera.position.z * sin(M_PI*angle/100);
    // float znew = camera.position.x * sin(M_PI*angle/180) + camera.position.z * cos(M_PI*angle/180);
    // camera.position.x = xnew;
    // camera.position.z = znew;
}

void Scene::update_spheres_position(int type){
    if(type==0){
        angle_sphere=-15;
    }else{
        angle_sphere=15;
    }
    for(int i=0;i<scene_objects_ptr.size();i++){
        if(scene_objects_ptr[i]->type==1){
            scene_objects_ptr[i]->update_position(angle_sphere);
        }
    }
}

void Scene::update_light_positions(int type){
    if(type==0){
        angle_lights = -15;
    }else
    {
        angle_lights = 15;
    } 
    std::cout<<"angle_lights: "<<angle_lights<<"\n";
    for(int i=0;i<scene_objects_ptr.size();i++){
        if(scene_objects_ptr[i]->type==2){
            scene_objects_ptr[i]->update_position(angle_lights);
        }
    }
    for(int i=0;i<light_objects.size();i++){
        light_objects[i].update_position(angle_lights);
    }
}

void Scene::place_objects(){
    scene_objects.push_back(Sphere(R,Vec3<float>(0,0,0)));
    Sphere s(R/4,Vec3<float>(-R,0,0));
    s.color = ColorRGB(1,0,0);//orange
    s.reflection = 0.60;
    s.refraction = 0.3;
    s.diffuse = 0.3;
    s.specular = 0.70;
    s.id = 1;
    scene_objects.push_back(s);
    scene_spheres.push_back(s);
    scene_objects_ptr.push_back(&scene_spheres[scene_spheres.size()-1]);
    Sphere s1(R/4,Vec3<float>(R,0,0));
    s1.color = ColorRGB(1,0,0);//orange
    s1.reflection = 0.60;
    s1.refraction = 0.3;
    s1.diffuse = 0.3;
    s1.specular = 0.70;
    s1.id = 2;
    scene_objects.push_back(s1);
    scene_spheres.push_back(s1);
    scene_objects_ptr.push_back(&scene_spheres[scene_spheres.size()-1]);
    place_snowman();
}

void Scene::place_all_objects(){
    // placing the 12 balls 
    ColorRGB cyan(0,1,1);
    ColorRGB orange(1,0.647,0);
    ColorRGB magenta(1,0,1);
    for(int i=0;i<12;i++){
        float angle = i*30;
        float x = 1.5*R*cos(M_PI*angle/180);
        float z = 1.5*R*sin(M_PI*angle/180);
        // Sphere s(R/4,Vec3<float>(x,-2*R+R/4,z));
        float refle = (i%2==0) ? 0.30:0.9;
        float refra = (i%2==0) ? 0.60:0.0;
        ColorRGB coltemp;
        switch (i%3)
        {
        case 0:
            coltemp = cyan;
            break;
        case 1:
            coltemp = orange;
            break;
        case 2:
            coltemp = magenta;
        default:
            break;
        }
        if(i%2==0){
            coltemp=Vec3<float>(1,1,1);
        }
        //reflection, refraction, color, diffuse, spec
        scene_objects_ptr.push_back(new Sphere(R/4,Vec3<float>(x,-2*R+R/4,z),i%2));
        scene_objects_ptr[scene_objects_ptr.size()-1]->set_properties(refle,refra,coltemp,0.25,0.75);
        if(i%2==0){
            scene_objects_ptr[scene_objects_ptr.size()-1]->belong=GLASS;
        }else{
            scene_objects_ptr[scene_objects_ptr.size()-1]->belong =BALL;
        }
    }

    //placing snowman
    place_snowman();

    //Placing lights
    float x_r[]= {2*R,-2*R};
    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
            scene_objects_ptr.push_back(new Cylinder(R/25,R,Vec3<float>(x_r[i],-2*R,x_r[j])));
            scene_objects_ptr[scene_objects_ptr.size()-1]->belong= CYLINDER;
        }
    }

    //placing walls

    scene_objects_ptr.push_back(new Wall(Vec3<float>(2*R,0,0),Vec3<float>(-1,0,0)));
    scene_objects_ptr[scene_objects_ptr.size()-1]->set_properties(1,0.0,ColorRGB(1,0,0),1.0,0.0);
    scene_objects_ptr[scene_objects_ptr.size()-1]->belong = WALL;

    scene_objects_ptr.push_back(new Wall(Vec3<float>(-2*R,0,0),Vec3<float>(1,0,0)));
    scene_objects_ptr[scene_objects_ptr.size()-1]->set_properties(1,0.0,ColorRGB(1,0,0),1.0,0.0);
    scene_objects_ptr[scene_objects_ptr.size()-1]->belong = WALL;

    scene_objects_ptr.push_back(new Wall(Vec3<float>(0,0,-2*R),Vec3<float>(0,0,1)));
    scene_objects_ptr[scene_objects_ptr.size()-1]->set_properties(1,0.0,ColorRGB(1,0,0),0.0,1.0);
    scene_objects_ptr[scene_objects_ptr.size()-1]->belong = WALL;

    scene_objects_ptr.push_back(new Wall(Vec3<float>(0,0,2*R),Vec3<float>(0,0,-1)));
    scene_objects_ptr[scene_objects_ptr.size()-1]->set_properties(1,0.0,ColorRGB(1,0,0),1.0,0.0);
    scene_objects_ptr[scene_objects_ptr.size()-1]->belong = WALL;

    // up
    scene_objects_ptr.push_back(new Wall(Vec3<float>(0,2*R,0),Vec3<float>(0,-1,0)));
    scene_objects_ptr[scene_objects_ptr.size()-1]->set_properties(1,0.0,ColorRGB(0,1,0),1.0,0.0);
    scene_objects_ptr[scene_objects_ptr.size()-1]->belong = WALL;

    // down
    scene_objects_ptr.push_back(new Wall(Vec3<float>(0,-2*R,0),Vec3<float>(0,1,0)));
    scene_objects_ptr[scene_objects_ptr.size()-1]->set_properties(1,0.0,ColorRGB(1,1,1),1.0,0.0);
    scene_objects_ptr[scene_objects_ptr.size()-1]->belong = WALL;

}

float getDistanceBetweenSpheres(float r1,float r2,float angle2){
    float b = -2 * r2 * cos(M_PI*angle2/180);
    float d = sqrt(pow(b,2)-(4*(pow(r2,2)-pow(r1,2))));
    float a1 = (-b + d)/2.0;
    float a2 = (-b - d)/2.0;
    if(a1<0){
        return a2;
    }
    if(a2<0){
        return a1;
    }
    return (a1<a2) ? a1:a2;
}

void Scene::place_snowman(){
    //reflection, refraction, color, diffuse, spec
    scene_objects_ptr.push_back(new Sphere(R,Vec3<float>(0,-R,0)));
    scene_objects_ptr[scene_objects_ptr.size()-1]->set_properties(0.9,0.0,ColorRGB(1,1,1),0.65,0.35);
    scene_objects_ptr[scene_objects_ptr.size()-1]->isSnowman = true;
    scene_objects_ptr[scene_objects_ptr.size()-1]->belong = SNOWMAN;
    float d1 = getDistanceBetweenSpheres(R,R/2,60);

    // Sphere s1(R/2,Vec3<float>(0,-R+d1,0));
    //reflection, refraction, color, diffuse, spec
    scene_objects_ptr.push_back(new Sphere(R/2,Vec3<float>(0,-R+d1,0)));
    scene_objects_ptr[scene_objects_ptr.size()-1]->set_properties(0.9,0.0,ColorRGB(1,1,1),0.65,0.35);
    scene_objects_ptr[scene_objects_ptr.size()-1]->isSnowman = true;
    scene_objects_ptr[scene_objects_ptr.size()-1]->belong = SNOWMAN;
    float d2 = getDistanceBetweenSpheres(R/2,R/4,60);

    Sphere s2(R/4,Vec3<float>(0,-R+d1+d2,0));
    //reflection, refraction, color, diffuse, spec
    scene_objects_ptr.push_back(new Sphere(R/4,Vec3<float>(0,-R+d1+d2,0)));
    scene_objects_ptr[scene_objects_ptr.size()-1]->set_properties(0.9,0.0,ColorRGB(1,1,1),0.65,0.35);
    scene_objects_ptr[scene_objects_ptr.size()-1]->isSnowman = true;
    scene_objects_ptr[scene_objects_ptr.size()-1]->belong = SNOWMAN;
}

void Scene::place_lights(){
    float x[] = {2*R,-2*R};
    float y[] = {2*R,(float)2*R/(float)3.0,(float)(-2*R)/(float)3.0,-2*R};
    // float y[] = {2*R};
    for(int xt=0;xt<2;xt++){
        for(int yt=0;yt<4;yt++){
            for(int zt=0;zt<2;zt++){
                light_objects.push_back(LightPoint(Vec3<float>(x[xt],-2*R+(yt*(R/3.0)),x[zt])));
                // lights.push_back(&light_objects[light_objects.size()-1]);
            }
        }
    }
    // light_objects.push_back(LightPoint(Vec3<float>(2*R,2*R,2*R)));
}

void Scene::trace_prep(){
    camera.position = Vec3<float> (0,0,14*R);
    Vec3<float> dir = Vec3<float>(0.0,0.0,0.0) - camera.position;
    dir = dir.normalize();
    Vec3<float> ref = camera.position + dir;
}

ColorRGB Scene::trace(int x,int y,int width,int height){
    
    Vec3<float> dir = Vec3<float>(0.0,0.0,0.0) - camera.position;
    dir = dir.normalize();
    Vec3<float> ref = camera.position + dir;
    //First need to generate ray
    // int width = 640;
    // int height = 480;
    float fov = 30;
    float tanV = tan(M_PI*(fov/2)/180.0);
    float wby2 = width/2.0;
    float hby2 = height/2.0;
    float xt = (x - wby2)*tanV/wby2;
    float yt = (y - hby2)*tanV/hby2;
    Vec3<float> screenPoint(xt+ref.x,yt+ref.y,ref.z);

    // Distributed Ray Tracing
    // Anti Aliasing
    int n = 5;
    float x1 = x+1;
    float y1 = y+1;
    float xt1 = (x1 - wby2)*tanV/wby2;
    float yt1 = (y1 - hby2)*tanV/hby2;
    Vec3<float> screenPoint2(xt1+ref.x,yt1+ref.y,ref.z);
    Vec3<float> diff = screenPoint2 - screenPoint;


    if(anti_aliasing==true){
        ColorRGB traceColor(0,0,0);

        for(int i=0;i<n;i++){
            float rx = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            float ry = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            Vec3<float> pointTemp = screenPoint +Vec3<float>(rx*diff.x,ry*diff.y,0);
            if(i==0){
                pointTemp = screenPoint;
            }
            Vec3<float> ray_dir = pointTemp - camera.position;
            ray_dir = ray_dir.normalize();
            Ray ray(camera.position, ray_dir);
            Vec3<float> point2 = ray.origin+ray.direction;
            ray.origin = rotate_point_yconstant(ray.origin,angle_y);
            ray.origin = rotate_point_xconstant(ray.origin,angle_x);
            point2 = rotate_point_yconstant(point2,angle_y);
            point2 = rotate_point_xconstant(point2,angle_x);
            ray.direction = point2 - ray.origin;
            ray.direction = ray.direction.normalize();
            // std::cout<<"x,y,z: "<<ray.direction.x<<" "<<ray.direction.y<<" "<<ray.direction.z<<"\n";
            traceColor += trace_ray(ray,4,nullptr);
        }

        traceColor = traceColor*(1.0/((float)n));
        return traceColor;
    }



    Vec3<float> ray_dir = screenPoint - camera.position;
    ray_dir = ray_dir.normalize();
    Ray ray(camera.position, ray_dir);
    Vec3<float> point2 = ray.origin+ray.direction;
    ray.origin = rotate_point_yconstant(ray.origin,angle_y);
    ray.origin = rotate_point_xconstant(ray.origin,angle_x);
    point2 = rotate_point_yconstant(point2,angle_y);
    point2 = rotate_point_xconstant(point2,angle_x);
    ray.direction = point2 - ray.origin;
    ray.direction = ray.direction.normalize();
    // std::cout<<"x,y,z: "<<ray.direction.x<<" "<<ray.direction.y<<" "<<ray.direction.z<<"\n";
    return trace_ray(ray,4,nullptr);
}

void fresnelcompute(const Vec3<float> I, const Vec3<float> N, const float ior, float &kr) 
{ 
    float cosi = I.dot(N);
    if(cosi<-1){
        cosi=-1;
    }else if(cosi>1){
        cosi = 1;
    }
    float etai = 1, etat = ior; 
    if (cosi > 0) { std::swap(etai, etat); } 
    // Compute sini using Snell's law
    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi)); 
    // Total internal reflection
    if (sint >= 1) { 
        kr = 1; 
    } 
    else { 
        float cost = sqrtf(std::max(0.f, 1 - sint * sint)); 
        cosi = fabsf(cosi); 
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost)); 
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost)); 
        kr = (Rs * Rs + Rp * Rp) / 2; 
    } 
} 

ColorRGB Scene::trace_ray(Ray ray,int depth,Object* exclude){

    if (depth<=0){
        return ColorRGB(1,1,1);
    }
    //////
    /*
    shade
    reflection
    refraction
    TIR
    */
    Object* nearest_object=nullptr;
    float minD = INFINITY;
    float t;
    
    // std::cout<<scene_objects.size()<<std::endl;
    for(int i=0;i<scene_objects_ptr.size();i++){
        if(scene_objects_ptr[i]->intersect(ray,t)&&!(depth==4&&scene_objects_ptr[i]->type==3)){
            if(t<minD){
                minD = t;
                nearest_object = scene_objects_ptr[i];
            }
        }
    }

    //if no intersection
    if(nearest_object==nullptr){
        return ColorRGB(0,0,0);
    }
    
    Vec3<float> intersectionPoint = ray.origin + ray.direction*minD;
    Ray normal = nearest_object->getNormal(intersectionPoint);

    bool inside = false;
    if (ray.direction.dot(normal.direction)>0){
        inside = true;
        normal.direction = -normal.direction;
    }

    if(nearest_object->belong==SNOWMAN){
        ColorRGB I_shade = shade(nearest_object,normal,intersectionPoint,ray);
        return I_shade;
    }else if(nearest_object->belong==GLASS){
        Vec3<float> reflection_ray = ray.direction - (normal.direction * 2* ray.direction.dot(normal.direction));
        reflection_ray = reflection_ray.normalize();
        float bias = 1e-2;

        // float facingratio = -ray.direction.dot(normal.direction);
        // float fresnel = 1*(0.1) + (1-0.1)*pow(1-facingratio,3);
        float fresnel;
        fresnelcompute(ray.direction,normal.direction,1.5,fresnel);
        ColorRGB I_reflection = trace_ray(Ray(intersectionPoint+normal.direction*bias,reflection_ray),depth -1,nearest_object);

        float ior = 1.1, eta = (inside) ? ior : 1 / ior; // are we inside or outside the surface?
        float cosi = -normal.direction.dot(ray.direction);
        float k = 1 - eta * eta * (1 - cosi * cosi);
        Vec3<float> refraction_dir = ray.direction * eta + normal.direction * (eta *  cosi - sqrt(k));
        refraction_dir = refraction_dir.normalize();
        ColorRGB I_refraction;
        if(fresnel>1){
            I_refraction=0;
        }else{
            I_refraction = trace_ray(Ray(intersectionPoint-normal.direction*bias,refraction_dir),depth -1,nearest_object);
        }
        
        ColorRGB fresnelcolor = (I_reflection*(fresnel/nearest_object->reflection) + I_refraction*((1-fresnel)/nearest_object->refraction));
        return fresnelcolor;

    }else if(nearest_object->belong==CYLINDER){
        return nearest_object->emissionColor;
    }else if(nearest_object->belong==WALL){
        Vec3<float> reflection_ray = ray.direction - (normal.direction * 2* ray.direction.dot(normal.direction));
        reflection_ray = reflection_ray.normalize();
        float bias = 1e-2;

        float facingratio = -ray.direction.dot(normal.direction);
        float fresnel = 1*(0.1) + (1-0.1)*pow(1-facingratio,3);
        ColorRGB I_reflection = trace_ray(Ray(intersectionPoint+normal.direction*bias,reflection_ray),depth -1,nearest_object);

        ColorRGB fresnelcolor = (I_reflection*fresnel*nearest_object->reflection);
        // ColorRGB I_shade = shade(nearest_object,normal,intersectionPoint,ray);

        return fresnelcolor;
    }else if(nearest_object->belong==BALL){
        ColorRGB I_shade = shade(nearest_object,normal,intersectionPoint,ray);
        Vec3<float> reflection_ray = ray.direction - (normal.direction * 2* ray.direction.dot(normal.direction));
        reflection_ray = reflection_ray.normalize();
        float bias = 1e-2;

        float facingratio = -ray.direction.dot(normal.direction);
        float fresnel = 1*(0.1) + (1-0.1)*pow(1-facingratio,3);
        ColorRGB I_reflection = trace_ray(Ray(intersectionPoint+normal.direction*bias,reflection_ray),depth -1,nearest_object);

        ColorRGB ballColor = I_reflection*(nearest_object->reflection)*fresnel*nearest_object->color;

        return ballColor+I_shade;
    }
    
}

ColorRGB Scene::shade(Object* obj,Ray normal,Vec3<float> intersection,Ray incident){
    ColorRGB iShade(0,0,0);
    iShade = ColorRGB(0.3,0.3,0.3)*obj->diffuse;
    for(int i=0;i<light_objects.size();i++){
        float attenuation = light_objects[i].DistanceDrop(intersection);
        //// shadow attenuation required
        float shadow_att = light_objects[i].ShadowDrop(this,intersection);
        ColorRGB light_color;
        Ray l = light_objects[i].getL(intersection);
        Vec3<float> h = incident.direction + l.direction;
        h = h.normalize();
        float temp = normal.direction.dot(h);
        temp = pow(temp,5);
        // temp *= temp;
        // t
        float combined = obj->diffuse*(normal.direction.dot(l.direction)) +
                            obj->specular*(temp);

        light_color = light_objects[i].color*combined;
        iShade += light_color*attenuation*shadow_att*std::max(0.0f,normal.direction.dot(l.direction));

    }
    // return iShade;
    return iShade*obj->color;
}