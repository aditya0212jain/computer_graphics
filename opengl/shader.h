#pragma once

#include <GL/glew.h>
#include <stdio.h>

class Shader {

public:
   int  id()  { return program; }
   bool use() { glUseProgram(program); return(glGetError() == GL_NO_ERROR); }

   void setXform(const GLfloat *mvp) {
      glUniformMatrix4fv(MVPid, 1, GL_FALSE, mvp);
   }

   Shader(const char *vsfile=NULL, const char *fsfile=NULL);

protected:
   GLuint program;
   GLuint MVPid; // Every shader must have at least an MVP.
   void defaults();
   void makeProgram(GLuint vs, GLint fs);
   virtual void getUniformIDs();
   GLuint vsFromsrc(char *vs_src, bool dofree=true);
   GLuint fsFromsrc(char *fs_src, bool dofree=true);
   void programFromsrc(char *vs_src, char *fs_src, bool dofree=true);
   void programFromfile(const char *vs_file, const char *fs_file);
};

