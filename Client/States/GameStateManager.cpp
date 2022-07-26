#include "GameStateManager.hpp"
#include "GameState.hpp"

GameStateManager::GameStateManager()
{
	// Default..
}

void GameStateManager::changeState(GameState* state)
{
	state->setGameStateManager(this);

	if (!m_states.empty()) {
		m_states.back()->cleanup(this);
		m_states.pop_back();
	}
	m_states.push_back(state);
	m_states.back()->init(this);
}

void GameStateManager::cleanUp()
{
	// cleanup the all states
	while (!m_states.empty()) {
		m_states.back()->cleanup(this);
		m_states.pop_back();
	}
}

void GameStateManager::pushState(GameState* state)
{
	state->setGameStateManager(this);

	// pause current state
	if (!m_states.empty()) {
		m_states.back()->pause(this);
	}

	// store and init the new state
	m_states.push_back(state);
	m_states.back()->init(this);
}

void GameStateManager::popState()
{
	// cleanup the current state
	if (!m_states.empty()) {
		m_states.back()->cleanup(this);
		m_states.pop_back();
	}

	// resume previous state
	if (!m_states.empty()) {
		m_states.back()->resume(this);
	}
}

void GameStateManager::handleEvent(sf::Event &event)
{
	// let the state handle events
	this->m_states.back()->handleEvent(this, event);
}

void GameStateManager::update(float deltaTime)
{
	// let the state update the game
	this->m_states.back()->update(this, deltaTime);
}

void GameStateManager::physicsUpdate(float deltaTime)
{
	// let the state physics update the game
	this->m_states.back()->physicsUpdate(this, deltaTime);
}

void GameStateManager::render(sf::RenderTarget& target)
{
	// let the state draw the screen
	this->m_states.back()->draw(this, target);
}