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
    if (netw::ChessClient::getInstance().session.isRegistered)
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
    try
    {
        m_gui.loadWidgetsFromFile("Assets/UI/RegistrationForm.ui");

        m_editBox1 = m_gui.get<tgui::EditBox>("EditBox1");
        m_button = m_gui.get<tgui::Button>("Button1");

        m_button->onPress([&] {
            netw::ChessClient::getInstance().registerPlayer(m_editBox1->getText().toStdString());
        });
    }
    catch (tgui::Exception e)
    {
        Logger::critical("TGUI Exception: {}", e.what());
    }
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
