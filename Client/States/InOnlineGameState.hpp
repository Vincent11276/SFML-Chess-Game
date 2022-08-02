#pragma once

#include <iostream>

#include "GameStateManager.hpp"
#include "GameState.hpp"

#include "Client/Scenes/Chess.hpp"
#include "Core/Utility/Helper.hpp"
#include "Client/ServerMessage.hpp"
#include "Client/ChessClient.hpp"


class InOnlineGameState : public GameState
{
public:
	void init(GameStateManager* game) override;
	void cleanup(GameStateManager* game) override { };
	void pause(GameStateManager* game) override { };
	void resume(GameStateManager* game) override { };

	void handleEvent(GameStateManager* manager, sf::Event& e) override;
	void update(GameStateManager* manager, float deltaTime) override;
	void physicsUpdate(GameStateManager* manager, float deltaTime) override { };
	void draw(GameStateManager* manager, sf::RenderTarget& target) const override;

	static InOnlineGameState* getInstance(ChessClient* client);

protected:
	InOnlineGameState(ChessClient* client);

private:
	void onMessageReceivedCallback();

	ChessClient* m_client;
	ChessGame m_chessGame;
};

