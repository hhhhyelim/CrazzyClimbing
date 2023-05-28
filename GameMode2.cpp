//GameMode2.cpp
#include "Game.h"
#include "GameMode2.h"
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

// hold
vector<string> hold_paths = { "../src/redHold.png", "../src/greenHold.png", "../src/blueHold.png" };
vector<SDL_Surface*> hold_surfaces;
vector<SDL_Texture*> hold_textures;
vector<SDL_Rect> hold_rects;
vector<SDL_Rect> hold_dest_rects;
int leftHoldY = 475;
int rightHoldY = 400;

int hold[100]; // 1:red 2:green 3:blue
int f_state; // 1:red 2: green 3:blue
bool isLeftUser = true;
string nextHoldPath = "";
int prevHoldIndex = -1;

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

//������
int gauge = 150; // �ʱ� ������ ��
int gaugeDecreaseRate = 1; // ������ ���� �ӵ�

//�ڷΰ����ư
SDL_Surface* back_surface;
SDL_Texture* back_texture;
SDL_Rect back_rect;
SDL_Rect back_dest_rect;

Mode2::Mode2() {
	g_flag_running = true;
	cur_i = 1;
	prevHold = 0;

	// ��� �̹��� �ε�
	bg_surface = IMG_Load("../src/bg_mode2.png");
	SDL_Surface* resized_surface = SDL_CreateRGBSurface(0, 800, 600, 32, 0, 0, 0, 0);
	SDL_Rect src_rect = { 0, 3000 - 600, 800, 600 };
	SDL_BlitSurface(bg_surface, &src_rect, resized_surface, NULL);
	bg_texture = SDL_CreateTextureFromSurface(g_renderer, resized_surface);
	SDL_FreeSurface(bg_surface);
	SDL_FreeSurface(resized_surface);
	bg_rect = { 0, 0, 800, 600 };
	bg_dest_rect = { 0, 0, 800, 600 };

	// wall �̹��� �ε�
	wall_surface = IMG_Load("../src/wall.png");
	wall_texture = SDL_CreateTextureFromSurface(g_renderer, wall_surface); // GPU�� �ű�� 
	SDL_FreeSurface(wall_surface);
	wall_rect = { 0, 0, 400, 600 };
	wall_dest_rect = { 200, 0, 400, 600 };

	srand((unsigned)time(NULL)); // srand�� �� ���� ȣ���ؾ� �մϴ�.
	for (int i = 0; i < 40; i++) {
		//int random_hold_idx = rand() % 3; // 0~2 �� �ϳ��� �ε����� �������� ����
		int random_hold_idx;
		do {
			random_hold_idx = rand() % 3; // 0~3 �� �ϳ��� �ε����� �������� ����
		} while (random_hold_idx == prevHoldIndex); // ���� �ε����� ���� �ε����� ������ ���ο� �ε����� ��� �����մϴ�
		prevHoldIndex = random_hold_idx;
		string hold_path = hold_paths[random_hold_idx];
		SDL_Surface* hold_surface = IMG_Load(hold_path.c_str());
		SDL_Texture* hold_texture = SDL_CreateTextureFromSurface(g_renderer, hold_surface);
		SDL_Rect hold_rect = { 0, 0, hold_surface->w, hold_surface->h };

		hold_surfaces.push_back(hold_surface);
		hold_textures.push_back(hold_texture);
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

		if (hold_path == "../src/redHold.png") {
			nextHoldPath = hold_path;
			hold[i] = 1;
		}
		else if (hold_path == "../src/greenHold.png") {
			nextHoldPath = hold_path;
			hold[i] = 2;
		}
		else if (hold_path == "../src/blueHold.png") {
			nextHoldPath = hold_path;
			hold[i] = 3;
		}
	}

	// leftUser
	leftUser_surface = IMG_Load("../src/leftUser.png");
	leftUser_texture = SDL_CreateTextureFromSurface(g_renderer, leftUser_surface);
	SDL_FreeSurface(leftUser_surface);
	leftUser_rect = { 0, 0, leftUser_surface->w, leftUser_surface->h };
	leftUser_dest_rect = { 240, 447, leftUser_surface->w, leftUser_surface->h };

	// rightUser
	rightUser_surface = IMG_Load("../src/rightUser.png");
	rightUser_texture = SDL_CreateTextureFromSurface(g_renderer, rightUser_surface);
	SDL_FreeSurface(rightUser_surface);
	rightUser_rect = { 0, 0, rightUser_surface->w, rightUser_surface->h };
	rightUser_dest_rect = { 240, 447, rightUser_surface->w, rightUser_surface->h };

	//�ڷΰ���
	back_surface = IMG_Load("../src/back.png");
	back_texture = SDL_CreateTextureFromSurface(g_renderer, back_surface);
	SDL_FreeSurface(back_surface);
	back_rect = { 0, 0, back_surface->w, back_surface->h };
	back_dest_rect = { 10, 10, back_surface->w, back_surface->h };
}

Mode2::~Mode2() {
	SDL_DestroyTexture(bg_texture);
	SDL_DestroyTexture(wall_texture);

	for (SDL_Texture* texture : hold_textures) {
		SDL_DestroyTexture(texture);
	}
	SDL_DestroyTexture(leftUser_texture);
	SDL_DestroyTexture(rightUser_texture);
	SDL_DestroyTexture(back_texture);
}

void Mode2::Update()
{
	if (checkHold()) {
		userMove();
		holdMove();
		cur_i++;
		gauge += 5;
	}
	else {
		// ������ ����
		gauge -= gaugeDecreaseRate;
		if (gauge < 0) {
			gauge = 0;
		}
	}
		// �������� 0�̸� ���� ����
		if (gauge == 0) {
			ResetGame();
			g_current_game_phase = PHASE_ENDING2;
		}

	if (checkHold()) {
		// Background Update
		bg_dest_rect.y += 10;
		if (bg_dest_rect.y >= 600) {
			bg_dest_rect.y = 0;
		}
		//wall
		if (wall_dest_rect.y >= 600) {
			wall_dest_rect.y = 0;
		}
	}
}

void Mode2::Render() {
	//����̾�����
	{
		SDL_Rect tmp_r;
		tmp_r.x = 0;
		tmp_r.y = bg_dest_rect.y - 600;
		tmp_r.w = bg_dest_rect.w;
		tmp_r.h = bg_dest_rect.h;
		SDL_RenderCopy(g_renderer, bg_texture, NULL, &tmp_r);

		tmp_r.x = 0;
		tmp_r.y = bg_dest_rect.y;
		tmp_r.w = bg_dest_rect.w;
		tmp_r.h = bg_dest_rect.h;
		SDL_RenderCopy(g_renderer, bg_texture, NULL, &tmp_r);
	}
	//���̾�����
	{
		SDL_Rect tmp_r;
		tmp_r.x = wall_dest_rect.x;
		tmp_r.y = wall_dest_rect.y - 600;
		tmp_r.w = wall_dest_rect.w;
		tmp_r.h = wall_dest_rect.h;
		SDL_RenderCopy(g_renderer, wall_texture, &wall_rect, &tmp_r);
		tmp_r.x = wall_dest_rect.x;
		tmp_r.y = wall_dest_rect.y;
		tmp_r.w = wall_dest_rect.w;
		tmp_r.h = wall_dest_rect.h;
		SDL_RenderCopy(g_renderer, wall_texture, &wall_rect, &tmp_r);
	}
		// ������ �׸���
		SDL_Rect gaugeRect = { 615, 20, gauge, 30 }; // ������ ��ġ�� ũ�� ����
		SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255); // ������ ���� (������)
		SDL_RenderFillRect(g_renderer, &gaugeRect);

		// ������ �׵θ� �׸���
		SDL_Rect gaugeBorderRect = { 615, 20, 150, 30 }; // ������ �׵θ� ��ġ�� ũ�� ����
		SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255); // ������ �׵θ� ���� (���)
		int borderWidth = 4; // �׵θ� �β� ����
		for (int i = 0; i < borderWidth; i++) {
			SDL_Rect borderRect = { gaugeBorderRect.x - i, gaugeBorderRect.y - i, gaugeBorderRect.w + 2 * i, gaugeBorderRect.h + 2 * i };
			SDL_RenderDrawRect(g_renderer, &borderRect);
		}

		/*
		//�ڷΰ����ư�׸���
		SDL_Rect back;
		back.x = back_dest_rect.x;
		back.y = back_dest_rect.y;
		back.w = back_dest_rect.w;
		back.h = back_dest_rect.h;
		SDL_RenderCopy(g_renderer, back_texture, &back_rect, &back);
		*/

		//Ȧ��׸���
		for (size_t i = 0; i < hold_textures.size(); i++) {
			SDL_RenderCopy(g_renderer, hold_textures[i], &hold_rects[i], &hold_dest_rects[i]);
		}
		SDL_RenderCopy(g_renderer, leftUser_texture, &leftUser_rect, &leftUser_dest_rect);

		SDL_RenderPresent(g_renderer);

	}

void Mode2::userMove() {
	//��� �Ʒ��� �����̱�
	wall_dest_rect.y += 30;
	bg_dest_rect.y += 5;

	//���������϶� ������������ �ٲٱ�
	if (isLeftUser) {
		leftUser_dest_rect.x += 200;
		rightUser_dest_rect.x -= 200;
	}
	//�����������϶� ���������� �ٲٱ�
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

void Mode2::holdMove() {
	for (SDL_Rect& hold_dest_rect : hold_dest_rects) {
		hold_dest_rect.y += 75;
	}
}

bool Mode2::checkHold() {
	if (hold[cur_i] == f_state) {
		prevHold = f_state;
		return true;
	}
	else {
		prevHold = 0; // hold�� ��ġ���� ������ prevHold ���� �ʱ�ȭ�մϴ�.
	}
	return false;
}

void Mode2::HandleEvents() {
	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			g_flag_running = false;
			break;

			/*
		case SDL_MOUSEBUTTONDOWN: //�ڷΰ����ư������
			//if the mouse left button is pressed
			if (event.button.button == SDL_BUTTON_LEFT) {
				int mouse_x = event.button.x;
				int mouse_y = event.button.y;

				if (mouse_x >= back_dest_rect.x &&
					mouse_x <= back_dest_rect.x + back_dest_rect.w &&
					mouse_y >= back_dest_rect.y &&
					mouse_y <= back_dest_rect.y + back_dest_rect.h)
					ResetGame();
					g_current_game_phase = PHASE_INTRO;
				break;
			}*/

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_r) {
				f_state = 1;
				gauge += 10;
				if (gauge > 100) {
					gauge = 100;
				}
			}
			else if (event.key.keysym.sym == SDLK_g) {
				f_state = 2;
				gauge += 10; // ��ư�� ���� �� �������� ������Ŵ
				if (gauge > 100) {
					gauge = 100;
				}
			}
			else if (event.key.keysym.sym == SDLK_b) {
				f_state = 3;
				gauge += 10; // ��ư�� ���� �� �������� ������Ŵ
				if (gauge > 100) {
					gauge = 100;
				}
			}
			//Ʋ������ ���ӿ���
			if (f_state != hold[cur_i]) {
				SDL_Delay(2000); //�´� ȿ���� ������ ��������!!!
				ResetGame();
				g_current_game_phase = PHASE_ENDING2; // ����� ����2�� ��ȯ
			}
			break;

		default:
			break;
			}
		}
	}

void Mode2::ResetGame() {
	f_state = 0;
	gauge = 150;
	cur_i = 1;
	prevHold = 0;

	wall_dest_rect.y = 0;
	bg_dest_rect.y = 0;

	isLeftUser = true;
	leftUser_dest_rect = { 240, 447, leftUser_surface->w, leftUser_surface->h };
	rightUser_dest_rect = { 560, 447, rightUser_surface->w, rightUser_surface->h };
	isLeftUser = true;

	// Reset hold positions
	leftHoldY = 475;
	rightHoldY = 400;
	for (size_t i = 0; i < hold_dest_rects.size(); i++) {
		if (i % 2 == 0) {
			hold_dest_rects[i].y = leftHoldY;
			leftHoldY -= 150;
		}
		else {
			hold_dest_rects[i].y = rightHoldY;
			rightHoldY -= 150;
		}
	}
}
