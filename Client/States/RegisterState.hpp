#pragma once

#include <TGUI/TGUI.hpp>
#include "Network/ClientMessage.hpp"
#include "Network/ServerMessage.hpp"
#include "Core/Engine/GameStateManager.hpp"
#include "Network/ChessClient.hpp"
#include "Client/Scenes/Game.hpp"
#include "Client/States/RoomSelectionState.hpp"


namespace states
{
    class RegisterState : public egn::GameState
    {
    public:
        void init() override;

        void cleanup() override { }
        void pause() override { }
        void resume() override { }

        void handleEvent(sf::Event& e) override;

        void update(float deltaTime) override;

        void physicsUpdate(float deltaTime) override { }

        void draw(sf::RenderTarget& target) const override;

        void initUI();

        static RegisterState* getInstance();

    private:
        mutable tgui::Gui m_gui;

        tgui::EditBox::Ptr m_editBox1;
        tgui::Button::Ptr m_button;

    protected:
        RegisterState();
    };
}