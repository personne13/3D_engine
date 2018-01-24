#ifndef LIGHT_H
#define LIGHT_H

  #include "primitives.h"

  enum{SWITCHED_OFF, SWITCHED_ON};
  enum{RED,GREEN,BLUE};

  typedef struct Light Light;
  struct Light{
    Point3d pos;
    GLfloat color[3];
    int state;//1 : switched on, 0 : switched off
  };

  Light * LIGHT_create_light(double x, double y, double z);
  void LIGHT_switch_on(Light *light);
  void LIGHT_switch_off(Light *light);
  Point3d LIGHT_get_pos_light(Light *light);
  void LIGHT_set_pos_light(Light *light, double x, double y, double z);
  int LIGHT_get_state_light(Light *light);
  void LIGHT_destroy_light(Light *light);
  //Gives the color c to p, c is an allocated 3-sized buffer
  void LIGHT_give_color(Light *l, Point3d p, GLfloat *c, GLfloat att_factor);
#endif
