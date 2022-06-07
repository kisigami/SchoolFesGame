#include "stdafx.h"
#include "Game.h"

#include "Player.h"
#include "BackGround.h"
#include "GameCamera.h"

bool Game::Start()
{
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");
	m_player = NewGO<Player>(0, "player");
	m_backGround = NewGO<BackGround>(0, "background");

	m_modelRender.Init("Assets/modelData/player/Demo.tkm");
	Vector3 pos = { 0.0f,0.0f,300.0f };
	m_modelRender.SetPosition(pos);

	return true;
}

void Game::Update()
{
	// g_renderingEngine->DisableRaytracing();
	m_modelRender.Update();
}

void Game::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}