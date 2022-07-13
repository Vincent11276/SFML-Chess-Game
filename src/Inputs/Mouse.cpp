#include "Mouse.hpp"


std::array<bool, sf::Mouse::ButtonCount> MouseInput::m_releasedBtns { };
std::array<bool, sf::Mouse::ButtonCount> MouseInput::m_pressedBtns { };

sf::Vector2i MouseInput::getRelativePosition()
{
	return getPosition(*m_window);
}

bool MouseInput::isButtonJustPressed(Button btn)
{
	bool state = MouseInput::m_pressedBtns[btn];

	MouseInput::m_pressedBtns[btn] = false;

	return state;
}

bool MouseInput::isButtonJustReleased(Button btn)
{
	bool state = MouseInput::m_releasedBtns[btn];

	MouseInput::m_releasedBtns[btn] = false;

	return state;
}

void MouseInput::setWindow(sf::RenderWindow& window)
{
	m_window = &window;
}

void MouseInput::handleEvent(sf::Event& e)
{
	std::memset(&m_releasedBtns, false, sizeof(bool) * Button::ButtonCount);
	std::memset(&m_pressedBtns, false, sizeof(bool) * Button::ButtonCount);

	switch (e.type)
	{
	case sf::Event::MouseButtonPressed:
		m_pressedBtns[e.mouseButton.button] = true;
		break;

	case sf::Event::MouseButtonReleased:
		m_releasedBtns[e.mouseButton.button] = true;
		break;

	default:
		break;
	}
}

sf::RenderWindow* MouseInput::m_window;