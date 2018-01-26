#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include "primitives.h"
#include "light.h"

#define DEFAULT_I_A 1.0
#define DEFAULT_I_D 1.0
#define DEFAULT_I_S 1.0

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

void LIGHT_give_color(Light *l, Point3d p, GLfloat *c, int is_direct){
  double distance = PRIMITIVES_distance(p,l->pos);
  for (int k = 0 ; k < 3 ; k++){
    //printf("%f\n",l->color[k] * att_factor / (distance));
    //c[k] += (double)l->color[k] * att_factor / (distance);//TODO fix
    if(is_direct)
      c[k] +=  20 / (distance * distance);
  }
}
