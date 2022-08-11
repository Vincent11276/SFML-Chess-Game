#include "RoomSelectionState.hpp"


RoomSelectionState::RoomSelectionState()
{
    this->initUI();
}


void RoomSelectionState::init()
{
    Logger::debug("RoomSelectionState has been initialized");

    ChessClient::getInstance().registerCallback(ServerMessage::Type::FetchedAvailableRooms, [&](ServerMessage* messsage) {
        auto& content = messsage->getContent<ServerMessage::FetchedAvailableRooms>();
        Logger::info("Fetched {} rooms from the server", content.roomListing.size());
    });

    ChessClient::getInstance().registerCallback(ServerMessage::Type::CreateRoomSuccess, [&](ServerMessage* messsage){
        Logger::info("Succesfully created room to the server");

        GameStateManager::getInstance()->changeState(InOnlineGameState::getInstance());
    });

    ChessClient::getInstance().registerCallback(ServerMessage::Type::CreateRoomFailed, [&](ServerMessage* messsage) {
        Logger::error("Failed created room to the server");
    });

    ChessClient::getInstance().registerCallback(ServerMessage::Type::JoinExistingRoomSuccess, [&](ServerMessage* messsage) {
        Logger::info("Succesfuly joined the designated room");

        GameStateManager::getInstance()->changeState(InOnlineGameState::getInstance());
    });

    ChessClient::getInstance().registerCallback(ServerMessage::Type::JoinExistingRoomFailed, [&](ServerMessage* messsage) {
        Logger::error("Failed to join the designated room");
    });
}

void RoomSelectionState::handleEvent(sf::Event& e)
{
    m_gui.handleEvent(e);
}

void RoomSelectionState::draw(sf::RenderTarget& target) const
{
    m_gui.draw();
}

RoomSelectionState* RoomSelectionState::getInstance()
{
    static RoomSelectionState newInstance;

    return &newInstance;
}

void RoomSelectionState::initUI()
{
    try
    {
        m_gui.loadWidgetsFromFile("Assets/UI/RoomSelection.ui");
        m_gui.setTarget(Game::getInstance()->getWindow());

        auto directConnect_Btn = m_gui.get<tgui::Button>("DirectConnect_Btn");
        auto createRoom_Btn = m_gui.get<tgui::Button>("CreateRoom_Btn");
        auto joinRoom_Btn = m_gui.get<tgui::Button>("JoinRoom_Btn");
        auto refresh_Btn = m_gui.get<tgui::Button>("Refresh_Btn");
        auto cancel_Btn = m_gui.get<tgui::Button>("Cancel_Btn");
        auto scrollablePanel1 = m_gui.get<tgui::ScrollablePanel>("ScrollablePanel1");

        directConnect_Btn->onPress(&RoomSelectionState::on_directConnect_Btn_Pressed, this);
        createRoom_Btn->onPress(&RoomSelectionState::on_createRoom_Btn_Pressed, this);
        joinRoom_Btn->onPress(&RoomSelectionState::on_joinRoom_Btn_Pressed, this);
        refresh_Btn->onPress(&RoomSelectionState::on_refresh_Btn_Pressed, this);
        cancel_Btn->onPress(&RoomSelectionState::on_cancel_Btn_Pressed, this);

        auto row1 = tgui::HorizontalLayout::create();
        //row1->setSize(100%, 100);
        scrollablePanel1->add(row1);

        auto a = ui::RoomWidget::create();
        a->setPosition("0%");
        row1->add(a);

        auto b = ui::RoomWidget::create();
        b->setPosition("33.33%");
        row1->add(b);

        auto c = ui::RoomWidget::create();
        b->setPosition("66.66%");
        row1->add(c);
    }
    catch (tgui::Exception e)
    {
        Logger::critical("TGUI Exception: {}", e.what());
    }
}

void RoomSelectionState::on_directConnect_Btn_Pressed()
{
    // Make sure only one instance of the widget will be created
    if (m_gui.get<ui::DirectConnectForm>("directConnectForm") == nullptr)
    {
        auto directConnectForm = ui::DirectConnectForm::create();
        m_gui.add(directConnectForm, "directConnectForm");

        directConnectForm->onFinish([&](ui::DirectConnectForm* form, ui::DirectConnectForm::Status status) {
            if (status == ui::DirectConnectForm::Status::Joined)
            {
                std::string roomName = form->getRoomName();
                std::string password = form->getPassword();

                ChessClient::getInstance().joinExistingRoom(roomName, password);
            }
            m_gui.remove(m_gui.get<ui::DirectConnectForm>("directConnectForm"));
        });
    }
}

void RoomSelectionState::on_createRoom_Btn_Pressed()
{
    // Make sure only one instance of the widget will be created
    if (m_gui.get<ui::CreateRoomForm>("createRoomForm") == nullptr)
    {
        auto createRoomForm = ui::CreateRoomForm::create();
        m_gui.add(createRoomForm, "createRoomForm");

        createRoomForm->onFinish([&](ui::CreateRoomForm* form, ui::CreateRoomForm::Status status) {
            if (status == ui::CreateRoomForm::Status::Created)
            {
                ClientMessage::CreateNewRoom newRoom{
                    .name = form->getRoomName(),
                    .password = form->getPassword(),
                    .mode = form->getMode(),
                    .duration = form->getDuration()
                };
                ChessClient::getInstance().createNewRoom(newRoom);
            }
            m_gui.remove(m_gui.get<ui::CreateRoomForm>("createRoomForm"));
        });
    }
}

void RoomSelectionState::on_joinRoom_Btn_Pressed()
{
    Logger::info("You have pressed join room button");
}

void RoomSelectionState::on_refresh_Btn_Pressed()
{
    ChessClient::getInstance().fetchAvailableRooms();
}

void RoomSelectionState::on_cancel_Btn_Pressed()
{
    Logger::info("You have pressed cancel button");

    GameStateManager::getInstance()->changeState(MainMenuState::getInstance());
}

void RoomSelectionState::updateRoomListing(const std::vector<ServerMessage::FetchedAvailableRooms::RoomListing>& listing)
{
    auto status_Lbl = m_gui.get<tgui::Label>("Status_Lbl");

    if (listing.size() != 0)
    {
        status_Lbl->setVisible(false);

        for (const auto& room : listing)
        {

        }
    }
    else status_Lbl->setVisible(true);
}
