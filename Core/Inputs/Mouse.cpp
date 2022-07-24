#include "Mouse.hpp"


std::array<bool, sf::Mouse::ButtonCount> MouseInput::m_buttonStates { };
std::array<bool, sf::Mouse::ButtonCount> MouseInput::m_justPressedBtns { };
std::array<bool, sf::Mouse::ButtonCount> MouseInput::m_justReleasedBtns { };

void MouseInput::setWindow(sf::RenderWindow& window)
{
	m_window = &window;
}

sf::Vector2i MouseInput::getRelativePosition()
{
	return getPosition(*m_window);
}

bool MouseInput::isButtonJustPressed(Button btn)
{
	return MouseInput::m_justPressedBtns[btn];
}

bool MouseInput::isButtonJustReleased(Button btn)
{
	return MouseInput::m_justReleasedBtns[btn];
}


void MouseInput::handleEvent(sf::Event& e)
{
	switch (e.type)
	{
	case sf::Event::MouseButtonPressed:
		m_buttonStates[e.mouseButton.button] = true;
		m_justPressedBtns[e.mouseButton.button] = true;
		break;

	case sf::Event::MouseButtonReleased:
		m_buttonStates[e.mouseButton.button] = true;
		m_justReleasedBtns[e.mouseButton.button] = true;
		break;

	default:
		break;
	}
}

void MouseInput::resetStates()
{
	std::memset(&m_justPressedBtns, false, sizeof(bool) * Button::ButtonCount);
	std::memset(&m_justReleasedBtns, false, sizeof(bool) * Button::ButtonCount);
}


sf::RenderWindow* MouseInput::m_window;