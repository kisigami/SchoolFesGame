#include "stdafx.h"
#include "Game.h"

#include "Player.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "Enemy.h"
#include "Fade.h"

#include "SpeedEnemy.h"
#include "PlayerUi.h"

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
	//�v���C���[��UI���쐬����
	m_playerUi = NewGO<PlayerUi>(0,"playerui");

	m_speedEnemy = NewGO<SpeedEnemy>(0, "speedenemy");

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

	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/bgm.wav");
	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(0);
	m_bgm->Play(true);
	m_bgm->SetVolume(0.2f);
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