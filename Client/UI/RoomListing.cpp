#include "RoomListing.hpp"


namespace ui
{
    RoomWidget::RoomWidget(const char* typeName = "RoomWidget", bool initRenderer = true) :
        tgui::SubwidgetContainer(typeName, initRenderer)
    {
        try
        {
            m_container->loadWidgetsFromFile("Assets/UI/RoomListing.ui");

            m_preview_Pic = m_container->get<tgui::Picture>("Preview_Img");
            m_signalStrength_Pic = m_container->get<tgui::Picture>("SignalStrength_Img");
            m_roomName_Lbl = m_container->get<tgui::Label>("RoomName_Lbl");
            m_status_Lbl = m_container->get<tgui::Label>("Status_Lbl");
            m_playerCount_Lbl = m_container->get<tgui::Label>("PlayerCount_Lbl");
            m_background_Pnl = m_container->get<tgui::Panel>("Background_Pnl");

            m_background_Pnl->getRenderer()->setRoundedBorderRadius(5);
        }
        catch (const tgui::Exception& e)
        {
            Logger::critical("TGUI Exception: {}", e.what());
        }
        this->setSize(m_background_Pnl->getSize());
    }

    void RoomWidget::setRoomName(const std::string& name)
    {

    }

    void RoomWidget::setRoomStatus()
    {

    }

    void RoomWidget::setPreviewImage()
    {

    }

    void RoomWidget::setPlayerCount()
    {

    }

    void RoomWidget::setRoomSize()
    {

    }

    RoomWidget::Ptr RoomWidget::create()
    {
        return std::make_shared<RoomWidget>();
    }

    RoomWidget::Ptr RoomWidget::copy(RoomWidget::ConstPtr widget)
    {
        if (widget)
            return std::static_pointer_cast<RoomWidget>(widget->clone());
        else
            return nullptr;
    }

    void RoomWidget::setSize(const tgui::Layout2d& size)
    {
        SubwidgetContainer::setSize(size);
    }

    RoomWidget::Widget::Ptr RoomWidget::clone() const
    {
        return std::make_shared<RoomWidget>(*this);
    }
}