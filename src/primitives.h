#ifndef PRIMITIVES_H
#define PRIMITIVES_H

  #include <GL/glew.h>
  #include <GL/glu.h>

  #include "materials.h"

  #define MAX_TRIANGLES_SCENE 512

  typedef struct Point3d Point3d;
	struct Point3d{
		double x;
		double y;
		double z;
	};

	typedef struct Triangle Triangle;
	struct Triangle{
		Point3d p[3];
    Point3d normal;
    GLuint shadow_map;
    Material mat;
	};

  typedef struct Ray Ray;
  struct Ray{
    Point3d origin;
    Point3d vec;
  };

  Point3d PRIMITIVES_get_point3d(double x, double y, double z);
  Triangle PRIMITIVES_get_triangle(Point3d p1, Point3d p2, Point3d p3);
  double PRIMITIVES_norm_vec(Point3d vec);
  void PRIMITIVES_normalize_vec(Point3d *vec);
  void PRIMITIVES_compute_normal(Triangle *t);
  void PRIMITIVES_render_triangle(Triangle *t);
  void PRIMITIVES_get_ray(Point3d *origin, Point3d *vec, Ray *out);
  void PRIMITIVES_make_vec(Point3d *vec1, Point3d *vec2, Point3d *out);

  double PRIMITIVES_distance(Point3d *p1, Point3d *p2);
  double PRIMITIVES_distance_square(Point3d *p1, Point3d *p2);
  //Returns 1 if there is a collision between the given ray and the triangle.
  int PRIMITIVES_collision_ray_triangle(Ray ray, Triangle *triangle, Point3d *out);

  void PRIMITIVES_vectorial_product(Point3d *vect1, Point3d *vect2, Point3d *out);
  double PRIMITIVES_scalar_product(Point3d *vect1, Point3d *vect2);
  void PRIMITIVES_sub_vector(Point3d *vect1, Point3d *vect2, Point3d *out);
  void PRIMITIVES_add_vector(Point3d *vect1, Point3d *vect2, Point3d *out);
  void PRIMITIVES_mul_vector(double a, Point3d *vect, Point3d *out);

  //Debuging stuff
  void PRIMITIVES_print_Point3d(Point3d point);
  void PRIMITIVES_print_Triangle(Triangle triangle);
  void PRIMITIVES_print_Ray(Ray ray);



#endif
