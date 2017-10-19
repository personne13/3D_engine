#ifndef INIT_H
#define INIT_H
  #define MAX_NAME 128

  typedef struct Window Window;

  Window * WINDOW_create_window(char *name, int width, int height);
  void WINDOW_destroy(Window *window);

	int init_SDL();
	void quit_SDL();

#endif
