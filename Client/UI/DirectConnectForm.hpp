#pragma once


#include <TGUI/TGUI.hpp>
#include "Core/Utility/Logger.hpp"

namespace ui
{
    class DirectConnectForm : public tgui::SubwidgetContainer
    {
    public:
        enum class Status
        {
            Joined,
            Canceled
        };

        typedef std::shared_ptr<DirectConnectForm> Ptr;
        typedef std::shared_ptr<const DirectConnectForm> ConstPtr;

        DirectConnectForm(const char* typeName, bool initRenderer);

        void onFinish(std::function<void(DirectConnectForm*, DirectConnectForm::Status)> callback);

        std::string getRoomName();

        std::string getPassword();

        static DirectConnectForm::Ptr create();

        static DirectConnectForm::Ptr copy(DirectConnectForm::ConstPtr widget);

        void setSize(const tgui::Layout2d& size) override;

        using tgui::SubwidgetContainer::setSize;

    private:
        std::function<void(DirectConnectForm*, DirectConnectForm::Status)> m_onFinish_Callback;

        tgui::EditBox::Ptr m_roomName_Edt;
        tgui::EditBox::Ptr m_password_Edt;
        tgui::CheckBox::Ptr m_password_Chk;
        tgui::Button::Ptr m_join_Btn;
        tgui::Button::Ptr m_cancel_Btn;

    protected:
        Widget::Ptr clone() const override;
    };
}