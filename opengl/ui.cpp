#include "ui.h"
#include "render.h"
#include "gwindow.h"
#include "uiConstants.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/transform.hpp> 
#include <iostream>

UI::UI(Renderer *renderer, gWindow *window)
{
  _quit = false;
  attach(renderer);
  attach(window);

  lastx = lasty = -1.0;
}

void UI::attach(Renderer *renderer)
{
   this->renderer = renderer;
}

void UI::attach(gWindow *window)
{
   this->gwindow = window;
}

int UI::render()
{
   bool r = renderer->update();
   return r? renderer->render() : r;
}

void UI::handleKeys(int key, float x, float y )
{
   switch (key)
   {
      case CHARDEF(Q):
      case KEYDEF(ESCAPE):
            _quit = true;
            break;
      case KEYDEF(LEFT):
            break;
      case KEYDEF(RIGHT):
            break;
      case KEYDEF(UP):
            break;
      case KEYDEF(DOWN):
            break;
      case KEYDEF(LEFT_SHIFT):
            camera_z_move = !camera_z_move;
            // std::cout<<"shift pressed\n";
   }
}


void UI::handleMouseDown(int button, float x, float y )
{
   lastx = x; lasty = y;
   // std::cout<<"mouse down: "<<button<<"\n";
   renderer->setSelectMode(true,x,y);
   if(button != 0) return;
}

void UI::handleMouseUp(int button, float x, float y )
{
   float mv_x = x - lastx;
   float mv_y = y - lasty;
   // renderer->update_camera_position(mv_x/gwindow->Width(),0);
   
   lastx = x; lasty = y;
   if(button != 0) return;
}


void UI::handleMouseMotion(float x, float y)
{
   float angleX = ((x - lastx)/gwindow->Width())*2.0;// rotate by 30 degress for full width
   float angleY = ((y - lasty)/gwindow->Height())*2.0;
   // renderer->update_camera_position(x/gwindow->Width(),0);
   // std::cout<<angleX<<"\n";
   int vao_id =renderer->getSelectedObject(); 
   if (vao_id>17){
      if(camera_z_move){
         renderer->translate_camera(angleY);
      }else{
         renderer->rotate_cameraX(angleX);
         renderer->rotate_cameraY(angleY);
      }
   }else if(vao_id <=15 && vao_id >12 ){
      glm::mat4 rotation;
      rotation = glm::rotate(angleX, glm::vec3(0,1,0));
      renderer->updateScene(vao_id,rotation);
   }else if(vao_id<=12){
      float disx = (x - lastx)/gwindow->Width();
      float disy = (y - lasty)/gwindow->Height();
      glm::mat4 transform = renderer->getTranslateXY(disx,disy);
      // glm::mat4 translate = glm::translate(glm::mat4(1.0),glm::vec3(disx,0.0,0.0));
      // glm::mat4 translate_o = glm::inverse(renderer->camera.matrix()) * translate * renderer->camera.matrix();
      renderer->updateScene(vao_id,transform);
   }
   lastx = x; lasty = y;
}

void UI::handleResize(int width, int height)
{
    renderer->resize(width, height);
}
