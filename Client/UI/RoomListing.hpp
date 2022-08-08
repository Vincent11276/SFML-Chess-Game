#pragma once


#include <TGUI/TGUI.hpp>
#include "Core/Utility/Logger.hpp"

namespace ui
{
    class RoomWidget : public tgui::SubwidgetContainer
    {
    public:
        typedef std::shared_ptr<RoomWidget> Ptr;
        typedef std::shared_ptr<const RoomWidget> ConstPtr;

        RoomWidget(const char* typeName, bool initRenderer);

        void setRoomName(const std::string& name);

        void setRoomStatus();

        void setPreviewImage();

        void setPlayerCount();

        void setRoomSize();

        static RoomWidget::Ptr create();

        static RoomWidget::Ptr copy(RoomWidget::ConstPtr widget);

        void setSize(const tgui::Layout2d& size) override;

        using tgui::SubwidgetContainer::setSize;

    private:
        tgui::Picture::Ptr m_preview_Pic;
        tgui::Picture::Ptr m_signalStrength_Pic;
        tgui::Label::Ptr m_roomName_Lbl;
        tgui::Label::Ptr m_status_Lbl;
        tgui::Label::Ptr m_playerCount_Lbl;
        tgui::Panel::Ptr m_background_Pnl;

    protected:
        Widget::Ptr clone() const override;
    };
}