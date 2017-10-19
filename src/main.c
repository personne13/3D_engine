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
  Input *in;

  in = INPUT_init();

  while(!INPUT_isTriggered(in, LEAVE, 0)){
    INPUT_update(in);
  }

  WINDOW_destroy(window);

	quit_SDL();
	INPUT_free(in);

  return 0;
}
