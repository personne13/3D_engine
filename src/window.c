#define GL2_prototypes 1

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "window.h"

//#define DEBUG

#if defined DEBUG

  void MessageCallback( GLenum source,
                        GLenum type,
                        GLuint id,
                        GLenum severity,
                        GLsizei length,
                        const GLchar* message,
                        const void* userParam );

#endif

int init_SDL(){
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
			fprintf(stderr, "Error initializing SDL\n");
			return 0;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	return 1;
}

int init_glew(){
	GLenum res = glewInit();

	if(res != GLEW_OK)
	{
	    fprintf(stderr, "Cannot init glew : %s\n", glewGetErrorString(res));
	    return 0;
	}

  #if defined DEBUG
    glEnable              ( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( (GLDEBUGPROC) MessageCallback, 0 );
  #endif

  return 1;
}

void quit_SDL(){
    SDL_Quit();
}

Window * WINDOW_create_window(char *name, int width, int height, int fov){
    Window *window = NULL;
    SDL_Window* w;

    window = malloc(sizeof(Window));

    if(!window){
      fprintf(stderr, "Error creating window\n");
      return NULL;
    }

    w = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if(!w){
      fprintf(stderr, "Error creating window\n");
      return NULL;
    }

    if(strlen(name) >= MAX_NAME - 1){
      fprintf(stderr, "Error creating window : name too long\n");
      return NULL;
    }

    window->context = SDL_GL_CreateContext(w);

    if(!window->context){
      fprintf(stderr, "Error creating context\n");
      return NULL;
    }

    sprintf(window->name, "%s", name);
    window->width = width;
    window->height = height;
    window->w = w;
    window->fov = fov;

    return window;
}

// On quitte la SDL
void WINDOW_destroy(Window *window){
  SDL_GL_DeleteContext(window->context);
  SDL_DestroyWindow(window->w);
  free(window);
}

#if defined DEBUG
void MessageCallback( GLenum source,
                      GLenum type,
                      GLuint id,
                      GLenum severity,
                      GLsizei length,
                      const GLchar* message,
                      const void* userParam ){
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}
#endif
