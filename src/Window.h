#ifndef WINDOW_H
#define WINDOW_H

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdlrenderer2.h"
#include "imgui/imgui_impl_sdl2.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "BezierCurve.h"

class Window
{
public:
	Window();

	void run();

private:
	void _init();
	void clear();

	void _update_events();
	void _update();
	void _draw();

	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	BezierCurve* m_curve;

	uint64_t dt;

	bool m_run;
};

#endif