//GameMode2.cpp
#include "Game.h"
#include "GameMode2.h"
#include <vector>
#include <cstdlib>
#include <chrono>

using namespace std;
vector<string> hold2_paths = { "../../Resources/m2/redHold.png", "../../Resources/m2/greenHold.png", "../../Resources/m2/blueHold.png" };
vector<SDL_Surface*> hold2_surfaces;
vector<SDL_Texture*> hold2_textures;
vector<SDL_Rect> hold2_rects;
vector<SDL_Rect> hold2_dest_rects;

string nextHoldPath2 = "";

Mode2::Mode2() {
	g_flag_running = true;
	cur_i = 1;
	prevHold = 0;

	//STATE
	tutorial = true;
	ready = false;
	start = false;
	game_start = false;
	game_over = false;
	game_ending = false;

	//��Ʈ�� Ʃ�丮��
	SDL_Surface* intro_surface = IMG_Load("../../Resources/m2/intro.png");
	g_texture_intro = SDL_CreateTextureFromSurface(g_renderer, intro_surface);
	SDL_FreeSurface(intro_surface);
	SDL_QueryTexture(g_texture_intro, NULL, NULL, &g_source_rectangle_intro.w, &g_source_rectangle_intro.h);
	g_source_rectangle_intro = { 0, 0, 800, 600 };
	g_destination_rectangle_intro = { 95, 40, 620, 480 };

	//play��ư
	SDL_Surface* play_surface = IMG_Load("../../Resources/m2/play.png");
	g_texture_play = SDL_CreateTextureFromSurface(g_renderer, play_surface);
	SDL_FreeSurface(play_surface);
	SDL_QueryTexture(g_texture_play, NULL, NULL, &g_source_rectangle_play.w, &g_source_rectangle_play.h);
	g_source_rectangle_play = { 0, 0, 160, 80 };
	g_destination_rectangle_play = { 320, 510, 160, 80 };

	//�ڷΰ���
	introBack_surface = IMG_Load("../../Resources/m2/back.png");
	introBack_texture = SDL_CreateTextureFromSurface(g_renderer, introBack_surface);
	SDL_FreeSurface(introBack_surface);
	introBack_rect = { 0, 0, introBack_surface->w,introBack_surface->h };
	introBack_dest_rect = { 10, 10, introBack_surface->w, introBack_surface->h };

	//����
	ready_surface = IMG_Load("../../Resources/m2/ready.png");
	ready_texture = SDL_CreateTextureFromSurface(g_renderer, ready_surface);
	SDL_FreeSurface(ready_surface);
	ready_rect = { 0, 0, ready_surface->w,ready_surface->h };
	ready_dest_rect = { 10, 10, ready_surface->w, ready_surface->h };

	//��ŸƮ
	start_surface = IMG_Load("../../Resources/m2/start.png");
	start_texture = SDL_CreateTextureFromSurface(g_renderer, start_surface);
	SDL_FreeSurface(start_surface);
	start_rect = { 0, 0, start_surface->w,start_surface->h };
	start_dest_rect = { 10, 10, start_surface->w, start_surface->h };

	// ��� �̹��� �ε�
	bg_surface = IMG_Load("../../Resources/m2/bg_mode2.png");
	bg_texture = SDL_CreateTextureFromSurface(g_renderer, bg_surface); // GPU�� �ű�� 
	SDL_FreeSurface(bg_surface);
	backgroundY = -2400; 	// ��� �̹��� �ʱ� ��ġ ���� (ȭ�� ����)

	// wall �̹��� �ε�
	wall_surface = IMG_Load("../../Resources/m2/wall_mode2.png");
	wall_texture = SDL_CreateTextureFromSurface(g_renderer, wall_surface); // GPU�� �ű�� 
	SDL_FreeSurface(wall_surface);
	wall_rect = { 0, 0, 400, 600 };
	wall_dest_rect = { 200, 0, 400, 600 };



	//Ȧ�� �̹��� �ε�
	srand((unsigned)time(NULL)); // srand�� �� ���� ȣ���ؾ� �մϴ�.
	for (int i = 0; i < 500; i++) {
		int random_hold_idx;
		do {
			random_hold_idx = rand() % 3; // 0~3 �� �ϳ��� �ε����� �������� ����
		} while (random_hold_idx == prevHoldIndex); // ���� �ε����� ���� �ε����� ������ ���ο� �ε����� ��� �����մϴ�

		prevHoldIndex = random_hold_idx;
		string hold_path = hold2_paths[random_hold_idx];
		SDL_Surface* hold_surface = IMG_Load(hold_path.c_str());
		SDL_Texture* hold_texture = SDL_CreateTextureFromSurface(g_renderer, hold_surface);
		SDL_Rect hold_rect = { 0, 0, hold_surface->w, hold_surface->h };

		hold2_surfaces.push_back(hold_surface);
		hold2_textures.push_back(hold_texture);
		hold2_rects.push_back(hold_rect);

		if (i % 2 == 0) {
			SDL_Rect hold_dest_rect = { 315, leftHoldY , 50, 50 };
			hold2_dest_rects.push_back(hold_dest_rect);
			leftHoldY -= 200;
		}
		else {
			SDL_Rect hold_dest_rect = { 440, rightHoldY , 50, 50 };
			hold2_dest_rects.push_back(hold_dest_rect);
			rightHoldY -= 200;
		}

		if (hold_path == "../../Resources/m2/redHold.png") {
			nextHoldPath2 = hold_path;
			hold[i] = 1;
		}
		else if (hold_path == "../../Resources/m2/greenHold.png") {
			nextHoldPath2 = hold_path;
			hold[i] = 2;
		}
		else if (hold_path == "../../Resources/m2/blueHold.png") {
			nextHoldPath2 = hold_path;
			hold[i] = 3;
		}
	}

	// leftUser
	leftUser_surface = IMG_Load("../../Resources/m2/leftUser.png");
	leftUser_texture = SDL_CreateTextureFromSurface(g_renderer, leftUser_surface);
	SDL_FreeSurface(leftUser_surface);
	leftUser_rect = { 0, 0, leftUser_surface->w, leftUser_surface->h };
	leftUser_dest_rect = { 280, 427, leftUser_surface->w, leftUser_surface->h };

	// rightUser
	rightUser_surface = IMG_Load("../../Resources/m2/rightUser.png");
	rightUser_texture = SDL_CreateTextureFromSurface(g_renderer, rightUser_surface);
	SDL_FreeSurface(rightUser_surface);
	rightUser_rect = { 0, 0, rightUser_surface->w, rightUser_surface->h };
	rightUser_dest_rect = { 410, 427, rightUser_surface->w, rightUser_surface->h };

	//ENDING
	//��������
	SDL_Surface* ending_surface = IMG_Load("../../Resources/m2/bg_mode2_ending.png");
	g_texture_ending = SDL_CreateTextureFromSurface(g_renderer, ending_surface);
	SDL_FreeSurface(ending_surface);
	SDL_QueryTexture(g_texture_ending, NULL, NULL, &g_source_rectangle_ending.w, &g_source_rectangle_ending.h);
	g_source_rectangle_ending = { 0, 0, 800, 600 };
	g_destination_rectangle_ending = { 0, 0, 800, 600 };

	//home ��ư
	SDL_Surface* home_surface = IMG_Load("../../Resources/m2/home_btn.png");
	g_texture_home = SDL_CreateTextureFromSurface(g_renderer, home_surface);
	SDL_FreeSurface(home_surface);
	SDL_QueryTexture(g_texture_home, NULL, NULL, &g_source_rectangle_home.w, &g_source_rectangle_home.h);
	g_source_rectangle_home = { 0, 0, 160, 80 };
	g_destination_rectangle_home = { 210, 435, 160, 80 };

	//retry
	SDL_Surface* retry_surface = IMG_Load("../../Resources/m2/retry.png");
	g_texture_retry = SDL_CreateTextureFromSurface(g_renderer, retry_surface);
	SDL_FreeSurface(retry_surface);
	SDL_QueryTexture(g_texture_retry, NULL, NULL, &g_source_rectangle_retry.w, &g_source_rectangle_retry.h);
	g_source_rectangle_retry = { 0, 0, 160, 80 };
	g_destination_rectangle_retry = { 440, 435, 160, 80 };

	// num
	font = TTF_OpenFont("../../Resources/m2/DungGeunMo.ttf", 60);
}

Mode2::~Mode2() {
	SDL_DestroyTexture(g_texture_intro);
	SDL_DestroyTexture(g_texture_play);

	SDL_DestroyTexture(bg_texture);
	SDL_DestroyTexture(wall_texture);

	for (SDL_Texture* texture : hold2_textures) {
		SDL_DestroyTexture(texture);
	}
	SDL_DestroyTexture(leftUser_texture);
	SDL_DestroyTexture(rightUser_texture);
	SDL_DestroyTexture(introBack_texture);

	SDL_DestroyTexture(g_texture_ending);
	SDL_DestroyTexture(g_texture_home);
	SDL_DestroyTexture(g_texture_retry);
}

void Mode2::Update()
{
	Uint32 currentTime = SDL_GetTicks();

	//����
	if (ready) {
		if (currentTime - startTime >= 2000) {
			startTime = currentTime;

			tutorial = false;
			ready = false;
			start = true;
			game_start = false;
			game_over = false;
			game_ending = false;
		}
	}
	//��ŸƮ
	else if (start) {
		if (currentTime - startTime >= 1000) {

			startTime = currentTime;

			tutorial = false;
			ready = false;
			start = false;
			game_start = true;
			game_over = false;
			game_ending = false;
		}
	}

	if (game_start && !game_over) {
		//�´µ� ������ ��
		if (checkHold()) {
			userMove();
			holdMove();
			cur_i++;
			gauge += 5;
			backgroundY += 30;
			holdCount++;

			if (cur_i >= 50) { //Ȧ�� 50�� ������ ���ǵ� ��!
				gaugeDecreaseRate = gaugeDecreaseRate2;
			}
		}
		else {
			gauge -= gaugeDecreaseRate;
			if (gauge < 0) {
				gauge = 0;
			}
		}
		// �������� 0�̸� ���� ����
		if (gauge == 0) {
			tutorial = false;
			ready = false;
			start = false;
			game_start = false;
			game_over = true;
			game_ending = false;

		}

		if (checkHold()) {
			//wall
			if (wall_dest_rect.y >= 600) {
				wall_dest_rect.y = 0;
			}
		}
	}

	if (game_over) {
		Uint32 elapsedTime = currentTime - gameoverTime;
		if (game_over && elapsedTime >= 2000) {
			tutorial = false;
			ready = false;
			start = false;
			game_start = false;
			game_over = false;
			game_ending = true;
		}
	}
	if (game_ending) {
		tutorial = false;
		ready = false;
		start = false;
		game_start = false;
		game_over = false;

		// �ð��� ȭ�鿡 ǥ���ϱ� ���� ���ڿ��� ��ȯ
		std::string gameTimeString = std::to_string(holdCount);

		SDL_Color black = { 0, 0, 0, 0 };
		num_surface = TTF_RenderText_Blended(font, gameTimeString.c_str(), black);

		// ǥ���� �ؽ�ó�� ��ȯ
		num_texture = SDL_CreateTextureFromSurface(g_renderer, num_surface);
		SDL_FreeSurface(num_surface);

		SDL_QueryTexture(num_texture, NULL, NULL, &(num_rect.w), &(num_rect.h));
		num_rect.x = 630;
		num_rect.y = 303;
	}
}

void Mode2::Render() {
	SDL_RenderClear(g_renderer);

	//����̾�����
	SDL_Rect backgroundRect = { 0, backgroundY, BACKGROUND_WIDTH, BACKGROUND_HEIGHT };
	SDL_RenderCopy(g_renderer, bg_texture, NULL, &backgroundRect);

	//���̾�����
	{
		int currentY = wall_dest_rect.y + wall_dest_rect.h;
		while (currentY > -wall_rect.h) {
			SDL_Rect tmp_r;
			tmp_r.x = wall_dest_rect.x;
			tmp_r.y = currentY - 600;
			tmp_r.w = wall_dest_rect.w;
			tmp_r.h = wall_dest_rect.h;
			SDL_RenderCopy(g_renderer, wall_texture, &wall_rect, &tmp_r);
			currentY -= wall_rect.h;
		}
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

	if (tutorial) {
		//Tutorial
		SDL_RenderCopy(g_renderer, g_texture_intro, &g_source_rectangle_intro, &g_destination_rectangle_intro);
		// Play Button
		SDL_RenderCopy(g_renderer, g_texture_play, &g_source_rectangle_play, &g_destination_rectangle_play);
		// Back Button
		SDL_RenderCopy(g_renderer, introBack_texture, &introBack_rect, &introBack_dest_rect);
	}

	else if (ready) {
		SDL_RenderCopy(g_renderer, ready_texture, &ready_rect, &ready_dest_rect);
	}

	else if (start) {
		SDL_RenderCopy(g_renderer, start_texture, &start_rect, &start_dest_rect);
	}

	else if (game_start) {
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

		//Ȧ��׸���
		for (size_t i = 0; i < hold2_textures.size(); i++) {
			SDL_RenderCopy(g_renderer, hold2_textures[i], &hold2_rects[i], &hold2_dest_rects[i]);
		}

		//���
		SDL_RenderCopy(g_renderer, leftUser_texture, &leftUser_rect, &leftUser_dest_rect);
	}
	else if (game_ending) {
		SDL_RenderCopy(g_renderer, g_texture_ending, &g_source_rectangle_ending, &g_destination_rectangle_ending);
		SDL_RenderCopy(g_renderer, g_texture_home, &g_source_rectangle_home, &g_destination_rectangle_home);
		SDL_RenderCopy(g_renderer, g_texture_retry, &g_source_rectangle_retry, &g_destination_rectangle_retry);

		// Ȧ�� ���� ���
		SDL_RenderCopy(g_renderer, num_texture, NULL, &num_rect);
	}
	SDL_RenderPresent(g_renderer);
}


void Mode2::userMove() {
	//��� �Ʒ��� �����̱�
	wall_dest_rect.y += 20;

	//���������϶� ������������ �ٲٱ�
	if (isLeftUser) {
		leftUser_dest_rect.x += 150;
		rightUser_dest_rect.x -= 150;
	}
	//�����������϶� ���������� �ٲٱ�
	else {
		leftUser_dest_rect.x -= 150;
		rightUser_dest_rect.x += 150;
	}

	SDL_Surface* tempSurface = leftUser_surface;
	leftUser_surface = rightUser_surface;
	rightUser_surface = rightUser_surface;

	SDL_Texture* tempTexture = leftUser_texture;
	leftUser_texture = rightUser_texture;
	rightUser_texture = tempTexture;

	isLeftUser = !isLeftUser;
}

void Mode2::holdMove() {
	for (SDL_Rect& hold_dest_rect : hold2_dest_rects) {
		hold_dest_rect.y += 100;
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

		case SDL_MOUSEBUTTONDOWN:

			// If the mouse left button is pressed. 
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				int mouseX = event.button.x;
				int mouseY = event.button.y;
				//�ڷΰ���(home����) ���߿� ��ġ��!!!!!
				if (mouseX >= introBack_rect.x && mouseX < introBack_dest_rect.x + introBack_dest_rect.w
					&& mouseY >= introBack_dest_rect.y && mouseY < introBack_dest_rect.y + introBack_dest_rect.h
					&& g_current_game_phase == PHASE_MODE2) {
					g_current_game_phase = PHASE_HOME;
					tutorial = true;
					ready = false;
					start = false;
					game_start = false;
					game_over = false;
					game_ending = false;
				}

				//game start
				if (mouseX >= start_dest_rect.x && mouseX < start_dest_rect.x + start_dest_rect.w
					&& mouseY >= start_dest_rect.y && mouseY < start_dest_rect.y + start_dest_rect.h
					&& g_current_game_phase == PHASE_MODE2 && tutorial == true) {
					tutorial = false;
					ready = true;
					start = false;
					game_start = false;
					game_over = false;
					game_ending = false;
					startTime = SDL_GetTicks();
				}
				// retry
				if (
					mouseX >= g_destination_rectangle_retry.x &&
					mouseX <= g_destination_rectangle_retry.x + g_destination_rectangle_retry.w &&
					mouseY >= g_destination_rectangle_retry.y &&
					mouseY <= g_destination_rectangle_retry.y + g_destination_rectangle_retry.h) {
					tutorial = true;
					ready = false;
					start = false;
					game_start = false;
					game_over = false;
					game_ending = false;
					ResetGame();
				}

				// home ��ư
				if (mouseX >= g_destination_rectangle_home.x &&
					mouseX <= g_destination_rectangle_home.x + g_destination_rectangle_home.w &&
					mouseY >= g_destination_rectangle_home.y &&
					mouseY <= g_destination_rectangle_home.y + g_destination_rectangle_home.h) {
					g_current_game_phase = PHASE_HOME;
					tutorial = true;
					ready = false;
					start = false;
					game_start = false;
					game_over = false;
					game_ending = false;
					ResetGame();
				}
			}
			break;

		case SDL_KEYDOWN:
			if (!tutorial && !ready && !start && event.key.keysym.sym)
			{
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
					SDL_Delay(2000); //�´� ȿ���� ������ ��������!!
					tutorial = false;
					ready = false;
					start = false;
					game_start = false;
					game_over = true;
					game_ending = true;
				}
				break;

		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_DOWN) {
				// �Ʒ� Ű�� ���� f_state ���� �ʱ�ȭ�մϴ�.
				f_state = 0;
			}
			}
			break;
		default:
			break;
		}
	}
}

void Mode2::ResetGame() {
	f_state = 0;
	cur_i = 1;
	prevHold = 0;
	gauge = 150;
	holdCount = 0;

	backgroundY = -2400;

	userMove();
	//isLeftUser = !isLeftUser;
	leftUser_dest_rect.x = 280;
	rightUser_dest_rect.x = 410;
	//leftUser_dest_rect = { 280, 427, leftUser_surface->w, leftUser_surface->h };
	//rightUser_dest_rect = { 410, 427, rightUser_surface->w, rightUser_surface->h };

	// Reset hold positions
	leftHoldY = 475;
	rightHoldY = 375;
	for (size_t i = 0; i < hold2_dest_rects.size(); i++) {
		if (i % 2 == 0) {
			hold2_dest_rects[i].y = leftHoldY;
			leftHoldY -= 200;
		}
		else {
			hold2_dest_rects[i].y = rightHoldY;
			rightHoldY -= 200;
		}
	}

}