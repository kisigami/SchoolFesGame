#include "stdafx.h"
#include "Game.h"

#include "Player.h"
#include "BackGround.h"
#include "GameCamera.h"

#include "Fade.h"
#include "PlayerUi.h"

#include "Enemy.h"
#include "SpeedEnemy.h"

namespace
{
	const int   BGM_NUMBER = 0;      //BGM�̔ԍ�
	const float BGM_VOLUME = 0.2f;   //BGM�̃T�E���h�{�����[��

}
Game::Game()
{
}

Game::~Game()
{
	//�J�������폜����
	DeleteGO(m_gameCamera);
	//�v���C���[���폜����
	DeleteGO(m_player);
	//�w�i���폜����
	DeleteGO(m_backGround);
	//�v���C���[�֘A��UI���폜����
	DeleteGO(m_playerUi);
}

bool Game::Start()
{
	//�Q�[���J�������쐬����
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");
	//�v���C���[���쐬����
	m_player = NewGO<Player>(0, "player");
	//�v���C���[��UI���쐬����
	m_playerUi = NewGO<PlayerUi>(0,"playerui");
	//�X�s�[�h�G�l�~�[���쐬����
	m_speedEnemy = NewGO<SpeedEnemy>(0, "speedenemy");

	//BGM��ǂݍ���
	g_soundEngine->ResistWaveFileBank(BGM_NUMBER, "Assets/sound/bgm.wav");

	////�X�e�[�W�̃��x���̓ǂݍ���
	m_levelRender.Init("Assets/level3D/stage.tkl", [&](LevelObjectData& objData)
		{
			//�I�u�W�F�N�g�̖��O���ubackground�v��������
			if (objData.EqualObjectName(L"background") == true)
			{
				//�w�i���쐬����
				m_backGround = NewGO<BackGround>(0, "background");
				//���W��ݒ肷��
				m_backGround->SetPosition(objData.position);
				m_backGround->SetScale(objData.scale);
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

	//�t�F�[�h�̃C���X�^���X��T��
	m_fade = FindGO<Fade>("fade");
	//�t�F�[�h�C�����J�n����
	m_fade->StartFadeIn();

	//BGM���쐬����
	m_bgm = NewGO<SoundSource>(0);
	//BGM������������
	m_bgm->Init(BGM_NUMBER);
	//BGM���Đ�����i���[�v����j
	m_bgm->Play(true);
	//BGM�̑傫����ݒ肷��
	m_bgm->SetVolume(BGM_VOLUME);
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