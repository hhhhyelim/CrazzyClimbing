#include "Game.h"
#include "GameMode1.h"
#include <vector>
#include <cstdlib>
#include <chrono>

using namespace std;

// ��� �̹��� ũ��
const int BACKGROUND_WIDTH = 800;
const int BACKGROUND_HEIGHT = 1800;

const int WALL_WIDTH = 400;
const int WALL_HEIGHT = 5400;


// bg
SDL_Surface* bg_surface;
SDL_Texture* bg_texture;
SDL_Rect bg_rect;
SDL_Rect bg_dest_rect;
int backgroundY; // ��� �̹����� Y ��ǥ

// wall
SDL_Surface* wall_surface;
SDL_Texture* wall_texture;
SDL_Rect wall_rect;
SDL_Rect wall_dest_rect;
int wallY;

// timeBg
SDL_Surface* timeBg_surface;
SDL_Texture* timeBg_texture;
SDL_Rect timeBg_rect;
SDL_Rect timeBg_dest_rect;

// time
TTF_Font* font;
SDL_Surface* time_surface;
SDL_Texture* time_texture;
SDL_Rect time_rect;

/*
// ���� ���� �ð�
std::chrono::steady_clock::time_point startTime;
// ���� ����
const int MAX_HOLDS = 40; // �ִ� Ȧ�� ����
int completedHolds = 0;   // �Ϸ�� Ȧ�� ����// hold
*/

// hold
vector<string> hold_paths = { "../src/downHold.png", "../src/leftHold.png", "../src/rightHold.png", "../src/upHold.png" };
vector<SDL_Surface*> hold_surfaces;
vector<SDL_Texture*> hold_textures;
vector<SDL_Rect> hold_rects;
vector<SDL_Rect> hold_dest_rects;
int leftHoldY = 475; //���� 150 ���̿��� 75 ���� ����� 200 ���̴ϱ� 100���̳���
int rightHoldY = 375;

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

//�ڷΰ����ư
SDL_Surface* back_surface;
SDL_Texture* back_texture;
SDL_Rect back_rect;
SDL_Rect back_dest_rect;

// stun ȿ��
SDL_Surface* stun_surface;
SDL_Texture* stun_texture;
SDL_Rect stun_rect;
SDL_Rect stun_dest_rect;
bool showStunEffect = false;
std::chrono::steady_clock::time_point stunEffectStartTime;

int hold[100]; // 1:down 2:left 3: right 4: up
int f_state; // 1:down 2:left 3: right 4: up
bool isLeftUser = true;
string nextHoldPath = "";
int prevHoldIndex = -1;
// Uint32 startTime = 0;


Mode1::Mode1() {
	g_flag_running = true;
	cur_i = 1;
	prevHold = 0;

	// ��� �̹��� �ε�
	
	bg_surface = IMG_Load("../src/bg_mode1.png");
	bg_texture = SDL_CreateTextureFromSurface(g_renderer, bg_surface); // GPU�� �ű�� 
	SDL_FreeSurface(bg_surface);
	backgroundY = -1200; 	// ��� �̹��� �ʱ� ��ġ ���� (ȭ�� ����)
	
	
	// wall �̹��� �ε�
	wall_surface = IMG_Load("../src/wall_mode1.png");
	wall_texture = SDL_CreateTextureFromSurface(g_renderer, wall_surface); 
	SDL_FreeSurface(wall_surface);
	wallY = -4800;

	// timeBg
	timeBg_surface = IMG_Load("../src/timeBg.png");
	timeBg_texture = SDL_CreateTextureFromSurface(g_renderer, timeBg_surface);
	SDL_FreeSurface(timeBg_surface);
	timeBg_rect = { 0, 0, timeBg_surface->w, timeBg_surface->h };
	timeBg_dest_rect = { 30, 20, timeBg_surface->w, timeBg_surface->h };

	//time
	/*
	startTime = std::chrono::steady_clock::now();
	font = TTF_OpenFont("../src/DungGeunMo.ttf", 17);
	SDL_Color black = { 0, 0, 0, 0 }; // �������� ����
	time_surface = TTF_RenderText_Solid(font, gameTimeString.c_str(), black);
	time_texture= SDL_CreateTextureFromSurface(g_renderer, time_surface);
	*/
	
	
	// hold �̹��� �ε�
	srand((unsigned)time(NULL)); // srand�� �� ���� ȣ���ؾ� �մϴ�.
	for (int i = 0; i < 40; i++) {
		// int random_hold_idx = rand() % 4; // 0~3 �� �ϳ��� �ε����� �������� ����
		int random_hold_idx;
		do {
			random_hold_idx = rand() % 4; // 0~3 �� �ϳ��� �ε����� �������� ����
		} while (random_hold_idx == prevHoldIndex); // ���� �ε����� ���� �ε����� ������ ���ο� �ε����� ��� �����մϴ�
		prevHoldIndex = random_hold_idx;
		string hold_path = hold_paths[random_hold_idx];
		SDL_Surface* hold_surface = IMG_Load(hold_path.c_str());
		SDL_Texture* hold_texture = SDL_CreateTextureFromSurface(g_renderer, hold_surface);
		SDL_FreeSurface(hold_surface);
		hold_surfaces.push_back(hold_surface);
		hold_textures.push_back(hold_texture);
		SDL_Rect hold_rect = { 0, 0, hold_surface->w, hold_surface->h };
		hold_rects.push_back(hold_rect);
	
		
		if (i % 2 == 0) {
			SDL_Rect hold_dest_rect = { 300, leftHoldY , hold_surface->w, hold_surface->h }; // ���� 270
			hold_dest_rects.push_back(hold_dest_rect);
			leftHoldY -= 200;
		}
		else {
			SDL_Rect hold_dest_rect = { 430, rightHoldY , hold_surface->w, hold_surface->h }; // ���� 470
			hold_dest_rects.push_back(hold_dest_rect);
			rightHoldY -= 200;
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
	leftUser_dest_rect = { 280, 427, leftUser_surface->w, leftUser_surface->h }; // ���� 240

	// rightUser
	rightUser_surface = IMG_Load("../src/rightUser.png");
	rightUser_texture = SDL_CreateTextureFromSurface(g_renderer, rightUser_surface);
	SDL_FreeSurface(rightUser_surface);
	rightUser_rect = { 0, 0, rightUser_surface->w, rightUser_surface->h };
	rightUser_dest_rect = { 410, 427, rightUser_surface->w, rightUser_surface->h }; // ���� 440

	//�ڷΰ���
	back_surface = IMG_Load("../src/back.png");
	back_texture = SDL_CreateTextureFromSurface(g_renderer, back_surface);
	SDL_FreeSurface(back_surface);
	back_rect = { 0, 0, back_surface->w, back_surface->h };
	back_dest_rect = { 10, 10, back_surface->w, back_surface->h };

	


}

Mode1::~Mode1() {
	
	SDL_DestroyTexture(bg_texture);
	SDL_DestroyTexture(wall_texture);
	//SDL_DestroyTexture(time_texture);
	SDL_DestroyTexture(timeBg_texture);
	
	for (SDL_Texture* texture : hold_textures) {
		SDL_DestroyTexture(texture);
	}
	SDL_DestroyTexture(leftUser_texture);
	SDL_DestroyTexture(rightUser_texture);
	SDL_DestroyTexture(back_texture);
	
}

void Mode1::Update()
{
	if (showStunEffect) {
		// stun ȿ���� ������ ������ �ش� ������ �������� ����
		// �ٸ� ó�� ����
		return;
	}

	if (checkHold()==true) {
		userMove();
		holdMove();
		cur_i++;
		
		backgroundY += 30;
		wallY += 118;

	}
	

}

void Mode1::Render() {
	SDL_RenderClear(g_renderer);

	//����̾�����
	SDL_Rect backgroundRect = { 0, backgroundY, BACKGROUND_WIDTH, BACKGROUND_HEIGHT };
	SDL_RenderCopy(g_renderer, bg_texture, NULL, &backgroundRect);

	//���̾�����
	SDL_Rect wallRect = { 200, wallY, WALL_WIDTH, WALL_HEIGHT };
	SDL_RenderCopy(g_renderer, wall_texture, NULL, &wallRect);
	
	
	//SDL_RenderCopy(g_renderer, time_texture, NULL, &time_rect);
	

	for (size_t i = 0; i < hold_textures.size(); i++) {
		SDL_RenderCopy(g_renderer, hold_textures[i], &hold_rects[i], &hold_dest_rects[i]);
	}
	SDL_RenderCopy(g_renderer, leftUser_texture, &leftUser_rect, &leftUser_dest_rect);
	SDL_RenderCopy(g_renderer, timeBg_texture, &timeBg_rect, &timeBg_dest_rect);
	
	// stun ȿ��
	
	if (showStunEffect)
	{
		stun_effect();
	}
	showStunEffect = false;

	SDL_RenderPresent(g_renderer);
}

void Mode1::stun_effect() {

	// stun ȿ��
	stun_surface = IMG_Load("../src/stun.png");
	stun_texture = SDL_CreateTextureFromSurface(g_renderer, stun_surface);
	SDL_FreeSurface(stun_surface);
	stun_rect = { 0, 0, stun_surface->w, stun_surface->h };
	stun_dest_rect = { 0, 0, stun_surface->w, stun_surface->h };

	/*
	std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedTime = currentTime - stunEffectStartTime;
	if (elapsedTime.count() < 1.0) {
		SDL_RenderCopy(g_renderer, stun_texture, &stun_rect, &stun_dest_rect);
	}
	else {
		showStunEffect = false;
	}
	*/
}

// bg, wall �Ʒ��� �������� user�� �¿� �����̱�
void Mode1::userMove() {
	//bg_dest_rect.y += 5;
	//wall_dest_rect.y += 30;
	

	if (isLeftUser) {
		leftUser_dest_rect.x += 150;
		rightUser_dest_rect.x -= 150;
	}
	else {
		leftUser_dest_rect.x -= 150;
		rightUser_dest_rect.x += 150;
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
		hold_dest_rect.y += 100;
	}
	bool allHoldsOffScreen = true;
	for(const SDL_Rect& hold_dest_rect : hold_dest_rects) {
		if (hold_dest_rect.y < 600) {
			allHoldsOffScreen = false;
			break;
		}
	}

	if (allHoldsOffScreen) {
		SDL_Delay(2000);
		g_current_game_phase = PHASE_ENDING1;
	}
}


bool Mode1::checkHold() {


	if (hold[cur_i] == f_state) {

		prevHold = f_state;
		return true;
	}
	else {
		showStunEffect = true;
		prevHold = 0; // hold�� ��ġ���� ������ prevHold ���� �ʱ�ȭ�մϴ�.
	}
	return false;

}

void Mode1::HandleEvents() {
	
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {                
		case SDL_QUIT:
			g_flag_running = false;
			break;


		case SDL_KEYDOWN: // 1:down 2:left 3: right 4: up
			/*
			if (showStunEffect) {
				break; // stun ȿ���� Ȱ��ȭ�Ǿ� ������ �ٸ� Ű �Է��� �����ϰ� ����
			}
			*/

			if (event.key.keysym.sym == SDLK_LEFT) {
				// ���� ���� leftHold�� �� hold, wall, bg �������� 
				// �ƴѰ�� stunȿ��?
				f_state = 2;
				// ��� �̹��� �̵�
				
				
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				// ���� ���� rightHold�� �� hold, wall, bg ��������
				// �ƴѰ�� stunȿ��?
				f_state = 3;
				// ��� �̹��� �̵�
				
				
			}
			else if (event.key.keysym.sym == SDLK_UP) {
				// ���� ���� upHold�� �� hold, wall, bg ��������
				// �ƴѰ�� stunȿ��?
				f_state = 4;
				// ��� �̹��� �̵�
				
				
			}
			else if (event.key.keysym.sym == SDLK_DOWN) {
				// ���� ���� downHold�� �� hold, wall, bg ��������
				// �ƴѰ�� stunȿ��?
				f_state = 1;
				// ��� �̹��� �̵�
				
			}
			if (f_state != hold[cur_i]) {
				showStunEffect = true;
				// SDL_Delay(1000);
			}
			
			break;

		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_DOWN) {
				// �Ʒ� Ű�� ���� f_state ���� �ʱ�ȭ�մϴ�.
				f_state = 0;
			}
			break;
		
		default:
			break;
		}
	}

}
/*

void Mode1::ResetGame() {
	f_state = 0;
	cur_i = 1;
	prevHold = 0;

	wall_dest_rect.y = 0;
	bg_dest_rect.y = 0;

	isLeftUser = true;
	leftUser_dest_rect = { 280, 427, leftUser_surface->w, leftUser_surface->h };
	rightUser_dest_rect = { 410, 427, rightUser_surface->w, rightUser_surface->h };
	isLeftUser = true;

	// Reset hold positions
	leftHoldY = 475;
	rightHoldY = 375;
	for (size_t i = 0; i < hold_dest_rects.size(); i++) {
		if (i % 2 == 0) {
			hold_dest_rects[i].y = leftHoldY;
			leftHoldY -= 200;
		}
		else {
			hold_dest_rects[i].y = rightHoldY;
			rightHoldY -= 200;
		}
	}
}
*/