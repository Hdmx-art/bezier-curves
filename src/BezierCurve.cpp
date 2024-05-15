#include "BezierCurve.h"
#include <windows.h>

BezierCurve::BezierCurve()
{
	m_points.push_back({ 50.f, 50.f });
	m_points.push_back({ 50.f, 200.f });
	m_points.push_back({ 200.f, 200.f });

	std::string basePath = SDL_GetBasePath();
	std::string path = basePath + "font.ttf";
	m_font = TTF_OpenFont(path.c_str(), 48);
	if (!m_font) {
		MessageBox(nullptr, L"La police d'écriture n'a pas pu être trouvée", L"Erreur", 0);
		exit(-1);
	}
}

void BezierCurve::update()
{
	m_curve.clear();
	float t = 0.f;
	for (int i = 0; i < m_steps; i++)
	{
		std::vector<std::vector<SDL_FPoint>> middle_points;
		middle_points.push_back(m_points);
		int step_count = 0;

		while (middle_points.back().size() > 1)
		{
			std::vector<SDL_FPoint> current_points;
			for (size_t i = 0; i < middle_points[step_count].size() - 1; i++)
			{
				current_points.push_back(lerp(middle_points[step_count][i], middle_points[step_count][i + 1], t));
			}

			middle_points.push_back(current_points);
			step_count++;
		}

		m_curve.push_back(middle_points.back().back());

		t += 1.f / m_steps;
	}
}

void BezierCurve::imgui_draw(float dt)
{
	m_changed = false;

	ImGui::SetNextWindowSize({380, 920});
	ImGui::SetNextWindowPos({1280-380, 0});
	ImGui::Begin("Parametres");

	ImGui::Text("Temps ecoule : %i MS \n", int(dt));

	ImGui::SliderInt("Nombre d'etapes", &m_steps, m_points.size() - 1, 20);

	for (size_t i = 0; i < m_points.size(); i++)
	{
		std::string name = "";
		name += char(65 + i);
		ImGui::Text(name.c_str());

		std::string x = "x_";
		x += char(65 + i);

		std::string y = "y_";
		y += char(65 + i);
		ImGui::SliderFloat(x.c_str(), &m_points[i].x, 0.f, 1280.f - 20.f - 380.f);
		ImGui::SliderFloat(y.c_str(), &m_points[i].y, 0.f, 890.f);
	}

	if (ImGui::Button("Ajouter un point"))
	{
		m_points.push_back({ 640, 460 });
	}
	if (ImGui::Button("Supprimer le dernier point"))
	{
		m_points.erase(m_points.begin() + m_points.size() - 1);
	}

	ImGui::End();
}

void BezierCurve::draw(SDL_Renderer* pRenderer)
{
	for (const auto& point : m_points)
	{
		SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
		SDL_RenderDrawPointF(pRenderer, point.x, point.y);
	}

	for (size_t i = 0; i < m_curve.size() - 1; i++)
	{
		SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
		SDL_RenderDrawLineF(pRenderer, m_curve[i].x, m_curve[i].y, m_curve[i + 1].x, m_curve[i + 1].y);
	}

	SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
	SDL_RenderDrawLineF(pRenderer, m_curve.back().x, m_curve.back().y, m_points.back().x, m_points.back().y);

	size_t i = 0;
	for (const auto& point : m_points)
	{
		std::string name = "";
		name += char(65 + i);
		SDL_Surface* surf = TTF_RenderText_Solid(m_font, name.c_str(), { 255, 255, 255, 255 });
		SDL_Texture* text = SDL_CreateTextureFromSurface(pRenderer, surf);

		SDL_FRect r = { point.x, point.y, 16.f, 32.f };
		SDL_RenderCopyF(pRenderer, text, nullptr, &r);

		SDL_FreeSurface(surf);
		SDL_DestroyTexture(text);

		i++;
	}
}

SDL_FPoint BezierCurve::lerp(SDL_FPoint p1, SDL_FPoint p2, float t)
{
	SDL_FPoint point{};
	point.x = p1.x + t * (p2.x - p1.x);
	point.y = p1.y + t * (p2.y - p1.y);
	return point;
}
