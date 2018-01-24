#ifndef MODEL_H
#define MODEL_H

  #include "primitives.h"

	typedef struct Model Model;
	struct Model{
		int nb_triangles;
		Triangle *triangle;
	};

	Model * MODEL_get_model(int nb_triangles, Triangle *t);//t stores all triangles of the model
  void MODEL_translate_model(Model *model, double x, double y, double z);
	void MODEL_free_model(Model *model);

	void MODEL_render_model(Model *model);

	Model * MODEL_get_cube(double x, double y, double z);

#endif
