#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include <array>


class MouseInput : public sf::Mouse
{
public:
	MouseInput() = delete;

	static sf::Vector2i getRelativePosition();

	static bool isButtonJustPressed(Button button);
	static bool isButtonJustReleased(Button button);

private:
	static sf::RenderWindow* m_window;

	static std::array<bool, Button::ButtonCount> m_releasedBtns;
	static std::array<bool, Button::ButtonCount> m_pressedBtns;

	static void setWindow(sf::RenderWindow& window);

	static void handleEvent(sf::Event& e);

	friend class Game;
};
