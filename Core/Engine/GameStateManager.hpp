#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Core/Utility/Logger.hpp"
#include "GameState.hpp"

namespace egn
{
	class GameState;

	class GameStateManager
	{
	public:
		void changeState(GameState* state);
		void cleanUp();
		void pushState(GameState* state);
		void popState();

		void handleEvent(sf::Event& event);
		void update(float deltaTime);
		void physicsUpdate(float deltaTime);
		void render(sf::RenderTarget& target);

		static GameStateManager* getInstance();

	private:
		GameStateManager();

		// the stack of states
		std::vector<GameState*> m_states;
	};
}