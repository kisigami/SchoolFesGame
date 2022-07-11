#pragma once

#include "Level3DRender/LevelRender.h"   //レベルレンダーをインクルードする

//クラス宣言
class Player;        //プレイヤークラス
class BackGround;    //背景クラス
class GameCamera;    //ゲームカメラクラス
class Fade;          //フェードクラス
class PlayerUi;      //プレイヤー関連のUIクラス
class Enemy;         //エネミークラス
class SpeedEnemy;    //スピードエネミークラス

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

private:
	//クラスを定義する
	SoundSource* m_bgm = nullptr;          //BGM
	Player*      m_player = nullptr;       //プレイヤー
	BackGround*  m_backGround = nullptr;   //背景
	GameCamera*  m_gameCamera = nullptr;   //ゲームカメラ
	Fade*        m_fade = nullptr;         //フェード
	PlayerUi*    m_playerUi = nullptr;     //プレイヤー関連のUI
	Enemy*       m_enemy = nullptr;        //エネミー
	SpeedEnemy*  m_speedEnemy = nullptr;   //スピードエネミー

	LevelRender  m_levelRender;            //レベルレンダー
};

