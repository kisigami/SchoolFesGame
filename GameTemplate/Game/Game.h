#pragma once

#include "Level3DRender/LevelRender.h"

//�N���X�錾
class Player;
class BackGround;
class GameCamera;
class Enemy;


class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	Player*     m_player = nullptr;       //�v���C���[
	BackGround* m_backGround = nullptr;   //�w�i
	GameCamera* m_gameCamera = nullptr;   //�Q�[���J����
	Enemy* m_enemy = nullptr;
};

