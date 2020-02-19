#pragma once

#include "ui.h"
#include "gwindowGLFW.h"

// Should maintain a reference to a window, returned by getwindow, which the UI would usually need

class UI_GLFW : public UI {

public:
   UI_GLFW(Renderer *renderer, gWindow_GLFW *window);

   float cursorY, cursorX;

private:

   gWindow_GLFW* getwindow();

   void makeCurrent();
   void setCallBacks(GLFWwindow* win);
   void setEventAction();

   static UI_GLFW *currentUI;

   static void handleMouseClickGLFW(GLFWwindow* window, int button, int action, int mods);
   static void handleKeysGLFW(GLFWwindow* window, int key, int scancode, int action, int mods);
   static void handleResizeGLFW(GLFWwindow* window, int width, int height);
   static void handleMouseMotionGLFW(GLFWwindow* window, double x, double y);
};
