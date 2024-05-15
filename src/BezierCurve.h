#ifndef BEZIER_CURVE_H
#define BEZIER_CURVE_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "imgui/imgui.h"
#include <string>
#include <vector>

class BezierCurve
{
public:
	BezierCurve();

	void update();
	void draw(SDL_Renderer* pRenderer);
	void imgui_draw(float dt);

private:
	std::vector<SDL_FPoint> m_points;
	std::vector<SDL_FPoint> m_curve;
	TTF_Font* m_font;

	SDL_FPoint lerp(SDL_FPoint p1, SDL_FPoint p2, float t);
	bool m_changed = false;

	int m_steps = 20;
};

#endif