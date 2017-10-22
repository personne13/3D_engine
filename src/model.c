#include <stdio.h>
#include <stdlib.h>
#include "model.h"

struct Point3d{
	double x;
	double y;
	double z;
};

struct Triangle{
	Point3d p[3];
};

struct Model{
	int nb_triangles;
	Triangle *triangle;
};

Point3d MODEL_get_point3d(double x, double y, double z){
	Point3d p = {x, y, z};

	return p;
}

Triangle MODEL_get_triangle(Point3d p1, Point3d p2, Point3d p3){
	Triangle t;

	t.p[0] = p1;
	t.p[1] = p2;
	t.p[2] = p3;

	return t;
}

Model * MODEL_get_model(int nb_triangles, Triangle *t){
	Model *m = malloc(sizeof(Model));

	if(!m){
		fprintf(stderr, "Error allocating model memory\n");
		return NULL;
	}

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
	for(int i = 0; i < model->nb_triangles; i++){
		free(model->triangle);
	}

	free(model);
}
