#pragma once

#include "Level3DRender/LevelRender.h"   //レベルレンダーをインクルードする

//クラス宣言
class Player;        //プレイヤークラス
class BackGround;    //背景クラス
class GameCamera;    //ゲームカメラクラス
class Fade;          //フェードクラス
class PlayerUi;      //プレイヤー関連のUIクラス
class SpeedEnemy;    //スピードエネミークラス
class SpawnEnemy;
class GameUi;
class Result;

/// <summary>
/// ゲームクラス
/// </summary>
class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	const float& GetGameTime() const
	{
		return m_gameTimer;
	}
private:
	enum GameState
	{
		enGameState_End,
		enGameState_Idle,
	};
	void GameTime();
	void NotifyGameEnd();
	//クラスを定義する
	GameState m_gameState = enGameState_Idle;
	SoundSource* m_bgm = nullptr;          //BGM
	Player*      m_player = nullptr;       //プレイヤー
	BackGround*  m_backGround = nullptr;   //背景
	GameCamera*  m_gameCamera = nullptr;   //ゲームカメラ
	Fade*        m_fade = nullptr;         //フェード
	PlayerUi*    m_playerUi = nullptr;     //プレイヤー関連のUI
	SpeedEnemy*  m_speedEnemy = nullptr;   //スピードエネミー
	SkyCube*     m_skyCube = nullptr;
	SpawnEnemy*  m_spawnEnemy = nullptr;
	GameUi* m_gameUi = nullptr;
	Result* m_result = nullptr;

	LevelRender  m_levelRender;            //レベルレンダー
	FPSLimitter m_fpsLimitter;

	FontRender m_fontRender;
	FontRender m_fontRender2;
	float m_gameTimer = 60.0f;
	bool m_isFadeOutWait = false;
};

