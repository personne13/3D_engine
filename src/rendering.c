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

void compute_scene(Scene *s);
void render_buffer();
int get_index_buffer(int i, int j);
void get_color_pixel(Scene *scene, int i, int j, Triangle **all_triangles, int nb_triangles);

Scene RENDERING_new_scene(){
  Scene s;

  s.nb_models = 0;
  s.nb_lights = 0;
  s.cam = NULL;

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

  if(!s->cam){
    fprintf(stderr, "Error : camera not set on scene\n");
    return;
  }

  SCENE_mode_render(win, RENDER_3D);
  CAMERA_set_camera(*s->cam);
  compute_scene(s);
  SCENE_mode_render(win, RENDER_2D);
  render_buffer();
  SCENE_refresh(win);
}

int get_index_buffer(int i, int j){
  return i * h_window + j;
}

void compute_scene(Scene *s){
  Triangle *all_triangles[MAX_TRIANGLES_SCENE];
  int nb_total_triangles = 0;

  for(int i = 0; i < s->nb_models; i++){
    for(int j = 0; j < s->model[i]->nb_triangles; j++){
      if(nb_total_triangles >= MAX_TRIANGLES_SCENE){
        fprintf(stderr, "Error : Too many triangles\n");
      }
      all_triangles[nb_total_triangles] = &s->model[i]->triangle[j];
      nb_total_triangles++;
    }
  }

  for(int i = 0; i < w_window; i++){
    for(int j = 0; j < h_window; j++){
      get_color_pixel(s, i, j, all_triangles, nb_total_triangles);
    }
  }
}

void RENDERING_get_real_coords_pixel(int i, int j, Point3d *coords){
  GLint viewport[4];
  GLdouble mvmatrix[16], projmatrix[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
  glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);
  glGetIntegerv(GL_VIEWPORT, viewport);
  gluUnProject(i, j, 0, mvmatrix, projmatrix, viewport, &coords->x, &coords->y, &coords->z);
}

void get_color_pixel(Scene *scene, int i, int j, Triangle **all_triangles, int nb_triangles){
  Point3d coords_pixel;
  Point3d pos_camera;
  Point3d vec_ray;
  Ray ray_camera;
  Point3d last_collision, collision_point;
  Triangle *triangle_last_collision = NULL;
  double last_distance;
  int index_current_buffer = get_index_buffer(i, j);

  pos_camera = PRIMITIVES_get_point3d(scene->cam->pos[0], scene->cam->pos[1], scene->cam->pos[2]);

  RENDERING_get_real_coords_pixel(i, j, &coords_pixel);

  PRIMITIVES_make_vec(&coords_pixel, &pos_camera, &vec_ray);
  PRIMITIVES_get_ray(&pos_camera, &vec_ray, &ray_camera);//ray between camera and pixel.

  for(int t = 0; t < nb_triangles; t++){
    if(PRIMITIVES_collision_ray_triangle(ray_camera, all_triangles[t], &collision_point)){
      double tmp = PRIMITIVES_distance(&pos_camera, &collision_point);
      if(triangle_last_collision){
        if(tmp < last_distance){
          last_distance = tmp;
          last_collision = collision_point;
          triangle_last_collision = all_triangles[t];
        }
      }
      else{
        last_distance = tmp;
        last_collision = collision_point;
        triangle_last_collision = all_triangles[t];
      }
    }
  }//at this point the nearest intersection is known

  if(!triangle_last_collision){
    buf_screen[index_current_buffer] = 0.0;
    buf_screen[index_current_buffer + 1] = 0.0;
    buf_screen[index_current_buffer + 2] = 0.0;
    return;
  }

  for(int k = 0; k < scene->nb_lights; k++){//is the collision point alight
    if(LIGHT_get_state_light(scene->light[k]) == SWITCHED_ON){
      Point3d vec;
      PRIMITIVES_make_vec(&scene->light[k]->pos, &last_collision, &vec);
      Ray ray;
      PRIMITIVES_get_ray(&last_collision, &vec, &ray);
      Point3d intersection;
      int is_direct = !SHADOW_collision_ray_triangles(ray, triangle_last_collision,
                                                      all_triangles, nb_triangles,
                                                      &intersection);
      LIGHT_give_color(scene->light[k], &last_collision, &pos_camera, triangle_last_collision, &buf_screen[index_current_buffer], is_direct);
    }
  }
}

void render_buffer(){
  glPointSize(1.0);
  glBegin(GL_POINTS);
  for(int i = 0; i < w_window; i++){
    for(int j = 0; j < h_window; j++){
      int index = get_index_buffer(i, j);
      glColor3f(buf_screen[index], buf_screen[index + 1], buf_screen[index + 2]);
      glVertex2i(i, j);
    }
  }
  glColor3f(1, 0, 0);
  glVertex2i(1, 10);
  glEnd();
}
