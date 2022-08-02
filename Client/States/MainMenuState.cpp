#include "MainMenuState.hpp"

#include "Client/States/InGameState.hpp"
#include "TGUI/Widgets/BitmapButton.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/HorizontalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/VerticalLayout.hpp"


void MainMenuState::init(GameStateManager* game)
{
    this->initUI();
}

void MainMenuState::cleanup(GameStateManager* game) { }
void MainMenuState::pause(GameStateManager* game) { }
void MainMenuState::resume(GameStateManager* game) { }

void MainMenuState::handleEvent(GameStateManager* manager, sf::Event& e)
{
    gui.handleEvent(e);
}

void MainMenuState::update(GameStateManager* manager, float deltaTime) { }
void MainMenuState::physicsUpdate(GameStateManager* manager, float deltaTime) { }

void MainMenuState::draw(GameStateManager* manager, sf::RenderTarget& target) const 
{ 
    gui.draw();
}

MainMenuState* MainMenuState::getInstance(sf::RenderWindow *window)
{
    static MainMenuState mainMenuState;
    mainMenuState.m_window = window;

    return &mainMenuState;
}

void MainMenuState::initUI()
{
    gui.setTarget(*m_window);

    try
    {
        gui.loadWidgetsFromFile("Assets/form.txt");

        auto playBtn = gui.get<tgui::Button>("Play Button");
        playBtn->onPress(&MainMenuState::on_PlayBtn_Pressed, this);

        auto exitBtn = gui.get<tgui::Button>("Exit Button");
        exitBtn->onPress(&MainMenuState::on_ExitBtn_Pressed, this);
    }
    catch (const tgui::Exception& e)
    {
        std::cerr << "TGUI Exception: " << e.what() << std::endl;
    }
}
void MainMenuState::on_PlayBtn_Pressed()
{
    std::cout << "You have pressed the play button" << std::endl;

    this->getGameStateManager()->changeState(InGameState::getInstance());
}

void MainMenuState::on_SettingsBtn_pressed()
{

}

void MainMenuState::on_OnlineBtn_Pressed()
{

}

void MainMenuState::on_ExitBtn_Pressed()
{
    
}

MainMenuState::MainMenuState()
{
    
}
