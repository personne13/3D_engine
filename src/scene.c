#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include "window.h"
#include "scene.h"

void SCENE_clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SCENE_refresh(Window *window)
{
	SDL_GL_SwapWindow(window->w);
  glFlush();
}

void SCENE_mode_render(Window *win, int mode, int FOV)
{
    if(mode == RENDER_3D)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        gluPerspective(FOV, (double)win->width/win->height, 0.01, 1000);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }
    else if(mode == RENDER_2D)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glOrtho(0, win->width, 0, win->height, 0, 10000);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }
}
