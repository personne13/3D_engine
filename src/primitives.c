#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include "primitives.h"

int magic(Point3d vect1,Point3d vect2,Point3d vect3);
int is_in(Triangle *t, Point3d m);


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
      glTexCoord2f(0, 1);
    }
		glVertex3f(t->p[1].x, t->p[1].y, t->p[1].z);

    if(t->shadow_map){
      glTexCoord2f(1, 0);
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

  t.shadow_map = 0;

	return t;
}

Ray PRIMITIVES_get_ray(Point3d origin, Point3d vec){
  Ray res;

  res.origin = origin;
  res.vec = vec;

  return res;
}

Point3d PRIMITIVES_make_vec(Point3d vec1, Point3d vec2){
  Point3d res;

  res.x = vec1.x - vec2.x;
  res.x = vec1.y - vec2.y;
  res.x = vec1.z - vec2.z;

  return res;
}

Point3d PRIMITIVES_vectorial_product(Point3d vect1, Point3d vect2){
  Point3d res;
  res.x = vect1.y * vect2.z - vect1.z * vect2.y;
  res.y = vect1.z * vect2.x - vect1.x * vect2.z;
  res.z = vect1.x * vect2.y - vect1.y * vect2.x;
  return res;
}

double PRIMITIVES_scalar_product(Point3d vect1, Point3d vect2){
    return vect1.x*vect2.x+vect1.y*vect2.y+vect1.z*vect2.z;
}

int magic(Point3d vect1,Point3d vect2,Point3d vect3){
  Point3d v1 = PRIMITIVES_vectorial_product(vect1,vect2);
  Point3d v2 = PRIMITIVES_vectorial_product(vect2,vect3);
  return PRIMITIVES_scalar_product(v1,v2) >= 0;
}

int is_in(Triangle *t, Point3d m){
  Point3d ab = PRIMITIVES_make_vec(t->p[0],t->p[1]);
  Point3d ba = PRIMITIVES_make_vec(t->p[1],t->p[0]);
  Point3d bc = PRIMITIVES_make_vec(t->p[1],t->p[2]);
  Point3d ac = PRIMITIVES_make_vec(t->p[0],t->p[2]);
  Point3d ca = PRIMITIVES_make_vec(t->p[2],t->p[0]);
  Point3d cb = PRIMITIVES_make_vec(t->p[2],t->p[1]);
  Point3d am = PRIMITIVES_make_vec(t->p[0],m);
  Point3d bm = PRIMITIVES_make_vec(t->p[1],m);
  Point3d cm = PRIMITIVES_make_vec(t->p[2],m);
  int b1,b2,b3;
  b1 = magic(ab,am,ac);
  b2 = magic(ba,bm,bc);
  b3 = magic(ca,cm,cb);
  return  b1 && b2 && b3;
}

int PRIMITIVES_collision_ray_triangle(Ray ray, Triangle *t){
  double a,b,c,d;//equation de plan ax+by+cz+d=0
  Point3d v1 = PRIMITIVES_make_vec(t->p[0],t->p[1]);
  Point3d v2 = PRIMITIVES_make_vec(t->p[1],t->p[2]);
  Point3d pv = PRIMITIVES_vectorial_product(v1,v2);//vect normal au plan
  a = pv.x;
  b = pv.y;
  c = pv.z;
  d = -( a*t->p[0].x + b*t->p[1].y + c*t->p[0].z);
  double denominator = a*ray.vec.x + b*ray.vec.y + c*ray.vec.z;
  if (denominator == 0){
    return 0;
  }
  else
  {
    double t0 = -(d+a*ray.origin.x+b*ray.origin.y+c*ray.origin.z)/denominator;
    Point3d m = PRIMITIVES_get_point3d(ray.origin.x+t0*ray.vec.x,ray.origin.y+t0*ray.vec.y,ray.origin.z+t0*ray.vec.z);
    return is_in(t,m);
  }
}
