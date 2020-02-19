#include "shader.h"
#include <fstream>

#include <iostream>

static char *filedata(const char* filename)
{
   if(filename == NULL) return NULL;

   // printf("%s\n",filename);
   FILE *fp = fopen(filename, "r");

   if (fp == NULL) { 
      printf("File Not Found!\n"); 
      return NULL; 
   } 
  
   fseek(fp, 0L, SEEK_END); 
   long int size = ftell(fp); 
   fseek(fp, 0L, SEEK_SET); 
   
   char *ch = new char[size+1];
   if(ch == NULL) return NULL;
   if (size == fread (ch, 1, size, fp)) {
      fclose(fp); 
      ch[size] = '\0';
      return ch;
   }
   fclose(fp);
   delete[] (ch);
   return NULL;
}


Shader::Shader(const char *vsfile, const char *fsfile)
{
   if(vsfile == NULL && fsfile == NULL) // Both should default or neither
      defaults();
   else
      programFromfile(vsfile, fsfile);
}

void Shader::getUniformIDs()
{
   // "MVP" as an arg is expected. Otherwise overwrite
   MVPid = glGetUniformLocation(program, "MVP");
}

static bool isCompiled(const char *prefix, GLuint shader)
{
   if(! glIsShader(shader)) return false;
   GLint status;
   glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
   if(status == GL_TRUE) return true;
   std::cerr << "*** " << prefix << " Shader compilation failed:\n";

   glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &status);
   if(status > 0) {
      char *log = new char[status];
      glGetShaderInfoLog(shader, status, &status, log);
      std::cerr << log << "----\n";
      delete[] log;
   }
   
   return false;
}

static bool isLinked(GLuint program)
{
   if(! glIsProgram(program)) return false;
   GLint status;
   glGetProgramiv(program, GL_LINK_STATUS, &status);
   if(status == GL_TRUE) return true;
   std::cerr << "*** Shader Program linking failed:\n";

   glGetProgramiv(program, GL_INFO_LOG_LENGTH, &status);
   if(status > 0) {
      char *log = new char[status];
      glGetProgramInfoLog(program, status, &status, log);
      std::cout << log << "----\n";
      delete[] log;
   }
   
   return false;
}
GLuint Shader::vsFromsrc(char vs_src[], bool dofree)
{
    if(vs_src == NULL) return -1;
    GLuint vsid = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vsid, 1, &vs_src, NULL);
    glCompileShader(vsid);
    GLuint status;
    if(dofree) delete[](vs_src);
    return vsid;
}

GLuint Shader::fsFromsrc(char fs_src[], bool dofree)
{
    if(fs_src == NULL) return -1;
    GLuint fsid = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fsid, 1, &fs_src, NULL);
    glCompileShader(fsid);
    if(dofree) delete[](fs_src);
    return fsid;
}

void Shader::programFromsrc(char *vs_src, char *fs_src, bool dofree)
{
   makeProgram(vsFromsrc(vs_src, dofree), fsFromsrc(fs_src, dofree));
}

void Shader::programFromfile(const char *vs_file, const char *fs_file)
{
   programFromsrc(filedata(vs_file), filedata(fs_file));
}

void Shader::makeProgram(GLuint vs, GLint fs)
{
    program = glCreateProgram();
    if(vs >= 0 && isCompiled("Vertex", vs)) glAttachShader(program, vs);
    if(fs >= 0 && isCompiled("Fragment", fs)) glAttachShader(program, fs);

    glLinkProgram(program);
    if(isLinked(program))
       glUseProgram(program);

    getUniformIDs();
}

void Shader::defaults()
{
   char vs_src[] = "\
         #version 410\n \
         layout(location = 0) in vec3 vp; \
         uniform mat4 MVP; \
         void main() { \
            gl_Position =  MVP * vec4(vp,1.0); \
         } \
      ";

   char fs_src[] = "\
         #version 410\n \
         out vec4 frag_colur; \
         void main() { \
           frag_color = vec4(0.5, 0.0, 0.5, 0.5); \
         } \
      ";

   programFromsrc(vs_src, fs_src);
}

