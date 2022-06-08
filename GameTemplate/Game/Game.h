#pragma once

#include "Level3DRender/LevelRender.h"

//�N���X�錾
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

	Player*     m_player = nullptr;       //�v���C���[
	BackGround* m_backGround = nullptr;   //�w�i
	GameCamera* m_gameCamera = nullptr;   //�Q�[���J����

};

