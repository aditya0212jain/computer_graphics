#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ui.h"
#include "shader.h"
#include "scene.h"
#include "render.h"

void Renderer::useShader(Shader *shader)
{
   if(shader != current_shader)
      { shader->use(); current_shader = shader;}
}
bool Renderer::init(int width, int height, Scene *scene, float fov, float near, float far)
{
    this->fov = fov;
    this->near = near;
    this->far = far;

    //Initialize clear color
    glClearColor( 0.1f, 0.1f, 0.1f, 1.f );
    
    resize(width, height);

    rendermat = NULL;
    this->scene = scene;
    return ( glGetError() == GL_NO_ERROR );
}

Renderer::Renderer(int width, int height, Scene *scene, float fov, float near, float far):
               				camera(width, height, fov, near, far)
{
   init(width, height, scene, fov, near, far);
}

int Renderer::update(){
    return true;
}

int Renderer::update_camera_position(float x,float y,float z)
{
   //No per frame update built-in
   camera.position(x,y,z,true);
   return true; 
}

void Renderer::updateScene(int vao_id,glm::mat4 mat){
    // std::cout<<"vao_id: "<<vao_id<<"\n";
    scene->updateScene(vao_id,mat);
}

void Renderer::rotate_cameraX(float angleX){
    camera.rotate_cameraX(angleX);
}

void Renderer::rotate_cameraY(float angleY){
    camera.rotate_cameraY(angleY);
}

void Renderer::translate_camera(float v){
    camera.translate_pos(v);
}

glm::mat4 Renderer::getTranslateXY(float x,float y){
    return camera.getTranslateXY(x,y); 
}

int Renderer::render(float *mat)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(mat != NULL)
        rendermat = mat;
    int a = scene->render(this, camera, rendermat);
    if(getSelectMode()){
        glFlush();
        glFinish();
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        unsigned char data[4];
        // std::cout<<"last_x "<<renderer->last_x<<" last_y: "<<renderer->last_y<<"\n";
        glReadPixels(last_x,height_w - last_y,1,1, GL_RGBA, GL_UNSIGNED_BYTE, data);
        int pickedID = data[0] + data[1] * 256 + data[2] * 256*256;
        setSelectedObject(pickedID);
        // std::cout<<"pickedId : "<<pickedID<<"\n";
        setSelectMode(false,0,0);
    }
    return a;
}

void Renderer::resize(int width, int height)
{
    if(height < 0)
        return;

    float ratio = (height > 0)? (float) width / (float) height : 1.0f;

    width_w = width;
    height_w = height;

    // Setup viewport
    glViewport(0, 0, width, height);

    // Change to the projection matrix and set our viewing volume
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
}

void Renderer::setSelectMode(bool t, int x,int y){
    if(t){
        select_mode = true;
        last_x = x;
        last_y = y;
    }else{
        select_mode = false;
    }
}
