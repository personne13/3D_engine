#ifndef INIT_H
#define INIT_H
  #define MAX_NAME 128

  typedef struct Window Window;
  struct Window{
    int width;
    int height;
    char name[MAX_NAME];
    SDL_Window *w;
    SDL_GLContext context;
  };

  Window * WINDOW_create_window(char *name, int width, int height);
  void WINDOW_destroy(Window *window);

#endif
