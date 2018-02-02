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
  int RENDERING_init_rendering(Window *window);
  int RENDERING_add_model_scene(Scene *s, Model *mdl);
  int RENDERING_add_light_scene(Scene *s, Light *light);
  void RENDERING_set_camera_scene(Scene *s, Camera *cam);

#endif
