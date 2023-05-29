#include "Game.h"
#include "GameEnding1.h"

static SDL_Texture* g_texture_ending;
static SDL_Rect g_source_rectangle_ending;
static SDL_Rect g_destination_rectangle_ending;

static SDL_Texture* g_texture_finish;
static SDL_Rect g_source_rectangle_finish;
static SDL_Rect g_destination_rectangle_finish;

static SDL_Texture* g_texture_home;
static SDL_Rect g_source_rectangle_home;
static SDL_Rect g_destination_rectangle_home;

static SDL_Texture* g_texture_retry;
static SDL_Rect g_source_rectangle_retry;
static SDL_Rect g_destination_rectangle_retry;

Ending1::Ending1() {
	//For Texture
	SDL_Surface* temp_surface = IMG_Load("../src/gameclear.png");
	g_texture_ending = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);
	SDL_QueryTexture(g_texture_ending, NULL, NULL, &g_source_rectangle_ending.w, &g_source_rectangle_ending.h);
	g_source_rectangle_ending = { 0, 0, temp_surface->w, temp_surface->h };
	g_destination_rectangle_ending = { 0, 0, temp_surface->w, temp_surface->h };


	//home
	SDL_Surface* home_surface = IMG_Load("../src/home.png");
	g_texture_home = SDL_CreateTextureFromSurface(g_renderer, home_surface);
	SDL_FreeSurface(home_surface);
	SDL_QueryTexture(g_texture_home, NULL, NULL, &g_source_rectangle_home.w, &g_source_rectangle_home.h);
	g_source_rectangle_home = { 0, 0, 160, 80 };
	g_destination_rectangle_home = { 210, 485, 160, 80 };

	//retry
	SDL_Surface* retry_surface = IMG_Load("../src/retry.png");
	g_texture_retry = SDL_CreateTextureFromSurface(g_renderer, retry_surface);
	SDL_FreeSurface(retry_surface);
	SDL_QueryTexture(g_texture_retry, NULL, NULL, &g_source_rectangle_retry.w, &g_source_rectangle_retry.h);
	g_source_rectangle_retry = { 0, 0, 160, 80 };
	g_destination_rectangle_retry = { 440, 485, 160, 80 };
}

Ending1::~Ending1() {
	SDL_DestroyTexture(g_texture_ending);
	SDL_DestroyTexture(g_texture_finish);
	SDL_DestroyTexture(g_texture_home);
	SDL_DestroyTexture(g_texture_retry);
}

void Ending1::Update() {
}

void Ending1::Render() {
	SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
	SDL_RenderClear(g_renderer);

	SDL_RenderCopy(g_renderer, g_texture_ending, &g_source_rectangle_ending, &g_destination_rectangle_ending);
	SDL_RenderCopy(g_renderer, g_texture_finish, &g_source_rectangle_finish, &g_destination_rectangle_finish);
	SDL_RenderCopy(g_renderer, g_texture_home, &g_source_rectangle_home, &g_destination_rectangle_home);
	SDL_RenderCopy(g_renderer, g_texture_retry, &g_source_rectangle_retry, &g_destination_rectangle_retry);

	SDL_RenderPresent(g_renderer);

}

void Ending1::HandleEvents() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_MOUSEBUTTONDOWN:
			//if the mouse left button is pressed
			if (event.button.button == SDL_BUTTON_LEFT) {
				// Get the cursor's x position.
				int mouse_x = event.button.x;
				int mouse_y = event.button.y;

				if (
					mouse_x >= g_destination_rectangle_retry.x &&
					mouse_x <= g_destination_rectangle_retry.x + g_destination_rectangle_retry.w &&
					mouse_y >= g_destination_rectangle_retry.y &&
					mouse_y <= g_destination_rectangle_retry.y + g_destination_rectangle_retry.h)
					g_current_game_phase = PHASE_MODE1;

				else if (mouse_x >= g_destination_rectangle_home.x &&
					mouse_x <= g_destination_rectangle_home.x + g_destination_rectangle_home.w &&
					mouse_y >= g_destination_rectangle_home.y &&
					mouse_y <= g_destination_rectangle_home.y + g_destination_rectangle_home.h)
					g_current_game_phase = PHASE_INTRO;
			}
			break;

		default:
			break;
		}
	}
}