#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>

#include "primitives.h"
#include "camera.h"
#include "window.h"
#include "scene.h"
#include "model.h"
#include "light.h"
#include "shadow.h"
#include "rendering.h"

GLfloat *buf_screen = NULL;
int w_window = 0;
int h_window = 0;

void render_buffer();
int get_index_buffer(int i, int j);

Scene RENDERING_new_scene(){
  Scene s;

  s.nb_models = 0;
  s.nb_lights = 0;

  return s;
}

int RENDERING_init_rendering(Window *window){
  w_window = window->width;
  h_window = window->height;

  buf_screen = malloc(w_window * h_window * sizeof(GLfloat) * 3);

  if(!buf_screen){
    fprintf(stderr, "Error : cannot allocate window rendering memory (w:%d) (h:%d)\n", w_window, h_window);
    return 0;
  }

  return 1;
}

void RENDERING_quit_rendering(){
  if(buf_screen){
    free(buf_screen);
    buf_screen = NULL;
  }
}

int RENDERING_add_model_scene(Scene *s, Model *mdl){
  if(s->nb_models >= MAX_MODELS_SCENE){
    fprintf(stderr, "Error : too many models in scene (%d)\n", s->nb_models);
    return 0;
  }

  s->model[s->nb_models] = mdl;
  s->nb_models++;
  return 1;
}

int RENDERING_add_light_scene(Scene *s, Light *light){
  if(s->nb_models >= MAX_MODELS_SCENE){
    fprintf(stderr, "Error : too many lights in scene (%d)\n", s->nb_lights);
    return 0;
  }

  s->light[s->nb_lights] = light;
  s->nb_lights++;
  return 1;
}

void RENDERING_set_camera_scene(Scene *s, Camera *cam){
  s->cam = cam;
}

void RENDERING_render_scene(Scene *s, Window *win){
  if(!buf_screen){
    fprintf(stderr, "Error : rendering not initializated\n");
    return;
  }

  SCENE_mode_render(win, RENDER_2D, 0);
  render_buffer();
  SCENE_refresh(win);
  printf("Scene rendered : %d\n", SDL_GetTicks());
}

int get_index_buffer(int i, int j){
  return i * h_window + j;
}

void render_buffer(){
  glPointSize(1.0);
  glBegin(GL_POINTS);
  printf("%d %d\n\n", w_window, h_window);
  for(int i = 0; i < w_window; i++){
    for(int j = 0; j < h_window; j++){
      int index = get_index_buffer(i, j);
      glColor3f(buf_screen[index], buf_screen[index + 1], buf_screen[index + 2]);
      glVertex2i(i, j);
    }
  }
  glEnd();
}
