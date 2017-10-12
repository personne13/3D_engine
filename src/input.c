#include <SDL.h>
#include "input.h"

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
		  event.motion.xrel = 0;
		  event.motion.yrel = 0;
		}

  	if(in->keyboard[SDL_SCANCODE_LALT] && in->keyboard[SDL_SCANCODE_F4])
  	  in->leave = 1;

  	event.type = -1;
  }
}

void INPUT_init(Input *in)
{
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
}
