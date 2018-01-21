#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include "primitives.h"
#include "light.h"

Light * LIGHT_create_light(double x, double y, double z){
  Light *light = malloc(sizeof(Light));

  if(!light){
    fprintf(stderr, "Error : cannot allocate light memory\n");
    return NULL;
  }

  light->pos = PRIMITIVES_get_point3d(x, y, z);
  light->state = SWITCHED_ON;

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
