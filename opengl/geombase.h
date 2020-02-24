#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

class Renderer;
class Shader;

class GeomBase {

public:
   virtual int render(Renderer *renderer, glm::mat4 rendermat, glm::mat4 rendermat2) const = 0;// PV and model matrix
   virtual int read_dummy(const char *filename) = 0;
   virtual int read(const char *filename,const char* indexfile) = 0;
   virtual int read(const char *filename,const char* indexfile,const char* vertex_shader, const char* fragment_shader,const char* texture=NULL,int id=-1) = 0 ;
   glm::vec3 getObjectColor() const ;
   void setObjectColor(glm::vec3 c) { objectColor = c;}
   void attachshader(Shader *shad) { shader = shad; }
   GLuint vao;

protected:
   glm::vec3 objectColor;
   Shader *shader;
   Shader *pickingShader;
};
