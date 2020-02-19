#pragma once

// Define here what you want.
#define USEGLFW

#include <GL/glew.h>

#ifdef USEGLFW

#include <GLFW/glfw3.h>

#define HELPERLIB GLFW
#define _CONST_PREFIX_	GLFW_
#define _KEY_PREFIX_	GLFW_KEY_
#define _CHAR_PREFIX_	GLFW_KEY_
#define _CLASS_SUFFIX_	_GLFW

#else // NOT USEGLFW

#ifdef USESDL

#include <SDL2/sdl.h>

#define HELPERLIB SDL
#define _CONST_PREFIX_	SDL_
#define _KEY_PREFIX_	SDLK_
#define _CHAR_PREFIX_	
// #define _CHAR_PREFIX_	SDL_KEY
#define _CLASS_SUFFIX_	_SDL

#define Q		'q'

#else // NOT USESDL

Never should be here

#endif // USESDL

#endif // USGLFW

// Utility macro to concatenate names
#define STRING(x)    	#x
#define CATINC1(a, b) 	STRING(a ## b.h)
#define CATINC(a, b)    CATINC1(a, b)
#define INC(a)       	CATINC(a, HELPERLIB)

#define CONCAT(P,S)	P ## S
#define CONC(x,y)	CONCAT(x,y)

// Define constants appropriately
#define FINDDEF(x)	CONC(_CONST_PREFIX_, x)
#define KEYDEF(x)	CONC(_KEY_PREFIX_, x)
#define CHARDEF(x)	CONC(_CHAR_PREFIX_, x)
#define DERIVED(x)	CONC(x, _CLASS_SUFFIX_)

