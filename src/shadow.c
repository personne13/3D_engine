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

  printf("ratio : %lf %lf\n", w_ratio, h_ratio);
  PRIMITIVES_print_Triangle(*triangle);
  printf("vector width : ");
  PRIMITIVES_print_Point3d(vec_width);
  printf("vector height : ");
  PRIMITIVES_print_Point3d(vec_height);

  res = PRIMITIVES_add_vector(triangle->p[0], PRIMITIVES_add_vector(PRIMITIVES_mul_vector(w_ratio, vec_width), PRIMITIVES_mul_vector(h_ratio, vec_height)));

  return res;
}

int SHADOW_compute_shadow_map(Triangle *triangle_to_compute,
                              Triangle **all_triangles, int nb_total_triangles,
                              Light **lights, int nb_lights){
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

  printf("texture : (w%d) (h%d)\n", w, h);

  for(int i = 0; i < w; i++){
    for(int j = 0; j < h; j++){
    //for(int j = 0; j < (int)(h - (((double)i/w)*h)); j++){
      coords_pixels = SHADOW_get_absolute_coords_shadow_map(triangle_to_compute, (double)(i + 0.5)/(double)w, (double)(j + 0.5)/(double)h);
      int p = (i * h + j) * 3;//TODO : verifier bon indice
      buf[p] = 0.0f;//Red at 0
      buf[p + 1] = 0.0f;//Green at 0
      buf[p + 2] = 0.0f;//Blue at 0
      for(int k = 0; k < nb_lights; k++){
        if(LIGHT_get_state_light(lights[k]) == SWITCHED_ON){
          Point3d vec = PRIMITIVES_make_vec(LIGHT_get_pos_light(lights[k]), coords_pixels);
          printf("coords_pixels %d %d : ", i, j);
          PRIMITIVES_print_Point3d(coords_pixels);
          printf("coords_light : ");
          PRIMITIVES_print_Point3d(lights[k]->pos);
          Ray ray = PRIMITIVES_get_ray(coords_pixels, vec);

          PRIMITIVES_print_Ray(ray);
          if(!SHADOW_collision_ray_triangles(ray, triangle_to_compute, all_triangles, nb_total_triangles)){
            buf[p] += 0.3f;
            buf[p + 1] += 0.3f;
            buf[p + 2] += 0.3f;
          }
        }
      }
    }
  }

  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_FLOAT, buf);

  return 1;
}

int SHADOW_collision_ray_triangles(Ray ray,
                                   Triangle *triangle_to_compute,
                                   Triangle **all_triangles, int nb_total_triangles){
  for(int i = 0; i < nb_total_triangles; i++){
    if(all_triangles[i] != triangle_to_compute){
      printf("triangle %d ... : ", i);
      PRIMITIVES_print_Triangle(*all_triangles[i]);
      if(PRIMITIVES_collision_ray_triangle(ray, all_triangles[i], NULL)){
        printf("collision\n");
        return 1;
      }
      printf("no collision\n");
    }
  }

  return 0;
}

int SHADOW_compute_shadows(Model **models, int nb_models,
                           Light **lights, int nb_lights){
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

  printf("all triangles : (%d tot)\n", nb_total_triangles);
  for(int k = 0; k < nb_total_triangles; k++){
    printf("%p\n", all_triangles[k]);
  }

  for(int i = 0; i < nb_models; i++){
    for(int j = 0; j < models[i]->nb_triangles; j++){
      if(!models[i]->triangle[j].shadow_map){
        SHADOW_generate_shadow_map(&models[i]->triangle[j], SIZE_MAP, SIZE_MAP);
      }
      printf("Testing model %d, triangle %d\n", i, j);

      SHADOW_compute_shadow_map(&models[i]->triangle[j], all_triangles, nb_total_triangles, lights, nb_lights);
    }
  }

  return 1;
}
