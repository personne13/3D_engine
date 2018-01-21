#ifndef PRIMITIVES_H
#define PRIMITIVES_H

  #include <GL/glew.h>
  #include <GL/glu.h>

  typedef struct Point3d Point3d;
	struct Point3d{
		double x;
		double y;
		double z;
	};

	typedef struct Triangle Triangle;
	struct Triangle{
		Point3d p[3];
    GLuint shadow_map;
	};

  typedef struct Ray Ray;
  struct Ray{
    Point3d origin;
    Point3d vec;
  };

  Point3d PRIMITIVES_get_point3d(double x, double y, double z);
  Triangle PRIMITIVES_get_triangle(Point3d p1, Point3d p2, Point3d p3);
  void PRIMITIVES_render_triangle(Triangle *t);
  Ray PRIMITIVES_get_ray(Point3d origin, Point3d vec);
  Point3d PRIMITIVES_sub_vec(Point3d vec1, Point3d vec2);

  //Returns 1 if there is a collision between the given ray and the triangle.
  int PRIMITIVES_collision_ray_triangle(Ray ray, Triangle *triangle);

#endif
