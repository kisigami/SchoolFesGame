#include "stdafx.h"
#include "Game.h"

#include "Player.h"
#include "BackGround.h"
#include "GameCamera.h"

#include "Fade.h"
#include "PlayerUi.h"

#include "Enemy.h"
#include "SpeedEnemy.h"

namespace
{
	const int   BGM_NUMBER = 0;      //BGMの番号
	const float BGM_VOLUME = 0.2f;   //BGMのサウンドボリューム

}
Game::Game()
{
}

Game::~Game()
{
	//カメラを削除する
	DeleteGO(m_gameCamera);
	//プレイヤーを削除する
	DeleteGO(m_player);
	//背景を削除する
	DeleteGO(m_backGround);
	//プレイヤー関連のUIを削除する
	DeleteGO(m_playerUi);
}

bool Game::Start()
{
	//ゲームカメラを作成する
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");
	//プレイヤーを作成する
	m_player = NewGO<Player>(0, "player");
	//プレイヤーのUIを作成する
	m_playerUi = NewGO<PlayerUi>(0,"playerui");
	//スピードエネミーを作成する
	m_speedEnemy = NewGO<SpeedEnemy>(0, "speedenemy");

	//BGMを読み込む
	g_soundEngine->ResistWaveFileBank(BGM_NUMBER, "Assets/sound/bgm.wav");

	////ステージのレベルの読み込み
	m_levelRender.Init("Assets/level3D/stage.tkl", [&](LevelObjectData& objData)
		{
			//オブジェクトの名前が「background」だったら
			if (objData.EqualObjectName(L"background") == true)
			{
				//背景を作成する
				m_backGround = NewGO<BackGround>(0, "background");
				//座標を設定する
				m_backGround->SetPosition(objData.position);
				m_backGround->SetScale(objData.scale);
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

	//フェードのインスタンスを探す
	m_fade = FindGO<Fade>("fade");
	//フェードインを開始する
	m_fade->StartFadeIn();

	//BGMを作成する
	m_bgm = NewGO<SoundSource>(0);
	//BGMを初期化する
	m_bgm->Init(BGM_NUMBER);
	//BGMを再生する（ループする）
	m_bgm->Play(true);
	//BGMの大きさを設定する
	m_bgm->SetVolume(BGM_VOLUME);
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