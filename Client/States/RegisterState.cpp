#include "RegisterState.hpp"


void RegisterState::init()
{
    Logger::debug("RegisterState has been initialized");

    this->initUI();
}

void RegisterState::handleEvent(sf::Event& e)
{
    m_gui.handleEvent(e);
}

void RegisterState::update(float deltaTime)
{
    if (ChessClient::getInstance().session.isRegistered)
    {
        // Proceed to next game state
        GameStateManager::getInstance()->changeState(RoomSelectionState::getInstance());
    }

}

void RegisterState::draw(sf::RenderTarget& target) const
{
    m_gui.draw();
}

void RegisterState::initUI()
{
    m_gui.loadWidgetsFromFile("Assets/UI/RegistrationForm.ui");

    auto editBox1 = m_gui.get<tgui::EditBox>("EditBox1");

    auto button = m_gui.get<tgui::Button>("Button1");
    button->onPress([&] {
        ChessClient::getInstance().registerPlayer("Vincent");
    });
}

RegisterState* RegisterState::getInstance()
{
    static RegisterState instance;

    return &instance;
}

RegisterState::RegisterState()
{
    m_gui.setTarget(Game::getInstance()->getWindow());
}
