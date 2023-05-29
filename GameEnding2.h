#pragma once

#include "SDL.h"

class Ending2 : public PhaseInterface
{
public:
	Ending2();
	~Ending2();
	virtual void HandleEvents();
	virtual void Update();
	virtual void Render();

private:

};

