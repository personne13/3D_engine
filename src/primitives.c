#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <math.h>
#include "primitives.h"
#include "materials.h"

void PRIMITIVES_render_triangle(Triangle *t){
  if(t->shadow_map){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, t->shadow_map);
  }

  glBegin(GL_TRIANGLES);
    if(t->shadow_map){
      glTexCoord2f(0, 0);
    }
		glVertex3f(t->p[0].x, t->p[0].y, t->p[0].z);

    if(t->shadow_map){
      glTexCoord2f(1, 0);
    }
		glVertex3f(t->p[1].x, t->p[1].y, t->p[1].z);

    if(t->shadow_map){
      glTexCoord2f(0, 1);
    }
    glVertex3f(t->p[2].x, t->p[2].y, t->p[2].z);
	glEnd();

  if(t->shadow_map){
    glDisable(GL_TEXTURE_2D);
  }
}

Point3d PRIMITIVES_get_point3d(double x, double y, double z){
	Point3d p = {x, y, z};

	return p;
}

Triangle PRIMITIVES_get_triangle(Point3d p1, Point3d p2, Point3d p3){
	Triangle t;

	t.p[0] = p1;
	t.p[1] = p2;
	t.p[2] = p3;

  t.mat = MATERIAL_create_default();
  PRIMITIVES_compute_normal(&t);

	return t;
}

double PRIMITIVES_norm_vec(Point3d vec){
  return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

void PRIMITIVES_normalize_vec(Point3d *vec){
  double norm = PRIMITIVES_norm_vec(*vec);

  vec->x /= norm;
  vec->y /= norm;
  vec->z /= norm;
}

void PRIMITIVES_compute_normal(Triangle *t){
  Point3d ab;
  Point3d ac;

  PRIMITIVES_make_vec(&t->p[1], &t->p[0], &ab);
  PRIMITIVES_make_vec(&t->p[2], &t->p[0], &ac);

  PRIMITIVES_vectorial_product(&ab, &ac, &t->normal);

  PRIMITIVES_normalize_vec(&t->normal);
}

void PRIMITIVES_get_ray(Point3d *origin, Point3d *vec, Ray *out){
  out->origin = *origin;
  out->vec = *vec;
}

void PRIMITIVES_make_vec(Point3d *vec1, Point3d *vec2, Point3d *out){
  out->x = vec1->x - vec2->x;
  out->y = vec1->y - vec2->y;
  out->z = vec1->z - vec2->z;
}

double PRIMITIVES_distance(Point3d *p1, Point3d *p2){
  double res = (p1->x-p2->x) * (p1->x-p2->x) +
               (p1->y-p2->y) * (p1->y-p2->y) +
               (p1->z-p2->z) * (p1->z-p2->z);
  return sqrt(res);
}

double PRIMITIVES_distance_square(Point3d *p1, Point3d *p2){
  double res = (p1->x-p2->x) * (p1->x-p2->x) +
               (p1->y-p2->y) * (p1->y-p2->y) +
               (p1->z-p2->z) * (p1->z-p2->z);
  return res;
}

void PRIMITIVES_vectorial_product(Point3d *vect1, Point3d *vect2, Point3d *out){
  out->x = vect1->y * vect2->z - vect1->z * vect2->y;
  out->y = vect1->z * vect2->x - vect1->x * vect2->z;
  out->z = vect1->x * vect2->y - vect1->y * vect2->x;
}

double PRIMITIVES_scalar_product(Point3d *vect1, Point3d *vect2){
    return vect1->x * vect2->x + vect1->y * vect2->y + vect1->z * vect2->z;
}


void PRIMITIVES_sub_vector(Point3d *vect1, Point3d *vect2, Point3d *out){
  out->x = vect1->x - vect2->x;
  out->y = vect1->y - vect2->y;
  out->z = vect1->z - vect2->z;
}

void PRIMITIVES_add_vector(Point3d *vect1, Point3d *vect2, Point3d *out){
  out->x = vect1->x + vect2->x;
  out->y = vect1->y + vect2->y;
  out->z = vect1->z + vect2->z;
}

void PRIMITIVES_mul_vector(double a, Point3d *vect, Point3d *out){
  out->x = a * vect->x;
  out->y = a * vect->y;
  out->z = a * vect->z;
}

int PRIMITIVES_collision_ray_triangle(Ray ray, Triangle *t, Point3d *out){
  const double EPSILON = 0.0000001;
  Point3d vertex0 = t->p[0];
  Point3d vertex1 = t->p[1];
  Point3d vertex2 = t->p[2];
  Point3d edge1, edge2, h, s, q;
  double a,f,u,v;
  PRIMITIVES_sub_vector(&vertex1, &vertex0, &edge1);
  PRIMITIVES_sub_vector(&vertex2, &vertex0, &edge2);
  PRIMITIVES_vectorial_product(&ray.vec, &edge2, &h);
  a = PRIMITIVES_scalar_product(&edge1, &h);
  if (a > -EPSILON && a < EPSILON)
      return 0;
  f = 1/a;
  PRIMITIVES_sub_vector(&ray.origin, &vertex0, &s);
  u = f * PRIMITIVES_scalar_product(&s, &h);
  if (u < 0.0 || u > 1.0)
      return 0;
  PRIMITIVES_vectorial_product(&s, &edge1, &q);
  v = f * PRIMITIVES_scalar_product(&ray.vec, &q);
  if (v < 0.0 || u + v > 1.0)
      return 0;
  // At this stage we can compute dt to find out where the intersection point is on the line.
  float dt = f * PRIMITIVES_scalar_product(&edge2, &q);
  if (dt > EPSILON){ // ray intersection
    if(out){
      Point3d tmp;
      PRIMITIVES_mul_vector(dt, &ray.vec, &tmp);
      PRIMITIVES_add_vector(&ray.origin, &tmp, out);
    }
    return 1;
  }
  else // This means that there is a line intersection but not a ray intersection.
      return 0;
}

void PRIMITIVES_print_Point3d(Point3d point){
  printf("(x:%lf) (y:%lf) (z:%lf)\n", point.x, point.y, point.z);
}

void PRIMITIVES_print_Triangle(Triangle triangle){
  for(int i = 0; i < 3; i++){
    printf("edge %d : ", i);
    PRIMITIVES_print_Point3d(triangle.p[i]);
  }
}

void PRIMITIVES_print_Ray(Ray ray){
  printf("origin : ");
  PRIMITIVES_print_Point3d(ray.origin);
  printf("vector : ");
  PRIMITIVES_print_Point3d(ray.vec);
}
