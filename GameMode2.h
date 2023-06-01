#pragma once
#include "Game.h"
#include <vector>

class Mode2 : public PhaseInterface {
public:
	Mode2();
	~Mode2();

	virtual void HandleEvents() override;
	virtual void Update() override;
	virtual void Render() override;
	void userMove();
	void holdMove();
	bool checkHold();
	void ResetGame();
	void printscore(int count);

private:
	int cur_i;
	int prevHold;


	Uint32 startTime = 0;
	Uint32 gameoverTime = 0;

	//Ȧ�尹�� ��Ʈ
	int holdCount = 0;

	// num
	TTF_Font* font;
	SDL_Surface* num_surface;
	SDL_Texture* num_texture;
	SDL_Rect num_rect;

	//��Ʈ��
	SDL_Texture* g_texture_intro;
	SDL_Rect g_source_rectangle_intro;
	SDL_Rect g_destination_rectangle_intro;

	//�÷��̹�ư
	SDL_Texture* g_texture_play;
	SDL_Rect g_source_rectangle_play;
	SDL_Rect g_destination_rectangle_play;

	//�ڷΰ����ư
	SDL_Surface* introBack_surface;
	SDL_Texture* introBack_texture;
	SDL_Rect introBack_rect;
	SDL_Rect introBack_dest_rect;

	//����
	SDL_Surface* ready_surface;
	SDL_Texture* ready_texture;
	SDL_Rect ready_rect;
	SDL_Rect ready_dest_rect;

	//����
	SDL_Surface* start_surface;
	SDL_Texture* start_texture;
	SDL_Rect start_rect;
	SDL_Rect start_dest_rect;

	// ��� �̹��� ũ��
	const int BACKGROUND_WIDTH = 800;
	const int BACKGROUND_HEIGHT = 3000;

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

	const int WALL_WIDTH = 400;
	const int WALL_HEIGHT = 600;

	// hold
	int leftHoldY = 475;
	int rightHoldY = 375;

	int hold[500]; // 1:red 2:green 3:blue
	int f_state; // 1:red 2: green 3:blue
	bool isLeftUser = true;
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
	int gaugeDecreaseRate2 = 2; //50�� ������ ������ ���� �ø���

	//��������
	SDL_Texture* g_texture_ending;
	SDL_Rect g_source_rectangle_ending;
	SDL_Rect g_destination_rectangle_ending;

	//Ȩ��ư
	SDL_Texture* g_texture_home;
	SDL_Rect g_source_rectangle_home;
	SDL_Rect g_destination_rectangle_home;

	//�ٽý��۹�ư
	SDL_Texture* g_texture_retry;
	SDL_Rect g_source_rectangle_retry;
	SDL_Rect g_destination_rectangle_retry;

	bool tutorial;
	bool ready;
	bool start;
	bool game_start;
	bool game_over;
	bool game_ending;

	int currentCharacterIndex;
	bool correct_button;
};