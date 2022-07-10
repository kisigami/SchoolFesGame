#include "stdafx.h"
#include "Game.h"

#include "Player.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "Enemy.h"
#include "Fade.h"

#include "SpeedEnemy.h"
#include "PlayerUi.h"

Game::Game()
{

}

Game::~Game()
{
	DeleteGO(m_gameCamera);
	DeleteGO(m_player);
	DeleteGO(m_backGround);
}

bool Game::Start()
{
	//ゲームカメラを作成する
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");
	//プレイヤーを作成する
	m_player = NewGO<Player>(0, "player");
	//プレイヤーのUIを作成する
	m_playerUi = NewGO<PlayerUi>(0,"playerui");

	m_speedEnemy = NewGO<SpeedEnemy>(0, "speedenemy");

	//ステージのレベルの読み込み
	m_levelRender.Init("Assets/level3D/stage.tkl", [&](LevelObjectData& objData)
		{
			//オブジェクトの名前が「background」だったら
			if (objData.EqualObjectName(L"background") == true)
			{
				//背景を作成する
				m_backGround = NewGO<BackGround>(0, "background");
				//座標を設定する
				m_backGround->SetPosition(objData.position);
				return true;
			}

			//オブジェクトの名前が「enemy」だったら
			else if (objData.ForwardMatchName(L"enemy") == true)
			{
				//エネミーを作成する
				auto enemy = NewGO<Enemy>(0, "enemy");
				//座標を設定する
				enemy->SetPosition(objData.position);
				//番号を設定する
				int number = _wtoi(&objData.name[5]);
				enemy->SetMyNumber(number);
				return true;
			}
			return true;
		});

	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/bgm.wav");
	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(0);
	m_bgm->Play(true);
	m_bgm->SetVolume(0.2f);
	return true;
}

void Game::Update()
{
	// g_renderingEngine->DisableRaytracing();
	//レベルの更新
	m_levelRender.Update();
}

void Game::Render(RenderContext& rc)
{
	//描画
	m_levelRender.Draw(rc);
}