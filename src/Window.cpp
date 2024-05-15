#include "Window.h"
#include <windows.h>

Window::Window()
{
	m_window = nullptr;
	m_renderer = nullptr;
	m_run = false;
	m_curve = nullptr;
	dt = 0;
}

void Window::run()
{
	_init();

	while (m_run) {
		uint64_t t1 = SDL_GetTicks64();
		_update();
		_draw();

		_update_events();
		uint64_t t2 = SDL_GetTicks64();
		dt = t2 - t1;
	}

	clear();
}

void Window::_init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		MessageBox(nullptr, L"Erreur lors de l'initialisation de SDL", L"Erreur", 0);
		exit(-1);
	}

	m_window = SDL_CreateWindow("Courbe bezier", 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		1280,
		920, 
		SDL_WINDOW_SHOWN);

	if (!m_window) {
		MessageBox(nullptr, L"Erreur lors de la création de la fenêtre", L"Erreur", 0);
		exit(-1);
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

	if (!m_renderer) {
		MessageBox(nullptr, L"Erreur lors de la création de la fenêtre", L"Erreur", 0);
		exit(-1);
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();

	if (!ImGui_ImplSDL2_InitForSDLRenderer(m_window, m_renderer)) {
		MessageBox(nullptr, L"Erreur lors de l'initialisation de ImGui", L"Erreur", 0);
		exit(-1);
	}
	if (!ImGui_ImplSDLRenderer2_Init(m_renderer)) {
		MessageBox(nullptr, L"Erreur lors de l'initialisation de ImGui", L"Erreur", 0);
		exit(-1);
	}

	if (TTF_Init() < 0) {
		MessageBox(nullptr, L"Erreur lors de l'initialisation de SDL_ttf", L"Erreur", 0);
		exit(-1);
	}

	m_curve = new BezierCurve();
	m_run = true;

	std::string basePath = SDL_GetBasePath();
	std::string finalPath = basePath + "icon.png";
	SDL_Surface* surface = IMG_Load(finalPath.c_str());

	if (!surface) {
		MessageBox(nullptr, L"L'icône de l'application n'a pas pu être trouvée", L"Erreur", 0);
		exit(-1);
	}

	SDL_SetWindowIcon(m_window, surface);
	SDL_FreeSurface(surface);
}

void Window::clear()
{
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	delete m_curve;

	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Window::_update_events()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (event.type == SDL_QUIT) {
			m_run = false;
		}
	}
}

void Window::_update()
{
	m_curve->update();
}

void Window::_draw()
{
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	m_curve->imgui_draw(dt);

	ImGui::Render();
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderClear(m_renderer);

	SDL_Surface* surf = IMG_Load("D:\\chicken.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surf);
	SDL_Rect r = { 0,0,32,32 };
	SDL_FRect r2 = { 150,150,64,64 };
	SDL_RenderCopyF(m_renderer, texture, &r, &r2);

	m_curve->draw(m_renderer);

	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(m_renderer);
}
