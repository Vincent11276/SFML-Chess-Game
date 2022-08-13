#include "GameStateManager.hpp"


namespace engine
{
	GameStateManager::GameStateManager()
	{
		// Default..
	}

	void GameStateManager::changeState(GameState* state)
	{
		if (!m_states.empty()) {
			m_states.back()->cleanup();
			m_states.pop_back();
		}
		m_states.push_back(state);
		m_states.back()->init();
	}

	void GameStateManager::cleanUp()
	{
		// cleanup the all states
		while (!m_states.empty()) {
			m_states.back()->cleanup();
			m_states.pop_back();
		}
	}

	void GameStateManager::pushState(GameState* state)
	{
		// pause current state
		if (!m_states.empty()) {
			m_states.back()->pause();
		}

		// store and init the new state
		m_states.push_back(state);
		m_states.back()->init();
	}

	void GameStateManager::popState()
	{
		// cleanup the current state
		if (!m_states.empty()) {
			m_states.back()->cleanup();
			m_states.pop_back();
		}

		// resume previous state
		if (!m_states.empty()) {
			m_states.back()->resume();
		}
	}

	void GameStateManager::handleEvent(sf::Event& event)
	{
		// let the state handle events
		this->m_states.back()->handleEvent(event);
	}

	void GameStateManager::update(float deltaTime)
	{
		// let the state update the game
		this->m_states.back()->update(deltaTime);
	}

	void GameStateManager::physicsUpdate(float deltaTime)
	{
		// let the state physics update the game
		this->m_states.back()->physicsUpdate(deltaTime);
	}

	void GameStateManager::render(sf::RenderTarget& target)
	{
		// let the state draw the screen
		this->m_states.back()->draw(target);
	}

	GameStateManager* GameStateManager::getInstance()
	{
		static GameStateManager instance;

		return &instance;
	}
}