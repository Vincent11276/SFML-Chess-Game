#pragma once

#include <iostream>

#include "Core/Engine/GameStateManager.hpp"
#include "Core/Engine/GameState.hpp"
#include "Client/Scenes/Chess.hpp"
#include "Core/Utility/Helper.hpp"
#include "Network/ServerMessage.hpp"
#include "Network/ChessClient.hpp"
#include <TGUI/TGUI.hpp>


namespace states
{
	class InOnlineGameState : public engine::GameState
	{
	public:
		void init() override;
		void cleanup() override { };
		void pause() override { };
		void resume() override { };

		void initUI();
		void initChess();
		void initNetworking();

		void handleEvent(sf::Event& e) override;
		void update(float deltaTime) override;
		void physicsUpdate(float deltaTime) override { };
		void draw(sf::RenderTarget& target) const override;

		static InOnlineGameState* getInstance();

	protected:
		InOnlineGameState();

	private:
		mutable tgui::Gui m_gui;

		tgui::Panel::Ptr m_yourProfile_Pnl;
		tgui::Panel::Ptr m_enemyProfile_Pnl;
		tgui::ChatBox::Ptr m_chatBox_Chtb;
		tgui::ListView::Ptr m_moveHistory_LstV;
		tgui::EditBox::Ptr m_chatBox_Edt;
		tgui::Button::Ptr m_send_Btn;

		ChessGame m_chessGame;
	};
}