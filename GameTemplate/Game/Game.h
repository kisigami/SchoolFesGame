#pragma once

struct EnemyInformation
{
	int    s_enemyNo = 0;
	bool   s_deadFlag = true;
};

#include "Level3DRender/LevelRender.h"

//クラス宣言
class Player;        //プレイヤークラス
class BackGround;    //背景クラス
class GameCamera;    //ゲームカメラクラス
class Enemy;         //エネミークラス
class Fade;


class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	LevelRender m_levelRender;            //レベルレンダー
	Player*     m_player = nullptr;       //プレイヤー
	BackGround* m_backGround = nullptr;   //背景
	GameCamera* m_gameCamera = nullptr;   //ゲームカメラ
	Enemy*      m_enemy = nullptr;        //エネミー
	Fade*       m_fade = nullptr;
};

