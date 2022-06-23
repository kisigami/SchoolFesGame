#include "stdafx.h"
#include "Game.h"

#include "Player.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "Enemy.h"

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
	//�Q�[���J�������쐬����
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");

	//�v���C���[���쐬����
	m_player = NewGO<Player>(0, "player");

	//�X�e�[�W�̃��x���̓ǂݍ���
	m_levelRender.Init("Assets/level3D/stage.tkl", [&](LevelObjectData& objData)
		{
			//�I�u�W�F�N�g�̖��O���ubackground�v��������
			if (objData.EqualObjectName(L"background") == true)
			{
				//�w�i���쐬����
				m_backGround = NewGO<BackGround>(0, "background");
				//���W��ݒ肷��
				m_backGround->SetPosition(objData.position);
				return true;
			}

			//�I�u�W�F�N�g�̖��O���uenemy�v��������
			else if (objData.ForwardMatchName(L"enemy") == true)
			{
				//�G�l�~�[���쐬����
				auto enemy = NewGO<Enemy>(0, "enemy");
				//���W��ݒ肷��
				enemy->SetPosition(objData.position);
				//�ԍ���ݒ肷��
				int number = _wtoi(&objData.name[5]);
				enemy->SetMyNumber(number);
				return true;
			}
			return true;
		});

	return true;
}

void Game::Update()
{
	// g_renderingEngine->DisableRaytracing();
	//���x���̍X�V
	m_levelRender.Update();
}

void Game::Render(RenderContext& rc)
{
	//�`��
	m_levelRender.Draw(rc);
}