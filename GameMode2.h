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
};