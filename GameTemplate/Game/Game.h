#pragma once

#include "Level3DRender/LevelRender.h"

//クラス宣言
class Player;
class BackGround;
class GameCamera;


class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	ModelRender m_modelRender;
	Vector3     m_pos;

	Player*     m_player = nullptr;       //プレイヤー
	BackGround* m_backGround = nullptr;   //背景
	GameCamera* m_gameCamera = nullptr;   //ゲームカメラ

};

