#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

double rad_to_deg(double rad){
  return rad * 180 / PI;
}

double deg_to_rad(double deg){
  return deg * PI / 180;
}
