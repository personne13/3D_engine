#ifndef CAMERA_H
#define CAMERA_H

#include "input.h"

typedef struct Camera Camera;
struct Camera{
  double pos[3];
  double target[3];
  double theta;// spheric coords
  double phi;// spheric coords, in degrees
  double sensivity;// mouse sensivity
};

Camera CAMERA_empty_camera();
void CAMERA_set_camera(Camera camera);//lookAt
void CAMERA_set_pos(Camera *cam, double x, double y, double z);
void CAMERA_set_angles(Camera *cam, double theta, double phi);
void CAMERA_update_target(Camera *cam);//update the target
void CAMERA_move_target_from_mouse(Camera *cam, Input *in);//Warning : Set the input xrel and yrel to 0.
void CAMERA_move_pos_from_keyboard(Camera *cam, Input *in, int delayed_time);// delayed_time : time delayed since last update, in ms

#endif
