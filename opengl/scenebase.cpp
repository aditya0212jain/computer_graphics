#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scenebase.h"
#include "camera.h"
#include "geombase.h"
#include "render.h"
#include<iostream>

int SceneBase::render(Renderer *renderer, const Camera &camera, const float *mat)
{
   if(mat == NULL) 
      return render(renderer, camera, glm::mat4(1.0f));
   else
      return render(renderer, camera, glm::make_mat4(mat));
}

int SceneBase::render(Renderer *renderer, const Camera &camera, glm::mat4 mat) {
   glm::mat4 nextmat = mat * objmat; // objmat must not be NULL
   object->render(renderer,camera.matrix(), nextmat);// this is PV*M // sent as camera matrix (PV) and model matrix 
   bool result = true;
   for(SceneBase* cur = firstchild; cur != NULL; cur = cur->sibling) {
      result &= cur->render(renderer, camera, nextmat);
   }
   return result;
}

void SceneBase::updateScene(int vao_id,glm::mat4 transformMat){
   // std::cout<<"object vao: "<<object->vao<<"\n";
   if(object->vao==vao_id){
      transformScene(transformMat);
      return;
   }
   for(SceneBase* cur = firstchild;cur!=NULL;cur=cur->sibling){
      cur->updateScene(vao_id,transformMat);
   }
   return;
}

SceneBase* SceneBase::addchild(const GeomBase *obj, glm::mat4 mat) {
   SceneBase *sn = new SceneBase(obj, mat, firstchild, NULL);
   firstchild = sn;
   return sn;
}

void SceneBase::init(const GeomBase *obj, glm::mat4 mat, SceneBase *next, SceneBase *child)
{
   set(obj); set(mat);
   this->sibling = next;
   this->firstchild = child;
}

SceneBase::SceneBase(const GeomBase *obj, glm::mat4 mat, SceneBase *next, SceneBase *child)
{
   init(obj, mat, next, child);
}

void SceneBase::attachSibling(SceneBase* sib){
   sibling = sib;
}

void SceneBase::attachChild(SceneBase* ch){
   firstchild = ch;
}

void SceneBase::transformScene(glm::mat4 r_mat){
   objmat = r_mat * objmat;
}

