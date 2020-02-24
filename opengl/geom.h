#include "geombase.h"

class Renderer;

class Geom : public GeomBase {
public:
   int read_dummy(const char *filename);
   int read(const char *filename,const char* indexfile);
   int read(const char *filename,const char* indexfile,const char* vertex_shader, const char* fragment_shader,const char* texture=NULL,int id=-1);
   int render(Renderer *renderer, glm::mat4 rendermat, glm::mat4 rendermat2) const;
   glm::vec3 getObjectColor() const { return objectColor;}
   Geom(const char *filename=NULL) { read_dummy(filename); }
   Geom(const char *filename,const char* indexfile=NULL) { read(filename,indexfile);}
   //// parameters which are required
   // points file, indices file, color, vertex shader, fragment shader, texture_file
   Geom(const char *filename,const char* indexfile,glm::vec3 color,const char* vertex_shader, const char* fragment_shader, const char* texture,int id=-1){
      read(filename,indexfile,vertex_shader,fragment_shader,texture,id);
      setObjectColor(color);
   }
   int num_elements_global;
   GLuint texture_id;

};

