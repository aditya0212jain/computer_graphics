#pragma once

#include "glConstants.h"
#include "camera.h"

class Scene;
class Shader;

class Renderer {
public:
   Renderer(int width, int height, Scene *scene, float fov = FOV_DEFAULT, float near = NEAR_DEFAULT, float far = FAR_DEFAULT);
   bool init(int width, int height, Scene *scene, float fov, float, float far);
   int update();
   int update_camera_position(float x,float y,float z);
   void rotate_cameraX(float angleX);
   void rotate_cameraY(float angleX);
   void translate_camera(float v);
   int render(float *mat = NULL);
   void resize(int width, int height);
   void setMat(float *mat) { rendermat = mat; }
   void useShader(Shader *shader);
   glm::mat4 getTranslateXY(float x,float y);
   glm::vec3 getCameraPos() { return camera.getPosition(); };
   void setSelectMode(bool t,int x,int y);
   bool getSelectMode() { return select_mode; };
   void setSelectedObject(int s) { selected_object = s;};
   int getSelectedObject() { return selected_object;};
   void updateScene(int vao_id,glm::mat4 mat);
   int last_x,last_y;
   int width_w,height_w;
   Camera camera;

private:
   float fov, near, far; // Should I bother remembering? May be useful later for UI purpose.?
       // A renderer owns a camera
   float *rendermat; // Renderer can use a global matrix for its scene. It is identity be default.
   Shader *current_shader; // Cache a shader. so it does not need to be set again unnecessarily
   Scene *scene;     // This is what I render
   int selected_object;
   bool select_mode=false;
   
};
