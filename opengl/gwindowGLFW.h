#pragma once

#include "gwindow.h"

class GLFWwindow;

// Must have a renderloop that is called from main.
// Should also have an internalWindow() that ui may need
//
class gWindow_GLFW : public gWindow {
public:
   gWindow_GLFW(const char *name, int width=WIDTH, int height=HEIGHT, int major=3, int minor=3);
   ~gWindow_GLFW();

   GLFWwindow *internalWindow() { return window; }
   int renderloop(Renderer &r);

private:
   GLFWwindow* window;
};

