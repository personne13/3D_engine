#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include "primitives.h"
#include "light.h"
#include "model.h"
#include "shadow.h"

#define SIZE_MAP 128
#define MAX_TRIANGLES_SCENE 128

int SHADOW_generate_shadow_map(Triangle *triangle, int nb_pixels_width, int nb_pixels_height){
  if(!triangle){
    fprintf(stderr, "Error : triangle not initializated.\n");
    return 0;
  }
  if(triangle == NULL || triangle->shadow_map != 0){
    fprintf(stderr, "Error : shadow map already created.\n");
    return 0;
  }
  if(nb_pixels_width <= 0 || nb_pixels_height <= 0){
    fprintf(stderr, "Error : bad quality argument generating shadow map.\n");
    return 0;
  }

  GLuint shadow_map;
  glGenTextures(1, &shadow_map);
  glBindTexture(GL_TEXTURE_2D, shadow_map);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nb_pixels_width, nb_pixels_height, 0, GL_RGB, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  triangle->shadow_map = shadow_map;

  return 1;
}

Point3d SHADOW_get_absolute_coords_shadow_map(Triangle *triangle, double w_ratio, double h_ratio){
  Point3d res;
  Point3d vec_width;
  Point3d vec_height;

  if(!triangle->shadow_map){
    fprintf(stderr, "Error : triangle don't have any shadow map.\n");
    return res;
  }

  vec_width = PRIMITIVES_make_vec(triangle->p[1], triangle->p[0]);
  vec_height = PRIMITIVES_make_vec(triangle->p[2], triangle->p[0]);

  res = PRIMITIVES_add_vector(triangle->p[0], PRIMITIVES_add_vector(PRIMITIVES_mul_vector(w_ratio, vec_width), PRIMITIVES_mul_vector(h_ratio, vec_height)));

  return res;
}

int SHADOW_compute_shadow_map(Triangle *triangle_to_compute,
                              Triangle **all_triangles, int nb_total_triangles,
                              Light **lights, int nb_lights,
                              Point3d *pos_camera){
  GLint w = 0, h = 0;
  GLfloat buf[SIZE_MAP * SIZE_MAP * 3];
  Point3d coords_pixels;

  if(!triangle_to_compute->shadow_map){
    fprintf(stderr, "Error : shadow map used before its creation\n");
    return 0;
  }

  glBindTexture(GL_TEXTURE_2D, triangle_to_compute->shadow_map);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);

  for(int i = 0; i < w; i++){
    for(int j = 0; j < (int)(h - (((double)i/w)*h)); j++){
      coords_pixels = SHADOW_get_absolute_coords_shadow_map(triangle_to_compute, (double)(i + 0.5)/(double)w, (double)(j + 0.5)/(double)h);
      int p = (i * h + j) * 3;
      for(int k = 0; k < nb_lights; k++){
        if(LIGHT_get_state_light(lights[k]) == SWITCHED_ON){
          Point3d vec = PRIMITIVES_make_vec(LIGHT_get_pos_light(lights[k]), coords_pixels);
          Ray ray = PRIMITIVES_get_ray(coords_pixels, vec);
          Point3d intersection;
          int is_direct = !SHADOW_collision_ray_triangles(ray, triangle_to_compute,
                                             all_triangles, nb_total_triangles,
                                             &intersection);
          LIGHT_give_color(lights[k], &coords_pixels, pos_camera, triangle_to_compute, &buf[p], is_direct);
        }
      }
    }
  }

  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_FLOAT, buf);

  return 1;
}

int SHADOW_collision_ray_triangles(Ray ray,
                                   Triangle *triangle_to_compute,
                                   Triangle **all_triangles, int nb_total_triangles,
                                   Point3d *collision_point){
  for(int i = 0; i < nb_total_triangles; i++){
    if(all_triangles[i] != triangle_to_compute){
      if(PRIMITIVES_collision_ray_triangle(ray, all_triangles[i], collision_point)){
        Triangle *tmp = all_triangles[i];
        for(int j = i; j > 0; j--){
          all_triangles[j] = all_triangles[j - 1];
        }
        all_triangles[0] = tmp;
        return 1;
      }
    }
  }

  return 0;
}

int SHADOW_compute_shadows(Model **models, int nb_models,
                           Light **lights, int nb_lights,
                           Point3d *pos_camera){
  Triangle *all_triangles[MAX_TRIANGLES_SCENE];
  int nb_total_triangles = 0;

  for(int i = 0; i < nb_models; i++){
    for(int j = 0; j < models[i]->nb_triangles; j++){
      if(nb_total_triangles >= MAX_TRIANGLES_SCENE){
        fprintf(stderr, "Error : Too many triangles\n");
      }
      all_triangles[nb_total_triangles] = &models[i]->triangle[j];
      nb_total_triangles ++;
    }
  }

  for(int i = 0; i < nb_models; i++){
    for(int j = 0; j < models[i]->nb_triangles; j++){
      if(!models[i]->triangle[j].shadow_map){
        SHADOW_generate_shadow_map(&models[i]->triangle[j], SIZE_MAP, SIZE_MAP);
      }

      SHADOW_compute_shadow_map(&models[i]->triangle[j], all_triangles, nb_total_triangles, lights, nb_lights, pos_camera);
    }
  }

  return 1;
}
