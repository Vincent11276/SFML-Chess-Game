#pragma once

#include "../Engine/StateMachine.hpp"

#include <SFML/Graphics.hpp>

class MachineState
{
public:
	virtual void init() = 0;
	virtual void cleanup() = 0;
	virtual void pause() = 0; 
	virtual void resume() = 0;

	void changeState(StateMachine* game, MachineState* state)
	{
		game->changeState(state);
	}

protected:
	MachineState() { }
};