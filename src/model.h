#ifndef MODEL_H
#define MODEL_H

	typedef struct Point3d Point3d;
	struct Point3d{
		double x;
		double y;
		double z;
	};

	typedef struct Triangle Triangle;
	struct Triangle{
		Point3d p[3];
	};

	typedef struct Model Model;
	struct Model{
		int nb_triangles;
		Triangle *triangle;
	};

	Point3d MODEL_get_point3d(double x, double y, double z);
	Triangle MODEL_get_triangle(Point3d p1, Point3d p2, Point3d p3);
	Model * MODEL_get_model(int nb_triangles, Triangle *t);//t stores all triangles of the model
	void MODEL_free_model(Model *model);

	void MODEL_render_model(Model *model);

	Model * MODEL_get_cube();

#endif
