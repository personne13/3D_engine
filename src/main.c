#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include "window.h"
#include "input.h"

int main(int argc, char **argv){
	if(!init_SDL())
		return 0;

  Window *window = WINDOW_create_window("test_engine", 800, 600);
  Input in;

  INPUT_init(&in);

  while(!in.leave){
    INPUT_update(&in);
  }

  WINDOW_destroy(window);

	quit_SDL();

  return 0;
}
