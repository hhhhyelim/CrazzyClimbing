#pragma once

#include "SDL.h"

class Mode1 : public PhaseInterface
{
public:
	Mode1();
	~Mode1();

	virtual void HandleEvents() override;
	virtual void Update() override;
	virtual void Render() override;
	void userMove();
	void holdMove();
	bool checkHold();
	void ResetGame();
	void stun_effect();
private:
	int cur_i;
	int prevHold;
};