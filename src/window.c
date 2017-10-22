#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "window.h"

int init_SDL(){
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
			fprintf(stderr, "Error initializing SDL\n");
			return 0;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 7);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	return 1;
}

void quit_SDL(){
	SDL_Quit();
}

Window * WINDOW_create_window(char *name, int width, int height){
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

    sprintf(window->name, name);
    window->width = width;
    window->height = height;
    window->w = w;

    return window;
}

// On quitte la SDL
void WINDOW_destroy(Window *window){
  SDL_GL_DeleteContext(window->context);
  SDL_DestroyWindow(window->w);
  free(window);
}
