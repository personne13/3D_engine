#ifndef RENDERING_H
#define RENDERING_H

  #include "primitives.h"
  #include "camera.h"
  #include "window.h"
  #include "model.h"
  #include "light.h"
  #include "shadow.h"

  #define MAX_MODELS_SCENE 10
  #define MAX_LIGHTS_SCENE 4

  typedef struct Scene Scene;
  struct Scene{
    Model *model[MAX_MODELS_SCENE];
    int nb_models;
    Light *light[MAX_LIGHTS_SCENE];
    int nb_lights;
    Camera *cam;
  };

  Scene RENDERING_new_scene();
  void RENDERING_get_real_coords_pixel(int i, int j, Point3d *coords);
  int RENDERING_init_rendering(Window *window);
  int RENDERING_add_model_scene(Scene *s, Model *mdl);
  int RENDERING_add_light_scene(Scene *s, Light *light);
  void RENDERING_set_camera_scene(Scene *s, Camera *cam);
  void RENDERING_render_scene(Scene *s, Window *win);
  void RENDERING_quit_rendering();

#endif
