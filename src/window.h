#ifndef INIT_H
#define INIT_H
  #define MAX_NAME 128

  typedef struct Window Window;
	struct Window{
		int width;
		int height;
    int fov;
		char name[MAX_NAME];
		SDL_Window *w;
		SDL_GLContext context;
	};

  Window * WINDOW_create_window(char *name, int width, int height, int fov);
  void WINDOW_destroy(Window *window);

	int init_SDL();
	void quit_SDL();
	int init_glew();

#endif
