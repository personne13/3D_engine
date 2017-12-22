#ifndef INPUT
#define INPUT

	enum {MOUSE, KEYBOARD, LEAVE, NONE};

  typedef struct Input Input;

  Input * INPUT_init();
  void INPUT_update(Input* in);
	int INPUT_isTriggered(Input *in, int input, int codeInput);
	//input : either MOUSE, KEYBOARD, LEAVE or NONE
	//codeInput : the SDL code associated to the input example : SDL_SCANCODE_A (beware of azerty, qwerty keyboard).

	void INPUT_getPositionCursor(const Input *in, int *x, int *y);
	//x and y have to be allocated.

  void INPUT_getRelativeCoords(Input *in, int *xrel, int *yrel);
  //xrel and yrel have to be allocated.

	void INPUT_free(Input *in);

#endif
