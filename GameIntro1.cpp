#include "Game.h"
#include "GameIntro1.h"

using namespace std;

//배경
SDL_Surface* intro_bg_surface;
SDL_Texture* intro_bg_texture;
SDL_Rect intro_bg_rect;
SDL_Rect intro_bg_dest_rect;

// wall
SDL_Surface* intro_wall_surface;
SDL_Texture* intro_wall_texture;
SDL_Rect intro_wall_rect;
SDL_Rect intro_wall_dest_rect;

//인트로
static SDL_Texture* g_texture_intro;
static SDL_Rect g_source_rectangle_intro;
static SDL_Rect g_destination_rectangle_intro;

//플레이버튼
static SDL_Texture* g_texture_play;
static SDL_Rect g_source_rectangle_play;
static SDL_Rect g_destination_rectangle_play;

//뒤로가기버튼
SDL_Surface* introBack_surface;
SDL_Texture* introBack_texture;
SDL_Rect introBack_rect;
SDL_Rect introBack_dest_rect;

//레디
SDL_Surface* ready_surface;
SDL_Texture* ready_texture;
SDL_Rect ready_rect;
SDL_Rect ready_dest_rect;

//시작
SDL_Surface* start_surface;
SDL_Texture* start_texture;
SDL_Rect start_rect;
SDL_Rect start_dest_rect;


Intro1::Intro1() {
	//배경
	intro_bg_surface = IMG_Load("../src/bg_mode1.png");
	intro_bg_texture = SDL_CreateTextureFromSurface(g_renderer, intro_bg_surface); // GPU로 옮기기 
	SDL_FreeSurface(intro_bg_surface);
	intro_bg_rect = { 0, 1200, 800, 600 };
	intro_bg_dest_rect = { 0, 0, 800, 600 };

	// wall 이미지 로드
	intro_wall_surface = IMG_Load("../src/wall_mode1.png");
	intro_wall_texture = SDL_CreateTextureFromSurface(g_renderer, intro_wall_surface); // GPU로 옮기기 
	SDL_FreeSurface(intro_wall_surface);
	intro_wall_rect = { 0, 1900, 400, 600 };
	intro_wall_dest_rect = { 200, 0, 400, 600 };

	//인트로 tuto
	SDL_Surface* temp_surface = IMG_Load("../src/tutorial_mode1.png");
	g_texture_intro = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);
	SDL_QueryTexture(g_texture_intro, NULL, NULL, &g_source_rectangle_intro.w, &g_source_rectangle_intro.h);
	g_source_rectangle_intro = { 0, 0, 800, 600 };
	g_destination_rectangle_intro = { 95, 40, 620, 480 };

	//play
	SDL_Surface* play_surface = IMG_Load("../src/play.png");
	g_texture_play = SDL_CreateTextureFromSurface(g_renderer, play_surface);
	SDL_FreeSurface(play_surface);
	SDL_QueryTexture(g_texture_play, NULL, NULL, &g_source_rectangle_play.w, &g_source_rectangle_play.h);
	g_source_rectangle_play = { 0, 0, 160, 80 };
	g_destination_rectangle_play = { 320, 510, 160, 80 };

	//뒤로가기
	introBack_surface = IMG_Load("../src/back.png");
	introBack_texture = SDL_CreateTextureFromSurface(g_renderer, introBack_surface);
	SDL_FreeSurface(introBack_surface);
	introBack_rect = { 0, 0, introBack_surface->w,introBack_surface->h };
	introBack_dest_rect = { 10, 10, introBack_surface->w, introBack_surface->h };

	//레디
	ready_surface = IMG_Load("../src/ready.png");
	ready_texture = SDL_CreateTextureFromSurface(g_renderer, ready_surface);
	SDL_FreeSurface(ready_surface);
	ready_rect = { 0, 0, ready_surface->w,ready_surface->h };
	ready_dest_rect = { 10, 10, ready_surface->w, ready_surface->h };

	//스타트
	start_surface = IMG_Load("../src/start.png");
	start_texture = SDL_CreateTextureFromSurface(g_renderer, start_surface);
	SDL_FreeSurface(start_surface);
	start_rect = { 0, 0, start_surface->w,start_surface->h };
	start_dest_rect = { 10, 10, start_surface->w, start_surface->h };
}

Intro1::~Intro1() {
	SDL_DestroyTexture(intro_bg_texture);
	SDL_DestroyTexture(intro_wall_texture);
	SDL_DestroyTexture(g_texture_intro);
	SDL_DestroyTexture(g_texture_play);
	SDL_DestroyTexture(introBack_texture);
}

void Intro1::Update() {
}

void Intro1::Render() {
	SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
	SDL_RenderClear(g_renderer);

	SDL_RenderCopy(g_renderer, intro_bg_texture, &intro_bg_rect, &intro_bg_dest_rect);
	SDL_RenderCopy(g_renderer, intro_wall_texture, &intro_wall_rect, &intro_wall_dest_rect);
	SDL_RenderCopy(g_renderer, g_texture_intro, &g_source_rectangle_intro, &g_destination_rectangle_intro);
	SDL_RenderCopy(g_renderer, g_texture_play, &g_source_rectangle_play, &g_destination_rectangle_play);
	SDL_RenderCopy(g_renderer, introBack_texture, &introBack_rect, &introBack_dest_rect);

	SDL_RenderPresent(g_renderer);
}

void Intro1::HandleEvents() {
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

				if (mouse_x >= g_destination_rectangle_play.x &&
					mouse_x <= g_destination_rectangle_play.x + g_destination_rectangle_play.w &&
					mouse_y >= g_destination_rectangle_play.y &&
					mouse_y <= g_destination_rectangle_play.y + g_destination_rectangle_play.h) {

					g_current_game_phase = PHASE_MODE1;
					//SDL_DestroyTexture(g_texture_intro);
					//SDL_DestroyTexture(g_texture_play);

					//레디
					SDL_RenderClear(g_renderer);
					SDL_RenderCopy(g_renderer, intro_bg_texture, &intro_bg_rect, &intro_bg_dest_rect);
					SDL_RenderCopy(g_renderer, intro_wall_texture, &intro_wall_rect, &intro_wall_dest_rect);
					SDL_RenderCopy(g_renderer, ready_texture, &ready_rect, &ready_dest_rect);
					SDL_RenderPresent(g_renderer);
					SDL_Delay(2000);  // Delay for 3 seconds
					//SDL_DestroyTexture(ready_texture);

					//시작버튼
					SDL_RenderClear(g_renderer);
					SDL_RenderCopy(g_renderer, intro_bg_texture, &intro_bg_rect, &intro_bg_dest_rect);
					SDL_RenderCopy(g_renderer, intro_wall_texture, &intro_wall_rect, &intro_wall_dest_rect);
					SDL_RenderCopy(g_renderer, start_texture, &start_rect, &start_dest_rect);
					SDL_RenderPresent(g_renderer);
					SDL_Delay(1000);  // Delay for 1 seconds
					//SDL_DestroyTexture(start_texture);
				}
			}
			break;

		default:
			break;
		}
	}
}