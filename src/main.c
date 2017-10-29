#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include "window.h"
#include "scene.h"
#include "model.h"
#include "input.h"

#define FPS 50

int main(int argc, char **argv){
	if(!init_SDL())
		return 0;

  Window *window = WINDOW_create_window("test_engine", 800, 600);
	int last_loop_update = 0;
	int current_loop_update = 0;
  Input *in;
	Model *cube = MODEL_get_cube();

  in = INPUT_init();

	glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_ALPHA_TEST);

  while(!INPUT_isTriggered(in, LEAVE, 0)){
    INPUT_update(in);
		if(current_loop_update - last_loop_update > 1000 / FPS){
			SCENE_clear();
			SCENE_mode_render(window, RENDER_3D, 90);
			glPushMatrix();
			gluLookAt(0, 0, 0, 1, 1, 1, 0, 1, 0);
			MODEL_render_model(cube);
			glPopMatrix();
			SCENE_refresh();
			current_loop_update = last_loop_update;
			last_loop_update = SDL_GetTicks();
		}
		else{
			current_loop_update = SDL_GetTicks();
		}
  }

  WINDOW_destroy(window);

	quit_SDL();
	INPUT_free(in);

	MODEL_free_model(cube);

  return 0;
}
