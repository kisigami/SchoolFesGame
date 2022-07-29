#include "stdafx.h"
#include "Game.h"

#include "Player.h"
#include "BackGround.h"
#include "GameCamera.h"

#include "Fade.h"
#include "PlayerUi.h"
#include "GameUi.h"
#include "Result.h"

#include "SpeedEnemy.h"
#include "nature/SkyCube.h"

#include "time/FPSLimitter.h"
#include "SpawnEnemy.h"

namespace
{
	const int   BGM_NUMBER = 0;      //BGMの番号
	const float BGM_VOLUME = 0.1f;   //BGMのサウンドボリューム

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
	//UIを削除する
	DeleteGO(m_playerUi);
	DeleteGO(m_gameUi);

	DeleteGO(m_skyCube);
	DeleteGO(m_spawnEnemy);
	DeleteGO(m_bgm);
}

bool Game::Start()
{
	m_fpsLimitter.SetMaxFPS(35);
	//スカイキューブとか環境光とかいろいろ
	DeleteGO(m_skyCube);
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetType((EnSkyCubeType)enSkyCubeType_Day);
	Vector3 dir = { 1, -1, 1 };
	dir.Normalize();
	g_renderingEngine->SetDirectionLight(
		0, 
		dir, 
		{20.0f, 20.0f, 20.0f}
	);
	m_skyCube->SetLuminance(10.0f);
	// 環境光の計算のためのIBLテクスチャをセットする。
	g_renderingEngine->SetAmbientByIBLTexture(m_skyCube->GetTextureFilePath(), 0.8f);

	g_renderingEngine->SetCascadeNearAreaRates(0.02f, 0.1f, 0.7f);
	g_renderingEngine->SetAmbient(Vector3::One * 0.1f);

	//ゲームカメラを作成する
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");
	g_camera3D->SetFar(20000.0f);
	//プレイヤーを作成する
	m_player = NewGO<Player>(0, "player");
	m_player = FindGO<Player>("player");
	//UIを作成する
	m_playerUi = NewGO<PlayerUi>(0, "playerui");
	m_gameUi = NewGO<GameUi>(0, "gameui");
	m_result = FindGO<Result>("result");
	m_spawnEnemy = NewGO<SpawnEnemy>(0, "spawnenemy");
	m_backGround = NewGO<BackGround>(0, "background");
	//BGMを読み込む
	g_soundEngine->ResistWaveFileBank(BGM_NUMBER, "Assets/sound/bgm.wav");
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
	GameTime();

	if (m_player->GetHP() <= 0)
	{
		NotifyGameEnd();
	}

	if (m_gameState == enGameState_End)
	{
		if (m_isFadeOutWait) {
			if (!m_fade->IsFade()) {
				m_result = NewGO<Result>(0, "result");
				m_result->SetScore(m_gameUi->GetScore());
				DeleteGO(this);
			}
		}
		else {
			m_isFadeOutWait = true;
			m_fade->StartFadeOut();
		}
	}
	m_levelRender.Update();
}

void Game::GameTime()
{
	m_gameTimer -= g_gameTime->GetFrameDeltaTime();
	
	if (m_gameTimer <= 0.0f)
	{
		m_gameTimer = 0.0f;
		//ゲームの終わりを通知する
		NotifyGameEnd();
	}
}

void Game::NotifyGameEnd()
{
	m_gameState = enGameState_End;
}

void Game::Render(RenderContext& rc)
{
	m_levelRender.Draw(rc);
}