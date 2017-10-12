#ifndef INPUT
#define INPUT

    typedef struct Input Input;
    struct Input
    {
        char keyboard[SDL_NUM_SCANCODES];
        char mouse[100];
        int leave;
        int mouseX, mouseY;
    };

    void INPUT_init(Input *in);
    void INPUT_update(Input* in);

#endif
