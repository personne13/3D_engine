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
#include "light.h"
#include "shadow.h"

#define FPS 60

int main(int argc, char **argv){
  if(!init_SDL())
		return 1;

  Window *window = WINDOW_create_window("test_engine", 1000, 700);

	if(!init_glew())
		return 1;

	int last_loop_update = 0;
	int current_loop_update = 0;
  Input *in;
	Model *cube1 = MODEL_get_cube();
	Model *cube2 = MODEL_get_cube();
  Camera cam = CAMERA_empty_camera();
  Light *light = LIGHT_create_light(4, 2, -1);

  in = INPUT_init();

  int angle = 0;
  CAMERA_set_pos(&cam, -2, 2, -2);
  CAMERA_set_angles(&cam, 48, 120);

  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel (GL_SMOOTH);
  glPointSize(5.0);

  MODEL_translate_model(cube2, 2, 0, 0);

  glEnable(GL_DEPTH_TEST);

  while(!INPUT_isTriggered(in, LEAVE, 0)){
    INPUT_update(in);

		if(current_loop_update - last_loop_update > 1000 / FPS){
      if(INPUT_isTriggered(in, KEYBOARD, SDL_SCANCODE_Q)){
        Light **lis;
        lis = malloc(sizeof(Light *));
        lis[0] = light;
        Model **mdls;
        mdls = malloc(sizeof(Model *) * 2);
        mdls[0] = cube1;
        mdls[1] = cube2;
        SHADOW_compute_shadows(mdls, 2, lis, 1);

        free(lis);
        free(mdls);
      }
      glPushMatrix();
      CAMERA_move_pos_from_keyboard(&cam, in, current_loop_update - last_loop_update);
      if(INPUT_isTriggered(in, MOUSE, SDL_BUTTON_LEFT)){
        CAMERA_move_target_from_mouse(&cam, in);
      }
      SCENE_mode_render(window, RENDER_3D, 70);
      CAMERA_set_camera(cam);
      SCENE_clear();

      glClearColor( 0.f, 0.f, 0.f, 1.f );
      angle++;
      if(angle > 360){
        angle = 0;
      }

      glColor3ub(255, 255, 255);
      Point3d LightPos = LIGHT_get_pos_light(light);
      /*LightPos.x =  4 *cos((double)angle * 3.1415 / 180.0);
      LightPos.z =  4 *sin((double)angle * 3.1415 / 180.0);//circle around the cube
      LIGHT_set_pos_light(light, LightPos.x, LightPos.y, LightPos.z);*/
      glBegin(GL_POINTS);
      glVertex3f(LightPos.x, LightPos.y, LightPos.z);
      glEnd();
      //glLightfv(GL_LIGHT0,GL_POSITION,LightPos);
			MODEL_render_model(cube1);
			MODEL_render_model(cube2);
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

  MODEL_free_model(cube1);
  MODEL_free_model(cube2);

  return 0;
}
