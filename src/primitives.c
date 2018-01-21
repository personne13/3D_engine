#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include "primitives.h"

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

Point3d PRIMITIVES_sub_vec(Point3d vec1, Point3d vec2){
  Point3d res;

  res.x = vec1.x - vec2.x;
  res.x = vec1.y - vec2.y;
  res.x = vec1.z - vec2.z;

  return res;
}

int PRIMITIVES_collision_ray_triangle(Ray ray, Triangle *triangle){
  return 0;
}
