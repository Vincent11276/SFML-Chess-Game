#pragma once

#include <array>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>


class KeyboardInput : public sf::Keyboard
{
public:
	KeyboardInput() = delete;

	static bool isKeyPressed(Key key);
	static bool isKeyJustPressed(Key key);
	static bool isKeyReleased(Key key);

private:
	static std::array<bool, sf::Keyboard::Key::KeyCount> m_keyStates;
	static std::array<bool, sf::Keyboard::Key::KeyCount> m_pressedKeys;

	static void handleEvent(sf::Event &event);

	static void resetStates();

	friend class Game;
};