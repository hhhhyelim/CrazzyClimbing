#include "Game.h"
#include "GameMode1.h"
#include <vector>
#include <cstdlib>

using namespace std;

// bg
SDL_Surface* bg_surface;
SDL_Texture* bg_texture;
SDL_Rect bg_rect;
SDL_Rect bg_dest_rect;

// wall
SDL_Surface* wall_surface;
SDL_Texture* wall_texture;
SDL_Rect wall_rect;
SDL_Rect wall_dest_rect;

// time
SDL_Surface* time_surface;
SDL_Texture* time_texture;
SDL_Rect time_rect;
SDL_Rect time_dest_rect;


// hold
vector<string> hold_paths = { "../src/downHold.png", "../src/leftHold.png", "../src/rightHold.png", "../src/upHold.png" };
vector<SDL_Surface*> hold_surfaces;
vector<SDL_Texture*> hold_textures;
vector<SDL_Rect> hold_rects;
vector<SDL_Rect> hold_dest_rects;
int leftHoldY = 475;
int rightHoldY = 400;

// leftUser
SDL_Surface* leftUser_surface;
SDL_Texture* leftUser_texture;
SDL_Rect leftUser_rect;
SDL_Rect leftUser_dest_rect;

// rightUser
SDL_Surface* rightUser_surface;
SDL_Texture* rightUser_texture;
SDL_Rect rightUser_rect;
SDL_Rect rightUser_dest_rect;

int hold[100]; // 1:down 2:left 3: right 4: up
int f_state; // 1:down 2:left 3: right 4: up
bool isLeftUser = true;
string nextHoldPath = "";

Mode1::Mode1() {
	g_flag_running = true;
	cur_i = 1;
	// 배경 이미지 로드
	bg_surface = IMG_Load("../src/bg_mode1.png");
	bg_texture = SDL_CreateTextureFromSurface(g_renderer, bg_surface); // GPU로 옮기기 
	SDL_FreeSurface(bg_surface);
	bg_rect = {0, 1900, 800, 600};
	bg_dest_rect = {0, 0, 800, 600};

	// wall 이미지 로드
	wall_surface = IMG_Load("../src/wall_mode1.png");
	wall_texture = SDL_CreateTextureFromSurface(g_renderer, wall_surface); 
	SDL_FreeSurface(wall_surface);
	wall_rect = { 0, 1900, 400, 600 };
	wall_dest_rect = { 200, 0, 400, 600	};

	// time 이미지 로드
	time_surface = IMG_Load("../src/time.png");
	time_texture = SDL_CreateTextureFromSurface(g_renderer, time_surface); 
	SDL_FreeSurface(time_surface);
	time_rect = { 0, 0, time_surface->w, time_surface->h };
	time_dest_rect = { 630, 10, time_surface->w, time_surface->h };


	// hold 이미지 로드
	srand((unsigned)time(NULL)); // srand는 한 번만 호출해야 합니다.
	for (int i = 0; i < 40; i++) {
		int random_hold_idx = rand() % 4; // 0~3 중 하나의 인덱스를 무작위로 선택
		string hold_path = hold_paths[random_hold_idx];
		SDL_Surface* hold_surface = IMG_Load(hold_path.c_str());
		SDL_Texture* hold_texture = SDL_CreateTextureFromSurface(g_renderer, hold_surface);
		SDL_FreeSurface(hold_surface);
		hold_surfaces.push_back(hold_surface);
		hold_textures.push_back(hold_texture);
		SDL_Rect hold_rect = { 0, 0, hold_surface->w, hold_surface->h };
		hold_rects.push_back(hold_rect);
	
		
		if (i % 2 == 0) {
			SDL_Rect hold_dest_rect = { 270, leftHoldY , hold_surface->w, hold_surface->h };
			hold_dest_rects.push_back(hold_dest_rect);
			leftHoldY -= 150;
		}
		else {
			SDL_Rect hold_dest_rect = { 470, rightHoldY , hold_surface->w, hold_surface->h };
			hold_dest_rects.push_back(hold_dest_rect);
			rightHoldY -= 150;
		}

		if (hold_path == "../src/downHold.png") {
			nextHoldPath = hold_path;
			hold[i] = 1;
		}
		else if (hold_path == "../src/leftHold.png") {
			nextHoldPath = hold_path;
			hold[i] = 2;
		}
		else if (hold_path == "../src/rightHold.png") {
			nextHoldPath = hold_path;
			hold[i] = 3;
		}
		else if (hold_path == "../src/upHold.png") {
			nextHoldPath = hold_path;
			hold[i] = 4;
		}
		// printf("%d\n", hold);
	}

	// leftUser
	leftUser_surface = IMG_Load("../src/leftUser.png");
	leftUser_texture = SDL_CreateTextureFromSurface(g_renderer, leftUser_surface);
	SDL_FreeSurface(leftUser_surface);
	leftUser_rect = { 0, 0, leftUser_surface->w, leftUser_surface->h };
	leftUser_dest_rect = { 240, 427, leftUser_surface->w, leftUser_surface->h };

	// rightUser
	rightUser_surface = IMG_Load("../src/rightUser.png");
	rightUser_texture = SDL_CreateTextureFromSurface(g_renderer, rightUser_surface);
	SDL_FreeSurface(rightUser_surface);
	rightUser_rect = { 0, 0, rightUser_surface->w, rightUser_surface->h };
	rightUser_dest_rect = { 440, 427, rightUser_surface->w, rightUser_surface->h };
	

}

Mode1::~Mode1() {
	SDL_DestroyTexture(bg_texture);
	SDL_DestroyTexture(wall_texture);
	SDL_DestroyTexture(time_texture);
	
	for (SDL_Texture* texture : hold_textures) {
		SDL_DestroyTexture(texture);
	}
	SDL_DestroyTexture(leftUser_texture);
	SDL_DestroyTexture(rightUser_texture);

}

void Mode1::Update()
{
	if (checkHold() == true) {
		userMove();
		holdMove();
		cur_i++;
	}
	else {

	}
}

void Mode1::Render() {
	SDL_RenderClear(g_renderer);

	SDL_RenderCopy(g_renderer, bg_texture, &bg_rect, &bg_dest_rect);
	SDL_RenderCopy(g_renderer, wall_texture, &wall_rect, &wall_dest_rect);
	SDL_RenderCopy(g_renderer, time_texture, &time_rect, &time_dest_rect);

	SDL_RenderCopy(g_renderer, leftUser_texture, &leftUser_rect, &leftUser_dest_rect);
	// SDL_RenderCopy(g_renderer, rightUser_texture, &rightUser_rect, &rightUser_dest_rect);
	
	for (size_t i = 0; i < hold_textures.size(); i++) {
		SDL_RenderCopy(g_renderer, hold_textures[i], &hold_rects[i], &hold_dest_rects[i]);
	}


	SDL_RenderPresent(g_renderer);
}

// bg, wall 아래로 내려가고 user는 좌우 움직이기
void Mode1::userMove() {
	wall_dest_rect.y += 30;
	bg_dest_rect.y += 5;
	if (isLeftUser) {
		leftUser_dest_rect.x += 200;
		rightUser_dest_rect.x -= 200;
	}
	else {
		leftUser_dest_rect.x -= 200;
		rightUser_dest_rect.x += 200;
	}
	SDL_Surface* tempSurface = leftUser_surface;
	leftUser_surface = rightUser_surface;
	rightUser_surface = tempSurface;

	SDL_Texture* tempTexture = leftUser_texture;
	leftUser_texture = rightUser_texture;
	rightUser_texture = tempTexture;

	isLeftUser = !isLeftUser;
}

void Mode1::holdMove() {
	for (SDL_Rect& hold_dest_rect : hold_dest_rects) {
		hold_dest_rect.y += 75;
	}
}


bool Mode1::checkHold() {
	if (hold[cur_i] == 1 && f_state == 1) {
		return true;
	}
	else if (hold[cur_i] == 2 && f_state == 2) {
		return true;
	}
	else if (hold[cur_i] == 3 && f_state == 3) {
		return true;
	}
	else if (hold[cur_i] == 4 && f_state == 4) {
		return true;
	}
	else {
		return false;
	}
	
}

void Mode1::HandleEvents() {
	
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {                
		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN: // 1:down 2:left 3: right 4: up
			if (event.key.keysym.sym == SDLK_LEFT) {
				// 다음 돌이 leftHold일 때 hold, wall, bg 내려가기 
				// 아닌경우 stun효과?
				f_state = 2;
				
				
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				// 다음 돌이 rightHold일 때 hold, wall, bg 내려가기
				// 아닌경우 stun효과?
				f_state = 3;
				
			}
			else if (event.key.keysym.sym == SDLK_UP) {
				// 다음 돌이 upHold일 때 hold, wall, bg 내려가기
				// 아닌경우 stun효과?
				f_state = 4;
				
			}
			else if (event.key.keysym.sym == SDLK_DOWN) {
				// 다음 돌이 downHold일 때 hold, wall, bg 내려가기
				// 아닌경우 stun효과?
				f_state = 1;
				
			}
			break;
		
		default:
			break;
		}
	}

}