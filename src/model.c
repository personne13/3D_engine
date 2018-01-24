#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include "primitives.h"
#include "model.h"
#include "shadow.h"

Model * MODEL_get_model(int nb_triangles, Triangle *t){
	Model *m = malloc(sizeof(Model));

	if(!m){
		fprintf(stderr, "Error allocating model memory\n");
		return NULL;
	}

	m->nb_triangles = nb_triangles;
	m->triangle = malloc(sizeof(Triangle) * nb_triangles);

	if(!m->triangle){
		fprintf(stderr, "Error allocating model memory (%d triangles)\n", nb_triangles);
		return NULL;
	}

	for(int i = 0; i < nb_triangles; i++){
		m->triangle[i] = t[i];
	}

	return m;
}

void MODEL_free_model(Model *model){
	free(model->triangle);
	free(model);
}

void MODEL_render_model(Model *model){
	for(int i = 0; i < model->nb_triangles; i++){
		PRIMITIVES_render_triangle(&model->triangle[i]);
	}
}

Model * MODEL_get_cube(){
	Point3d p[8];
	Triangle t[12];

	p[0] = PRIMITIVES_get_point3d(1, 0, 0);
	p[1] = PRIMITIVES_get_point3d(0, 0, 0);
	p[2] = PRIMITIVES_get_point3d(0, 1, 0);
	p[3] = PRIMITIVES_get_point3d(1, 1, 0);
	p[4] = PRIMITIVES_get_point3d(1, 0, 1);
	p[5] = PRIMITIVES_get_point3d(0, 0, 1);
	p[6] = PRIMITIVES_get_point3d(0, 1, 1);
	p[7] = PRIMITIVES_get_point3d(1, 1, 1);

  t[0] = PRIMITIVES_get_triangle(p[0], p[1], p[3]);
	t[1] = PRIMITIVES_get_triangle(p[2], p[3], p[1]);
	t[2] = PRIMITIVES_get_triangle(p[1], p[5], p[2]);
	t[3] = PRIMITIVES_get_triangle(p[6], p[2], p[5]);
	t[4] = PRIMITIVES_get_triangle(p[4], p[7], p[5]);
	t[5] = PRIMITIVES_get_triangle(p[6], p[5], p[7]);
	t[6] = PRIMITIVES_get_triangle(p[0], p[3], p[4]);
	t[7] = PRIMITIVES_get_triangle(p[7], p[4], p[3]);
	t[8] = PRIMITIVES_get_triangle(p[0], p[4], p[1]);
	t[9] = PRIMITIVES_get_triangle(p[5], p[1], p[4]);
	t[10] = PRIMITIVES_get_triangle(p[3], p[2], p[7]);
	t[11] = PRIMITIVES_get_triangle(p[6], p[7], p[2]);

	Model *m = MODEL_get_model(12, t);

	return m;
}

void MODEL_translate_model(Model *m, double x, double y, double z){
  for(int i = 0; i < m->nb_triangles; i++){
    for(int j = 0; j < 3; j++){
      m->triangle[i].p[j].x += x;
      m->triangle[i].p[j].y += y;
      m->triangle[i].p[j].z += z;
    }
  }
}
