#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include "model.h"

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

void render_triangle(Triangle *t){
	glBegin(GL_TRIANGLES);
		glVertex3f(t->p[0].x, t->p[0].y, t->p[0].z);
		glVertex3f(t->p[1].x, t->p[1].y, t->p[1].z);
		glVertex3f(t->p[2].x, t->p[2].y, t->p[2].z);
	glEnd();
}

void MODEL_render_model(Model *model){
	for(int i = 0; i < model->nb_triangles; i++){
		render_triangle(&model->triangle[i]);
	}
}

Model * MODEL_get_cube(){
	Point3d p[8];
	Triangle t[12];

	p[0] = MODEL_get_point3d(0, 0, 0);
	p[1] = MODEL_get_point3d(0, 0, 1);
	p[2] = MODEL_get_point3d(0, 1, 1);
	p[3] = MODEL_get_point3d(0, 1, 0);
	p[4] = MODEL_get_point3d(1, 0, 0);
	p[5] = MODEL_get_point3d(1, 0, 1);
	p[6] = MODEL_get_point3d(1, 1, 1);
	p[7] = MODEL_get_point3d(1, 1, 0);

	t[0] = MODEL_get_triangle(p[0], p[1], p[2]);
	t[1] = MODEL_get_triangle(p[0], p[3], p[2]);
	t[2] = MODEL_get_triangle(p[0], p[4], p[3]);
	t[3] = MODEL_get_triangle(p[3], p[7], p[4]);
	t[4] = MODEL_get_triangle(p[3], p[2], p[7]);
	t[5] = MODEL_get_triangle(p[2], p[6], p[7]);
	t[6] = MODEL_get_triangle(p[1], p[5], p[2]);
	t[7] = MODEL_get_triangle(p[2], p[6], p[7]);
	t[8] = MODEL_get_triangle(p[7], p[6], p[5]);
	t[9] = MODEL_get_triangle(p[5], p[4], p[7]);
	t[10] = MODEL_get_triangle(p[5], p[1], p[0]);
	t[11] = MODEL_get_triangle(p[5], p[0], p[4]);

	Model *m = MODEL_get_model(12, t);

	return m;
}
