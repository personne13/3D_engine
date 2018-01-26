#include <stdio.h>
#include <stdlib.h>
#include "materials.h"

#define DEFAULT_K_A 0.5
#define DEFAULT_K_D 0.5
#define DEFAULT_K_S 1.0
#define DEFAULT_ALPHA 10

Material MATERIAL_create_material(double k_a, double k_d, double k_s, double alpha){
  Material material;

  material.k_a = k_a;
  material.k_d = k_d;
  material.k_s = k_s;
  material.alpha = alpha;

  return material;
}

Material MATERIAL_create_default(){
  return MATERIAL_create_material(DEFAULT_K_A, DEFAULT_K_D, DEFAULT_K_S, DEFAULT_ALPHA);
}
