#ifndef SHADOW_H
#define SHADOW_H

  #include "primitives.h"
  #include "model.h"
  #include "light.h"

  //Gives to a triangle a shadow map, nb_pixels_width and nb_pixels_height have to be power of 2.
  int SHADOW_generate_shadow_map(Triangle *triangle, int nb_pixels_width, int nb_pixels_height);

  //Returns the absolute coords of a shadow map pixel.
  Point3d SHADOW_get_absolute_coords_shadow_map(Triangle *triangle, double w_ratio, double h_ratio);

  //Compute shadow maps foreach triangle of each model.
  int SHADOW_compute_shadows(Model **models, int nb_models,
                             Light **lights, int nb_lights);

  //Create a shadow map for triangle_to_compute.
  int SHADOW_compute_shadow_map(Triangle *triangle_to_compute,
                                Triangle **all_triangles, int nb_total_triangles,
                                Light **lights, int nb_lights);

  //Returns 1 if there is a collision between the ray and the other triangles, exluding triangle_to_compute.
  int SHADOW_collision_ray_triangles(Ray ray,
                                     Triangle *triangle_to_compute,
                                     Triangle **all_triangles, int nb_total_triangles,
                                     Point3d *collision_point);

#endif
