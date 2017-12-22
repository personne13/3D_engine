#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "window.h"
#include "scene.h"
#include "model.h"
#include "input.h"
#include "camera.h"

#define FPS 60

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
  Camera cam = CAMERA_empty_camera();
  GLfloat LightPos[4];

  in = INPUT_init();

	glEnable(GL_DEPTH_TEST);
  glEnable(GL_ALPHA_TEST);
  glEnable(GL_LIGHTING);
 	glEnable(GL_LIGHT0);

  LightPos[0] = 2;
  LightPos[1] = 1;
  LightPos[2] = 0;
  LightPos[3] = 1;
  int angle = 0;
  CAMERA_set_pos(&cam, -2, 2, -2);
  CAMERA_set_angles(&cam, 48, 120);


  while(!INPUT_isTriggered(in, LEAVE, 0)){
    INPUT_update(in);


		if(current_loop_update - last_loop_update > 1000 / FPS){
      CAMERA_move_pos_from_keyboard(&cam, in, current_loop_update - last_loop_update);
      CAMERA_move_target_from_mouse(&cam, in);

      angle++;
      if(angle > 360){
        angle = 0;
      }

      LightPos[0] = 2 * cos((double)angle * 3.1415 / 180.0);
      LightPos[2] = 2 * sin((double)angle * 3.1415 / 180.0);
			glPushMatrix();
			SCENE_mode_render(window, RENDER_3D, 70);
			CAMERA_set_camera(cam);
			SCENE_clear();
      glLightfv(GL_LIGHT0,GL_POSITION,LightPos);
			glClearColor( 0.f, 0.f, 0.f, 1.f );
      glColor3ub(255, 255, 255);
      glPointSize(5.0);
      glBegin(GL_POINTS);
        glVertex3f(LightPos[0], LightPos[1], LightPos[2]);
      glEnd();
			MODEL_render_model(cube);
			glColor3ub(255, 0, 0);

			SCENE_refresh(window);
			glPopMatrix();
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
