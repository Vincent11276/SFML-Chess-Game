#include "MainMenuState.hpp"

#include "TGUI/Widgets/HorizontalLayout.hpp"
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
        auto verticalLayout = tgui::VerticalLayout::create();
        verticalLayout->setSize(250, 250);
        verticalLayout->setPosition("50%", "50%");
        verticalLayout->setOrigin(0.5, 0.5);
        gui.add(verticalLayout);

        auto playBtn = tgui::Button::create();
        playBtn->setText("Play");
        playBtn->onPress(&MainMenuState::on_PlayBtn_Pressed, this);
        verticalLayout->add(playBtn, "playBtn");

        auto onlineBtn = tgui::Button::create();
        onlineBtn->setText("Multiplayer");
        onlineBtn->onPress(&MainMenuState::on_OnlineBtn_Pressed, this);
        verticalLayout->add(onlineBtn, "onlineBtn");

        auto settingsBtn = tgui::Button::create();
        settingsBtn->setText("Settings");
        settingsBtn->onPress(&MainMenuState::on_SettingsBtn_pressed, this);
        verticalLayout->add(settingsBtn, "settingsBtn");

        auto exitBtn = tgui::Button::create();
        exitBtn->setText("Quit");
        exitBtn->onPress(&MainMenuState::on_ExitBtn_Pressed, this);
        verticalLayout->add(exitBtn, "exitBtn");
    }
    catch (const tgui::Exception& e)
    {
        std::cerr << "TGUI Exception: " << e.what() << std::endl;
    }
}
void MainMenuState::on_PlayBtn_Pressed()
{
    std::cout << "You have pressed the play button" << std::endl;

    
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

MainMenuState::MainMenuState() = default;
