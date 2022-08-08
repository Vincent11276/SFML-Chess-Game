#include "DirectConnectForm.hpp"


namespace ui
{
    DirectConnectForm::DirectConnectForm(const char* typeName = "DirectConnectForm", bool initRenderer = true) :
        tgui::SubwidgetContainer(typeName, initRenderer)
    {
        try
        {
            m_container->loadWidgetsFromFile("Assets/UI/DirectConnectForm.ui");

            m_roomName_Edt = m_container->get<tgui::EditBox>("RoomName_Edt");
            m_password_Edt = m_container->get<tgui::EditBox>("Password_Edt");
            m_password_Chk = m_container->get<tgui::CheckBox>("Password_Chk");
            m_join_Btn = m_container->get<tgui::Button>("Join_Btn");
            m_cancel_Btn = m_container->get<tgui::Button>("Cancel_Btn");

            m_join_Btn->onPress([&] {
                m_onFinish_Callback(this, Status::Joined);
            });

            m_cancel_Btn->onPress([&] {
                m_onFinish_Callback(this, Status::Canceled);
            });
        }
        catch (const tgui::Exception& e)
        {
            Logger::critical("TGUI Exception: {}", e.what());
        }
        this->setSize("100%", "100%");
    }

    std::string DirectConnectForm::getRoomName()
    {
        return std::string(m_roomName_Edt->getText());
    }

    std::string DirectConnectForm::getPassword()
    {
        return std::string(m_password_Edt->getText());
    }

    void DirectConnectForm::onFinish(std::function<void(DirectConnectForm*, DirectConnectForm::Status)> callback)
    {
        m_onFinish_Callback = callback;
    }

    DirectConnectForm::Ptr DirectConnectForm::create()
    {
        return std::make_shared<DirectConnectForm>();
    }

    DirectConnectForm::Ptr DirectConnectForm::copy(DirectConnectForm::ConstPtr widget)
    {
        if (widget)
            return std::static_pointer_cast<DirectConnectForm>(widget->clone());
        else
            return nullptr;
    }

    void DirectConnectForm::setSize(const tgui::Layout2d& size)
    {
        SubwidgetContainer::setSize(size);
    }

    DirectConnectForm::Widget::Ptr DirectConnectForm::clone() const
    {
        return std::make_shared<DirectConnectForm>(*this);
    }
}