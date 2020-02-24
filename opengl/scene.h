#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/transform.hpp> 
#include <glm/gtx/string_cast.hpp>

#include "scenebase.h"
#include "geom.h"

#include <iostream>

class Scene : public SceneBase {
public:
   Scene(const char *filename=NULL) { // Read scene from filename
      if(filename == NULL) {
          
          SceneBase* balls[12];
          float R = 1.0;
          for(int i=0;i<12;i++){
            float angle = i*30;
            float x = 1.5*R*cos(M_PI*angle/180);
            float z = 1.5*R*sin(M_PI*angle/180);
            glm::mat4 translation = glm::translate(glm::mat4(1.0f),glm::vec3(x, -2*R+(R/4), z));
            glm::mat4 transform2 = glm::scale(translation,glm::vec3(R/4, R/4 ,R/4));
            const char* vertex_shader = "../data/vs.glsl";
            char* fragment_shader = "../data/fs.glsl";
            if(i%2==0){
              fragment_shader ="../data/fs_without_texture.glsl";
            }
            const char* texture = "../data/rough.jpg";
            
            glm::vec3 color;
            switch (i%3)
            {
            case 0:
              color = glm::vec3(0.0,1.0,1.0);
              break;
            case 1:
              color = glm::vec3(1.0,0.647,0.0);
              break;
            case 2:
              color = glm::vec3(1.0,0.0,1.0);
              break;
            default:
              color = glm::vec3(1.0,1.0,1.0);
              break;
            }

            // geom object things needed : points, elements, color, vertex shader, fragment shader, 
            balls[i] = new SceneBase(new Geom("sphere.txt","sphere_indices.txt",color,vertex_shader,fragment_shader,texture),transform2,NULL,NULL);
            if(i!=0){
              balls[i-1]->attachSibling(balls[i]);
            }
          }
          ////////////////////////////Snowman////////////////////////////////////////////////
          const char* vertex_shader_sb = "../data/snow_vs.glsl";
          const char* fragment_shader_sb = "../data/snow_fs.glsl";
          const char* texture_sb = "../data/sn.jpeg";
          Geom* sb = new Geom("sphere.txt","sphere_indices.txt",glm::vec3(1.0,1.0,1.0),vertex_shader_sb,fragment_shader_sb,texture_sb,0);
          glm::mat4 rotation;
          float angle = 45.0*(glm::pi<float>()/180.0);
          rotation = glm::rotate(angle, glm::vec3(0,1,0));
          SceneBase* snow_base = new SceneBase(sb,glm::translate(rotation,glm::vec3(0, -R, 0)),NULL,NULL);
          balls[11]->attachSibling(snow_base);
          Geom* sm = new Geom("sphere.txt","sphere_indices.txt",glm::vec3(1.0,1.0,1.0),vertex_shader_sb,fragment_shader_sb,texture_sb,1);
          SceneBase* snow_middle = new SceneBase(sm,glm::scale(glm::translate(glm::mat4(1.0f),glm::vec3(0, +R, 0)),glm::vec3(0.5,0.5,0.5)),NULL,NULL);
          snow_base->attachChild(snow_middle);
          Geom* st = new Geom("sphere.txt","sphere_indices.txt",glm::vec3(1.0,1.0,1.0),vertex_shader_sb,fragment_shader_sb,texture_sb,2);
          SceneBase* snow_top = new SceneBase(st,glm::scale(glm::translate(glm::mat4(1.0f),glm::vec3(0, +R, 0)),glm::vec3(0.5,0.5,0.5)),NULL,NULL);
          snow_middle->attachChild(snow_top);
          ///////////////////////////////////////////////////////////////////////////////////
          // Floor
          const char* vertex_shader = "../data/floor_vs.glsl";
          const char* fragment_shader = "../data/floor_fs.glsl";
          const char* texture = "../data/snow_floww.jpg";
          SceneBase* floor = new SceneBase(new Geom("floor.txt","floor_indices.txt",glm::vec3(1.0,1.0,1.0),vertex_shader,fragment_shader,texture,3),glm::mat4(1.0f),NULL,NULL);
          // snow_base->attachSibling(floor);
          floor->attachSibling(balls[0]);
          // Scene::init(new Geom(), glm::mat4(1.0f), NULL, balls[0]); // I have a temporary scene generator
          Scene::init(new Geom(), glm::mat4(1.0f), NULL, floor);
      } else {
        printf("Scene reading from file Unimplemented");
      }
   }
};
