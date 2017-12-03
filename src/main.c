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
		return 1;

  Window *window = WINDOW_create_window("test_engine", 800, 600);

	if(!init_glew())
		return 1;

	int last_loop_update = 0;
	int current_loop_update = 0;
  Input *in;
	Model *cube = MODEL_get_cube();

  in = INPUT_init();

	//glEnable(GL_DEPTH_TEST);
  //glEnable(GL_ALPHA_TEST);

  while(!INPUT_isTriggered(in, LEAVE, 0)){
    INPUT_update(in);
		if(current_loop_update - last_loop_update > 1000 / FPS){
			SCENE_mode_render(window, RENDER_3D, 70);
			SCENE_clear();
			glPushMatrix();
			glClearColor( 0.f, 0.f, 0.f, 1.f );
			//MODEL_render_model(cube);
			glColor3ub(255, 0, 0);
			/*glBegin(GL_TRIANGLES);
				glVertex2f(-0.5, 0.0);
				glVertex2f( 0.0, 1.0);
				glVertex2f(0.5, 0.0);
			glEnd();

			glBegin(GL_TRIANGLES);
				glVertex2f(0.0, 100.0);
				glVertex2f(100.0, 100.0);
				glVertex2f(100.0, 0.0);
			glEnd();*/

			glBegin(GL_TRIANGLES);
				glVertex3f(0.0, 0.0, 0.0);
				glVertex3f(2.0, 0.0, 2.0);
				glVertex3f(0.0, 1.0, 2.0);
			glEnd();
			gluLookAt(-10, 2, -10, 0, 0, 0, 0, 1, 0);
			glPopMatrix();
			SCENE_refresh(window);
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
