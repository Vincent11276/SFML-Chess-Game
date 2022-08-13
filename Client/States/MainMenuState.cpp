#include "MainMenuState.hpp"


namespace states
{
    void MainMenuState::init()
    {
        Logger::debug("MainMenuState has been initialized");

        this->initUI();
    }

    void MainMenuState::handleEvent(sf::Event& e)
    {
        gui.handleEvent(e);
    }

    void MainMenuState::draw(sf::RenderTarget& target) const
    {
        gui.draw();
    }

    MainMenuState* MainMenuState::getInstance()
    {
        static MainMenuState mainMenuState;

        return &mainMenuState;
    }

    void MainMenuState::initUI()
    {
        gui.setTarget(Game::getInstance()->getWindow());

        try
        {
            gui.loadWidgetsFromFile("Assets/UI/MainMenu.ui");

            auto playBtn = gui.get<tgui::Button>("Play Button");
            playBtn->onPress(&MainMenuState::on_PlayBtn_Pressed, this);

            auto onlineBtn = gui.get<tgui::Button>("Online Button");
            onlineBtn->onPress(&MainMenuState::on_OnlineBtn_Pressed, this);

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

        engine::GameStateManager::getInstance()->changeState(InOfflineGameState::getInstance());
    }

    void MainMenuState::on_SettingsBtn_pressed()
    {
        netw::ChessClient::getInstance().session.isRegistered = true;
    }

    void MainMenuState::on_OnlineBtn_Pressed()
    {
        // Check if player is connected to the server
        if (!netw::ChessClient::getInstance().isConnected())
        {
            Logger::info("You are not connected to the server! Check your internet connection and try again.");

            // Attempt to reconnect
            netw::ChessClient::getInstance().connect();
        }

        // Check if client is authenticated otherwise try it (By default, program tries to authenticate as it starts)
        else if (!netw::ChessClient::getInstance().session.isAuthenticated)
        {
            std::cout << "You are not yet authenticated! Trying to authenticate you to the server.." << std::endl;

            netw::ChessClient::getInstance().authenticate();
        }

        // Check if player is registered or not
        else if (!netw::ChessClient::getInstance().session.isRegistered)
        {
            Logger::info("You are not yet registered! Please identify yourself");

            engine::GameStateManager::getInstance()->changeState(RegisterState::getInstance());
        }

        // If there's no exception, continue to the next game state
        else engine::GameStateManager::getInstance()->changeState(RoomSelectionState::getInstance());
    }

    void MainMenuState::on_ExitBtn_Pressed()
    {
        Game::getInstance()->getWindow().close();
    }

    MainMenuState::MainMenuState() = default;
}