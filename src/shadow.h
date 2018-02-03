#ifndef SHADOW_H
#define SHADOW_H

  #include "primitives.h"
  #include "model.h"
  #include "light.h"

  //Returns 1 if there is a collision between the ray and the other triangles, exluding triangle_to_compute.
  int SHADOW_collision_ray_triangles(Ray ray,
                                     Triangle *triangle_to_compute,
                                     Triangle **all_triangles, int nb_total_triangles,
                                     Point3d *collision_point);

  int SHADOW_generate_shadow_map(Triangle *triangle);

#endif
