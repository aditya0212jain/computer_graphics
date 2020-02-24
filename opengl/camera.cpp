#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/transform.hpp> 

Camera::Camera(int width, int height, float fov, float near, float far)
{
    // projection = glm::perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 100.0f);
    projection(width, height, fov, near, far); 
    position(0.0, 0.0, 5.0, false);//4,3,2
    lookat(0.0, 0.0, 0.0, false);
    upward(0.0, 1.0, 0.0, true);

    // orientation = glm::quat(up);
}

void Camera::rotate_cameraX(float angleX){
   glm::mat4 rotation;
   rotation = glm::rotate(angleX, glm::vec3(0,1,0));
   glm::vec3 new_pos = glm::mat3(rotation)*pos;
   position(new_pos.x,new_pos.y,new_pos.z,true);
}

void Camera::rotate_cameraY(float angleY){
   glm::mat4 rotation;
   glm::vec3 base = glm::cross(at - pos,up);
   rotation = glm::rotate(angleY,base);
   glm::vec3 new_pos = glm::mat3(rotation)*pos;
   position(new_pos.x,new_pos.y,new_pos.z,true);
}

glm::mat4 Camera::getTranslateXY(float x,float y){
   glm::vec3 a = glm::normalize(at - pos) * y;
   a.y = 0;
   glm::vec3 b = glm::normalize(glm::cross(at - pos,up)) * x;
   glm::mat4 c = glm::translate(glm::mat4(1.0),a) * glm::translate(glm::mat4(1.0),b);
   return c;
}

void Camera::translate_pos(float v){

   glm::vec3 new_pos = pos + v*(at - pos);
   // glm::vec3 new_pos = glm::mat3(glm::translate(glm::mat4(1.0f),glm::normalize((at*v) - (pos*v)))) * pos;
   position(new_pos.x,new_pos.y,new_pos.z,true);
}


// void Camera::rotate_cameraY(float angleY){
//    glm::mat4 rotation;
//    rotation = glm::rotate(angleX, glm::vec3(0,1,0));
//    glm::vec3 new_pos = glm::mat3(rotation)*pos;
//    position(new_pos.x,new_pos.y,new_pos.z,true);
// }

void Camera::moveZ(float value){
   position(pos.x,pos.y,pos.z+value,true);
}

void Camera::calcProjView()
{
   viewmat = glm::lookAt(pos, at, up);
   pvmat = projmat * viewmat;
}

void Camera::view(glm::vec3 pos, glm::vec3 at, glm::vec3 up)
{
   viewmat = glm::lookAt(pos, at, up);
}

void Camera::projection(int width, int height, float fovd, float near, float far, bool update) 
{
   projmat = glm::perspective(glm::radians(fovd), (float) width / (float)height, near, far);
   if(update) calcProjView();
}

void Camera::position(float x, float y, float z, bool update)
{
   pos = glm::vec3(x, y, z);
   if(update) calcProjView();
}

void Camera::lookat(float x, float y, float z, bool update)
{
   at = glm::vec3(x, y, z);
   if(update) calcProjView();
}

void Camera::upward(float x, float y, float z, bool update)
{
   up = glm::vec3(x, y, z);
   if(update) calcProjView();
}

const glm::mat4 Camera::viewmatrix()
{
   return viewmat; 
}

const glm::mat4 Camera::pvmatrix()
{
   return pvmat;
}

const glm::mat4 Camera::matrix() const
{
    // return glm::mat4(1.0f); // Test identity
    // return projmat * viewmat; // Remember, matrix multiplication is the other way around
    return pvmat; // Remember, matrix multiplication is the other way around
}

const glm::mat4 Camera::matrix(const glm::mat4 modelmat) const
{
    // ModelViewProjection
    return pvmat * modelmat; // Remember, matrix multiplication is the other way around
}
