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
#include "rendering.h"

#define FPS 60

int main(int argc, char **argv){
  if(!init_SDL())
		return 1;

  Window *window = WINDOW_create_window("test_engine", 1000, 700, 70);

	if(!init_glew())
		return 1;

	int last_loop_update = 0;
	int current_loop_update = 0;
  Input *in;
	Model *cube1 = MODEL_get_cube(50,1,50);
	Model *cube2 = MODEL_get_cube(5,4,9);
	Model *cube3 = MODEL_get_cube(2,2,2);
  Camera cam = CAMERA_empty_camera();
  Light *light = LIGHT_create_light(1, 20, 6);

  in = INPUT_init();

  int angle = 0;
  CAMERA_set_pos(&cam, 28.031269, 18.641634 , -32.671514);
  CAMERA_set_angles(&cam, 132.000000, 114.000000);


  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel (GL_SMOOTH);
  glPointSize(5.0);
  MODEL_translate_model(cube1, -25, 0, -25);
  MODEL_translate_model(cube2, 0, 6, 0);
  MODEL_translate_model(cube3, 0, 6, -7);
  Scene scene = RENDERING_new_scene();
  RENDERING_init_rendering(window);
  RENDERING_set_camera_scene(&scene, &cam);
  RENDERING_add_model_scene(&scene, cube1);
  RENDERING_add_model_scene(&scene, cube2);
  RENDERING_add_model_scene(&scene, cube3);
  RENDERING_add_light_scene(&scene, light);

  //glEnable(GL_DEPTH_TEST);
  int time_exec_shadow;
  while(!INPUT_isTriggered(in, LEAVE, 0)){
    INPUT_update(in);

		if(current_loop_update - last_loop_update > 1000 / FPS){
      /*if(INPUT_isTriggered(in, KEYBOARD, SDL_SCANCODE_Q)){
        Light **lis;
        lis = malloc(sizeof(Light *));
        lis[0] = light;
        Model **mdls;
        mdls = malloc(sizeof(Model *) * 3);
        mdls[0] = cube1;
        mdls[1] = cube2;
        mdls[2] = cube3;
        time_exec_shadow = SDL_GetTicks();
        Point3d pos_camera = PRIMITIVES_get_point3d(cam.pos[0], cam.pos[1], cam.pos[2]);
        SHADOW_compute_shadows(mdls, 3, lis, 1, &pos_camera);
        //SHADOW_compute_shadows(mdls, 1, lis, 1, &pos_camera);
        printf("time_exec_shadow: %d ms\n", SDL_GetTicks() - time_exec_shadow);
        free(lis);
        free(mdls);
      }
      glPushMatrix();
      CAMERA_move_pos_from_keyboard(&cam, in, current_loop_update - last_loop_update);
      if(INPUT_isTriggered(in, MOUSE, SDL_BUTTON_LEFT)){
        CAMERA_move_target_from_mouse(&cam, in);
      }
      SCENE_mode_render(window, RENDER_3D);
      CAMERA_set_camera(cam);
      SCENE_clear();

      glClearColor( 0.f, 0.f, 0.f, 1.f );
      angle++;
      if(angle > 360){
        angle = 0;
      }

      glColor3ub(255, 255, 255);
      Point3d LightPos = LIGHT_get_pos_light(light);
      LightPos.x =  10 *cos((double)angle * 3.1415 / 180.0);
      LightPos.z =  10 *sin((double)angle * 3.1415 / 180.0);//circle around the cube
      LIGHT_set_pos_light(light, LightPos.x, LightPos.y, LightPos.z);
      glBegin(GL_POINTS);
      glVertex3f(LightPos.x, LightPos.y, LightPos.z);
      glEnd();
      //glLightfv(GL_LIGHT0,GL_POSITION,LightPos);
			MODEL_render_model(cube1);
			MODEL_render_model(cube2);
			MODEL_render_model(cube3);
			SCENE_refresh(window);
			glPopMatrix();*/
      CAMERA_move_pos_from_keyboard(&cam, in, current_loop_update - last_loop_update);
      if(INPUT_isTriggered(in, MOUSE, SDL_BUTTON_LEFT)){
        CAMERA_move_target_from_mouse(&cam, in);
      }
      time_exec_shadow = SDL_GetTicks();
      RENDERING_render_scene(&scene, window);
      printf("time_exec_rendering : %d ms\n", SDL_GetTicks() - time_exec_shadow);

			current_loop_update = SDL_GetTicks();
      last_loop_update = current_loop_update;
		}
		else{
      SDL_Delay((1000 / FPS) - (current_loop_update - last_loop_update));
			current_loop_update = SDL_GetTicks();
		}
  }
  RENDERING_quit_rendering();
  WINDOW_destroy(window);

  quit_SDL();
  INPUT_free(in);

  MODEL_free_model(cube1);
  MODEL_free_model(cube2);

  return 0;
}
