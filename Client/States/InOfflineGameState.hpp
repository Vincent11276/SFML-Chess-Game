#pragma once

#include <iostream>

#include "Core/Engine/GameStateManager.hpp"
#include "Core/Engine/GameState.hpp"
#include "Client/Scenes/Chess.hpp"
#include "Core/Utility/Helper.hpp"


namespace states
{
	class InOfflineGameState : public egn::GameState
	{
	public:
		void init() override;
		void cleanup() override { };
		void pause() override { };
		void resume() override { };

		void handleEvent(sf::Event& e) override;
		void update(float deltaTime) override;
		void physicsUpdate(float deltaTime) override;
		void draw(sf::RenderTarget& target) const override;

		static InOfflineGameState* getInstance();

	protected:
		InOfflineGameState();

	private:
		ChessGame m_chessGame;
	};
}