#ifndef SCENE_H
#define SCENE_H

	enum{RENDER_2D, RENDER_3D};

	void SCENE_clear();
	void SCENE_mode_render(Window *win, int mode);
	void SCENE_refresh(Window *window);

#endif
