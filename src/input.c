#include <SDL.h>
#include "input.h"

#define NB_MOUSE_BUTTONS 10

struct Input
{
		char keyboard[SDL_NUM_SCANCODES];
		char mouse[NB_MOUSE_BUTTONS];
		int leave;
		int mouseX, mouseY;
    int mouseXRel, mouseYRel;
};

void INPUT_update(Input* in)
{
	static SDL_Event event;

  while(SDL_PollEvent(&event)){
		switch (event.type){
			case SDL_KEYDOWN:
				if(event.key.keysym.scancode < SDL_NUM_SCANCODES && event.key.repeat == 0)
					in->keyboard[event.key.keysym.scancode] = 1;
					break;
			case SDL_KEYUP:
				if(event.key.keysym.scancode < SDL_NUM_SCANCODES && event.key.repeat == 0)
					in->keyboard[event.key.keysym.scancode] = 0;
					break;
			case SDL_MOUSEMOTION:
				in->mouseX = event.motion.x;
				in->mouseY = event.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if(event.button.button < 8)
				in->mouse[event.button.button] = 1;
				break;
			case SDL_MOUSEBUTTONUP:
				if(event.button.button < 8)
				in->mouse[event.button.button] = 0;
				break;
			case SDL_WINDOWEVENT:
				if(event.window.event == SDL_WINDOWEVENT_CLOSE)
				in->leave = 1;
				break;
			default:
				break;
			}

		if(event.type == SDL_MOUSEMOTION){
		  in->mouseXRel = event.motion.xrel;
		  in->mouseYRel = event.motion.yrel;
		}

  	if(in->keyboard[SDL_SCANCODE_LALT] && in->keyboard[SDL_SCANCODE_F4])
  	  in->leave = 1;

  	event.type = -1;
  }
}

Input * INPUT_init()
{
	Input *in = malloc(sizeof(Input));

	if(!in){
		fprintf(stderr, "Error allocating input memory\n");
		return NULL;
	}

  int i;

  in->leave = 0;
  in->mouseX = 0;
  in->mouseY = 0;

  for(i = 0; i < 8; i++)
      in->mouse[i] = 0;

  for(i = 0; i < SDL_NUM_SCANCODES; i++)
  {
      in->keyboard[i] = 0;
  }

	return in;
}

int INPUT_isTriggered(Input *in, int input, int codeInput){
	switch(input){
		case KEYBOARD:
			if(codeInput < 0 || codeInput >= SDL_NUM_SCANCODES)
				return 0;

			return in->keyboard[codeInput];
			break;
		case MOUSE:
			if(codeInput < 0 || codeInput >= NB_MOUSE_BUTTONS)
				return 0;

			return in->mouse[codeInput];
			break;
		case LEAVE:
			return in->leave;
			break;
		default:
			break;
	}

	return 0;
}

void INPUT_getRelativeCoords(Input *in, int *xrel, int *yrel){
  (*xrel) = in->mouseXRel;
	(*yrel) = in->mouseYRel;

  in->mouseXRel = 0;
  in->mouseYRel = 0;
}

void INPUT_getPositionCursor(const Input *in, int *x, int *y){
	(*x) = in->mouseX;
	(*y) = in->mouseY;
}

void INPUT_free(Input *in){
	free(in);
}
