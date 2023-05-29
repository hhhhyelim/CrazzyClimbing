#pragma once
#include "Game.h"

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

private:
	int cur_i;
	int prevHold;

	bool tutorial;
	bool ready;
	bool start;
	bool game_start;
	bool game_over;
	bool game_ending;

	int currentCharacterIndex;
	bool correct_button;
};