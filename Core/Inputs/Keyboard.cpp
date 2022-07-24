#include "Keyboard.hpp"


std::array<bool, sf::Keyboard::Key::KeyCount> KeyboardInput::m_keyStates { };
std::array<bool, sf::Keyboard::Key::KeyCount> KeyboardInput::m_pressedKeys { };

bool KeyboardInput::isKeyPressed(Key key)
{
	return m_keyStates[key];
}

bool KeyboardInput::isKeyJustPressed(Key key)
{
	return m_pressedKeys[key];
}

bool KeyboardInput::isKeyReleased(Key key)
{
	return !m_pressedKeys[key];
}

void KeyboardInput::handleEvent(sf::Event &e)
{
	switch (e.type)
	{
	case sf::Event::KeyPressed:
		m_keyStates[e.key.code] = true;
		m_pressedKeys[e.key.code] = true;
		break;

	case sf::Event::KeyReleased:
		m_keyStates[e.key.code] = false;
		break;

	default:
		break;
	}
}

void KeyboardInput::resetStates()
{
	std::memset(&m_pressedKeys, false, sizeof(bool) * Key::KeyCount);
}
