#pragma once


#include <TGUI/TGUI.hpp>
#include "Core/Misc/GameMode.hpp"
#include "Core/Misc/MatchDuration.hpp"


namespace ui
{
    class CreateRoomForm : public tgui::SubwidgetContainer
    {
    public:   
        enum class Status
        {
            Canceled,
            Created
        };

        typedef std::shared_ptr<CreateRoomForm> Ptr;
        typedef std::shared_ptr<const CreateRoomForm> ConstPtr;

        CreateRoomForm(const char* typeName, bool initRenderer);

        void onFinish(std::function<void(CreateRoomForm*, CreateRoomForm::Status)> callback);

        std::string getRoomName();
        
        std::string getPassword();

        GameMode getMode();

        MatchDuration getDuration();

        static CreateRoomForm::Ptr create();

        static CreateRoomForm::Ptr copy(CreateRoomForm::ConstPtr widget);

        void setSize(const tgui::Layout2d& size) override;

        using tgui::SubwidgetContainer::setSize;

    private:
        std::function<void(CreateRoomForm*, CreateRoomForm::Status)> m_onFinish_Callback;

        tgui::EditBox::Ptr m_roomName_Edt;
        tgui::EditBox::Ptr m_password_Edt;
        tgui::ComboBox::Ptr m_gameMode_Cmb;
        tgui::ComboBox::Ptr m_duration_Cmb;
        tgui::Button::Ptr m_create_Btn;
        tgui::Button::Ptr m_cancel_Btn;
        
    protected:
        Widget::Ptr clone() const override;
    };
}