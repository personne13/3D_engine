#ifndef MODEL_H
#define MODEL_H

	typedef struct Point3d Point3d;
	typedef struct Triangle Triangle;
	typedef struct Model Model;


	Point3d MODEL_get_point3d(double x, double y, double z);
	Triangle MODEL_get_triangle(Point3d p1, Point3d p2, Point3d p3);
	Model * MODEL_get_model(int nb_triangles, Triangle *t);
	void MODEL_free_model(Model *model);

	void MODEL_render_model(Model *model);

#endif
