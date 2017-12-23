#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "camera.h"
#include "utils.h"

void get_normal_vector(double v1[3], double v2[3]);/*Stores in v2 the normal vector to v1, of norm 1,
                                                    with y = 0, (2 solutions, only 1 given, the other is -v2).*/

Camera CAMERA_empty_camera(){
  Camera cam;

  cam.pos[0] = 0;
  cam.pos[1] = 0;
  cam.pos[2] = 0;

  cam.target[0] = 0;
  cam.target[1] = 0;
  cam.target[2] = 0;

  cam.theta = 0;
  cam.phi = 0;

  cam.sensivity = 1.5;

  return cam;
}

void CAMERA_set_camera(Camera cam){
  gluLookAt(cam.pos[0], cam.pos[1], cam.pos[2], cam.target[0], cam.target[1], cam.target[2], 0, 1, 0);
}

void CAMERA_set_pos(Camera *cam, double x, double y, double z){
  cam->pos[0] = x;
  cam->pos[1] = y;
  cam->pos[2] = z;

  CAMERA_update_target(cam);
}

void CAMERA_set_angles(Camera *cam, double theta, double phi){
  cam->theta = theta;
  cam->phi = phi;

  CAMERA_update_target(cam);
}

void CAMERA_update_target(Camera *cam){
  double dist = 1;
  double sinp = sin(deg_to_rad(cam->phi));
  double cosp = cos(deg_to_rad(cam->phi));
  double sint = sin(deg_to_rad(cam->theta));
  double cost = cos(deg_to_rad(cam->theta));

  cam->target[0] = cam->pos[0] + dist * sinp * cost;
  cam->target[2] = cam->pos[2] + dist * sinp * sint;
  cam->target[1] = cam->pos[1] + dist * cosp;
}

void get_normal_vector(double v1[3], double v2[3]){
  double x2 = v1[0] * v1[0];
  double z2 = v1[2] * v1[2];

  v2[0] = -sqrt(z2 / (x2 + z2));
  v2[1] = 0;
  v2[2] = sqrt(x2 / (x2 + z2));

  if(v1[2] > 0){
    v2[0] *= -1;
  }
  if(v1[0] > 0){
    v2[2] *= -1;
  }
}

void CAMERA_move_pos_from_keyboard(Camera *cam, Input *in, int delayed_time){
  double dir[3];
  double pos[3];
  double speed = 1;
  double factor = speed * delayed_time / 1000;

  pos[0] = cam->pos[0];
  pos[1] = cam->pos[1];
  pos[2] = cam->pos[2];

  dir[0] = cam->target[0] - pos[0];
  dir[1] = cam->target[1] - pos[1];
  dir[2] = cam->target[2] - pos[2];

  if(INPUT_isTriggered(in, KEYBOARD, SDL_SCANCODE_W)){//forward
    pos[0] += factor * dir[0];
    pos[1] += factor * dir[1];
    pos[2] += factor * dir[2];
    CAMERA_set_pos(cam, pos[0], pos[1], pos[2]);
  }

  else if(INPUT_isTriggered(in, KEYBOARD, SDL_SCANCODE_S)){//backward
    pos[0] -= factor * dir[0];
    pos[1] -= factor * dir[1];
    pos[2] -= factor * dir[2];
    CAMERA_set_pos(cam, pos[0], pos[1], pos[2]);
  }

  else if(INPUT_isTriggered(in, KEYBOARD, SDL_SCANCODE_A)){//left
    double norm_vec[3];
    get_normal_vector(dir, norm_vec);
    pos[0] += factor * norm_vec[0];
    pos[1] += factor * norm_vec[1];
    pos[2] += factor * norm_vec[2];
    CAMERA_set_pos(cam, pos[0], pos[1], pos[2]);
  }

  else if(INPUT_isTriggered(in, KEYBOARD, SDL_SCANCODE_D)){//right
    double norm_vec[3];
    get_normal_vector(dir, norm_vec);
    pos[0] -= factor * norm_vec[0];
    pos[1] -= factor * norm_vec[1];
    pos[2] -= factor * norm_vec[2];
    CAMERA_set_pos(cam, pos[0], pos[1], pos[2]);
  }
}

void CAMERA_move_target_from_mouse(Camera *cam, Input *in){
  double th = cam->theta;
  double ph = cam->phi;
  double s = cam->sensivity;
  int offset_th;
  int offset_ph;

  INPUT_getRelativeCoords(in, &offset_th, &offset_ph);

  th += s * (double)offset_th;
  ph += s * (double)offset_ph;

  if(th > 360)
    th = 0;
  else if(th < 0)
    th = 360;
  if(ph > 180)
    ph = 180;
  else if(ph < 0)
    ph = 0;

  CAMERA_set_angles(cam, th, ph);
}
