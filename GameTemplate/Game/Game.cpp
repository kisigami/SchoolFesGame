#include "stdafx.h"
#include "Game.h"

#include "Player.h"

bool Game::Start()
{
	m_player = NewGO<Player>(0, "player");

	return true;
}

void Game::Update()
{
	// g_renderingEngine->DisableRaytracing();
}

void Game::Render(RenderContext& rc)
{

}