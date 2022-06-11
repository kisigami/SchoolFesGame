#include "stdafx.h"
#include "Game.h"

#include "Player.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "Enemy.h"

Game::Game()
{

}

Game::~Game()
{

}

bool Game::Start()
{
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");
	m_player = NewGO<Player>(0, "player");
	m_backGround = NewGO<BackGround>(0, "background");
	m_enemy = NewGO<Enemy>(0, "enemy");

	return true;
}

void Game::Update()
{
	// g_renderingEngine->DisableRaytracing();

}

void Game::Render(RenderContext& rc)
{

}