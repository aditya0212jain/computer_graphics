#pragma once

#include "glConstants.h"

#define WIDTH		640
#define HEIGHT		480

class Renderer;

class gWindow {

public:
   virtual int renderloop(Renderer &renderer) = 0;
   
   void setSize(int width, int height) {
     this->width = width;
     this->height = height;
   } 

   void setGLcontext(int major, int minor, bool coreProfile=false) {
      this->major = major;
      this->minor = minor;
      this->coreProfile = coreProfile;
   }

   int Width()  { return width; }
   int Height() { return height; }

protected:
   int width, height;
   int major, minor; 
   bool coreProfile;

   gWindow(int w=WIDTH, int h=HEIGHT, int maj=OPENGLREVMAJOR, int min=OPENGLREVMINOR, bool iscore=COREPROFILE)
   {
      setSize(w, h);
      setGLcontext(maj, min, iscore);
   }
   virtual ~gWindow()  {} 
};
