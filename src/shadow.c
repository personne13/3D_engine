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

  vec_width.x = triangle->p[1].x - triangle->p[0].x;
  vec_width.y = triangle->p[1].y - triangle->p[0].y;
  vec_width.z = triangle->p[1].z - triangle->p[0].z;

  vec_height.x = triangle->p[2].x - triangle->p[0].x;
  vec_height.y = triangle->p[2].y - triangle->p[0].y;
  vec_height.z = triangle->p[2].z - triangle->p[0].z;

  res.x = triangle->p[0].x + vec_width.x * w_ratio + vec_height.x * h_ratio;
  res.y = triangle->p[0].y + vec_width.y * w_ratio + vec_height.y * h_ratio;
  res.z = triangle->p[0].z + vec_width.z * w_ratio + vec_height.z * h_ratio;

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

  for(int i = 0; i < w; i++){
    for(int j = 0; j < h; j++){
      coords_pixels = SHADOW_get_absolute_coords_shadow_map(triangle_to_compute, (double)i/(double)w, (double)j/(double)h);
      int p = (i * SIZE_MAP + j) * 3;//TODO : verifier bon indice
      buf[p] = 0.0f;
      buf[p + 1] = 0.0f;
      buf[p + 2] = 0.0f;
      for(int k = 0; k < nb_lights; k++){
        Point3d vec = PRIMITIVES_sub_vec(coords_pixels, LIGHT_get_pos_light(lights[k]));
        Ray ray = PRIMITIVES_get_ray(coords_pixels, vec);
        if(!SHADOW_collision_ray_triangles(ray, triangle_to_compute, all_triangles, nb_total_triangles)){
          buf[p] += 0.3f;
          buf[p + 1] += 0.3f;
          buf[p + 2] += 0.3f;
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
      if(PRIMITIVES_collision_ray_triangle(ray, all_triangles[i])){
        return 1;
      }
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

  for(int i = 0; i < nb_models; i++){
    for(int j = 0; j < models[i]->nb_triangles; j++){
      if(!models[i]->triangle[j].shadow_map){
        SHADOW_generate_shadow_map(&models[i]->triangle[j], SIZE_MAP, SIZE_MAP);
      }
      SHADOW_compute_shadow_map(&models[i]->triangle[j], all_triangles, nb_total_triangles, lights, nb_lights);
    }
  }

  return 1;
}
