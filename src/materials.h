#ifndef MATERIALS_H
#define MATERIALS_H

  typedef struct Material Material;
  struct Material{
    double k_a;//ambiant component, between 0 and 1
    double k_d;//diffuse component, between 0 and 1
    double k_s;//specular component, between 0 and 1
    int alpha;//material shine, more than 1. The larger it is, the shinest the surface is.
  };

  Material MATERIAL_create_material(double k_a, double k_d, double k_s, double alpha);
  Material MATERIAL_create_default();

#endif
