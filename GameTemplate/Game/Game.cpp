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

	/*m_levelRender.Init("Assets/level3D/stage.tkl", [&](LevelObjectData& objData)
		{
			if (objData.EqualObjectName(L"background") == true)
			{*/
				m_backGround = NewGO<BackGround>(0, "background");
		/*		m_player->SetPosition(objData.position);
				return true;
			}
			else if (objData.ForwardMatchName(L"enemy") == true)
			{*/
			//	m_enemy = NewGO<Enemy>(0, "enemy");
			/*	m_enemy->SetPosition(objData.position);
				m_enemy->SetRotation(objData.rotation);
				return true;
			}
			return true;
		});*/

	return true;
}

void Game::Update()
{
	// g_renderingEngine->DisableRaytracing();
	//m_levelRender.Update();
}

void Game::Render(RenderContext& rc)
{

}