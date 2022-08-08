#include "CreateRoomForm.hpp"
#include "Core/Utility/Logger.hpp"

namespace ui
{
    CreateRoomForm::CreateRoomForm(const char* typeName = "CreateRoomForm", bool initRenderer = true) :
        tgui::SubwidgetContainer(typeName, initRenderer)
    {
        try
        {
            m_container->loadWidgetsFromFile("Assets/UI/CreateRoomForm.ui");

            m_roomName_Edt = m_container->get<tgui::EditBox>("RoomName_Edt");
            m_password_Edt = m_container->get<tgui::EditBox>("Password_Edt");
            m_gameMode_Cmb = m_container->get<tgui::ComboBox>("GameMode_Cmb");
            m_duration_Cmb = m_container->get<tgui::ComboBox>("Duration_Cmb");
            m_create_Btn = m_container->get<tgui::Button>("Create_Btn");
            m_cancel_Btn = m_container->get<tgui::Button>("Cancel_Btn");
      
            m_create_Btn->onPress([&] {
                m_onFinish_Callback(this, CreateRoomForm::Status::Created);
            });

            m_cancel_Btn->onPress([&] {
                m_onFinish_Callback(this, CreateRoomForm::Status::Canceled);
            });
        }
        catch (const tgui::Exception& e)
        {
            Logger::critical("TGUI Exception: {}", e.what());
        }


        this->setSize("100%", "100%");
    }

    void CreateRoomForm::onFinish(std::function<void(CreateRoomForm*, CreateRoomForm::Status)> callback)
    {
        m_onFinish_Callback = callback;
    }

    std::string CreateRoomForm::getRoomName()
    {
        return std::string(m_roomName_Edt->getText());
    }

    std::string CreateRoomForm::getPassword()
    {
        return std::string(m_password_Edt->getText());
    }

    GameMode CreateRoomForm::getMode()
    {
        switch (m_gameMode_Cmb->getSelectedItemIndex())
        {
        case 0:
            return GameMode::Duo;

        case 1:
            return GameMode::Quad;

        default:
            throw std::invalid_argument("Invalid ComboBox Index");
        }
    }

    MatchDuration CreateRoomForm::getDuration()
    {
        switch (m_gameMode_Cmb->getSelectedItemIndex())
        {
        case 0:
            return MatchDuration::OneMinute;

        case 1:
            return MatchDuration::FiveMinutes;

        case 2:
            return MatchDuration::TenMinutes;

        case 3:
            return MatchDuration::FifteenMinutes;

        case 4:
            return MatchDuration::ThirtyMinutes;

        default:
            throw std::invalid_argument("Invalid ComboBox Index");
        }
    }

    CreateRoomForm::Ptr CreateRoomForm::create()
    {
        return std::make_shared<CreateRoomForm>();
    }

    CreateRoomForm::Ptr CreateRoomForm::copy(CreateRoomForm::ConstPtr widget)
    {
        if (widget)
            return std::static_pointer_cast<CreateRoomForm>(widget->clone());
        else
            return nullptr;
    }

    void CreateRoomForm::setSize(const tgui::Layout2d& size)
    {
        SubwidgetContainer::setSize(size);
    }

    CreateRoomForm::Widget::Ptr CreateRoomForm::clone() const
    {
        return std::make_shared<CreateRoomForm>(*this);
    }
}