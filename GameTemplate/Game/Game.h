#pragma once

struct EnemyInformation
{
	int    s_enemyNo = 0;
	bool   s_deadFlag = true;
};

#include "Level3DRender/LevelRender.h"

//�N���X�錾
class Player;        //�v���C���[�N���X
class BackGround;    //�w�i�N���X
class GameCamera;    //�Q�[���J�����N���X
class Enemy;         //�G�l�~�[�N���X
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
	LevelRender m_levelRender;            //���x�������_�[
	Player*     m_player = nullptr;       //�v���C���[
	BackGround* m_backGround = nullptr;   //�w�i
	GameCamera* m_gameCamera = nullptr;   //�Q�[���J����
	Enemy*      m_enemy = nullptr;        //�G�l�~�[
	Fade*       m_fade = nullptr;
};

