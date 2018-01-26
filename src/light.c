#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <math.h>
#include "materials.h"
#include "primitives.h"
#include "light.h"

#define DEFAULT_I_A 0.5
#define DEFAULT_I_D 0.5
#define DEFAULT_I_S 0.5

Light * LIGHT_create_light(double x, double y, double z){
  Light *light = malloc(sizeof(Light));

  if(!light){
    fprintf(stderr, "Error : cannot allocate light memory\n");
    return NULL;
  }

  light->pos = PRIMITIVES_get_point3d(x, y, z);
  light->state = SWITCHED_ON;

  for(int i = 0; i < 3; i++){
    light->i_a[i] = DEFAULT_I_A;
    light->i_d[i] = DEFAULT_I_D;
    light->i_s[i] = DEFAULT_I_S;
  }

  return light;
}

void LIGHT_set_pos_light(Light *light, double x, double y, double z){
  if(!light){
    fprintf(stderr, "Error : light not allocated (set_pos_light)\n");
    return;
  }

  light->pos = PRIMITIVES_get_point3d(x, y, z);
}

void LIGHT_switch_on(Light *light){
  if(!light){
    fprintf(stderr, "Error : light not allocated (switch_on)\n");
    return;
  }

  light->state = SWITCHED_ON;
}

void LIGHT_switch_off(Light *light){
  if(!light){
    fprintf(stderr, "Error : light not allocated (switch_off)\n");
    return;
  }

  light->state = SWITCHED_OFF;
}

Point3d LIGHT_get_pos_light(Light *light){
  if(!light){
    fprintf(stderr, "Error : light not allocated (get_pos_light)\n");
    exit(EXIT_FAILURE);
  }

  return light->pos;
}

void Light_destroy_light(Light *light){
  if(!light){
    fprintf(stderr, "Error : light not allocated (destroy_light)\n");
    return;
  }

  free(light);
}

int LIGHT_get_state_light(Light *light){
  if(!light){
    fprintf(stderr, "Error : light not allocated (get_state_light)\n");
    return 0;
  }

  return light->state;
}

void LIGHT_give_color(Light *l, Point3d *pos_pixel,
                      Point3d *pos_camera,
                      Triangle *triangle_target,
                      GLfloat *c,
                      int is_direct){
  GLfloat i_a[3];
  GLfloat i_d[3];
  GLfloat i_s[3];
  Point3d l_vec, r, v;
  double costheta, cosomega;
  if(is_direct){
    l_vec = PRIMITIVES_make_vec(LIGHT_get_pos_light(l), *pos_pixel);
    PRIMITIVES_normalize_vec(&l_vec);
    costheta = PRIMITIVES_scalar_product(triangle_target->normal, l_vec);
    r = PRIMITIVES_sub_vector(PRIMITIVES_mul_vector(2 * costheta, triangle_target->normal), l_vec);
    v = PRIMITIVES_make_vec(*pos_camera, *pos_pixel);
    PRIMITIVES_normalize_vec(&v);
    cosomega = PRIMITIVES_scalar_product(r, v);
  }

  for (int k = 0 ; k < 3 ; k++){
    i_a[k] = l->i_a[k] * triangle_target->mat.k_a;
    if(is_direct){
      i_d[k] = l->i_d[k] * triangle_target->mat.k_d * costheta;
      i_s[k] = l->i_s[k] * triangle_target->mat.k_s * pow(cosomega, triangle_target->mat.alpha);
    }
    else{
      i_d[k] = 0.0;
      i_s[k] = 0.0;
    }
  }

  for(int k = 0; k < 3; k++){
    c[k] = i_a[k] + i_d[k] + i_s[k];
  }
}
