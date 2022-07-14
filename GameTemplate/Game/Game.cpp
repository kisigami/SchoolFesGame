#include "stdafx.h"
#include "Game.h"

#include "Player.h"
#include "BackGround.h"
#include "GameCamera.h"

#include "Fade.h"
#include "PlayerUi.h"
#include "GameUi.h"
#include "Result.h"

#include "Enemy.h"
#include "SpeedEnemy.h"
#include "nature/SkyCube.h"

#include "time/FPSLimitter.h"
#include "SpawnEnemy.h"

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
	DeleteGO(m_gameUi);
	DeleteGO(m_skyCube);
	DeleteGO(m_spawnEnemy);
	DeleteGO(m_bgm);
}

bool Game::Start()
{
	m_fpsLimitter.SetMaxFPS(45);
	//�X�J�C�L���[�u�Ƃ������Ƃ����낢��
	DeleteGO(m_skyCube);
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetType((EnSkyCubeType)enSkyCubeType_Day);
	m_skyCube->SetLuminance(0.8f);
	g_renderingEngine->SetCascadeNearAreaRates(0.01f, 0.1f, 0.5f);
	g_renderingEngine->SetAmbient(Vector3::One*0.5f);


	//�Q�[���J�������쐬����
	m_gameCamera = NewGO<GameCamera>(0, "gamecamera");
	g_camera3D->SetFar(40000.0f);
	//�v���C���[���쐬����
	m_player = NewGO<Player>(0, "player");
	m_player = FindGO<Player>("player");

	//�v���C���[��UI���쐬����
	m_playerUi = NewGO<PlayerUi>(0,"playerui");
	m_gameUi = NewGO<GameUi>(0, "gameui");
	m_result = FindGO<Result>("result");

	m_spawnEnemy = NewGO<SpawnEnemy>(0,"spawnenemy");
	//BGM��ǂݍ���
	g_soundEngine->ResistWaveFileBank(BGM_NUMBER, "Assets/sound/bgm.wav");

	m_backGround = NewGO<BackGround>(0, "background");		

	//�t�F�[�h�̃C���X�^���X��T��
	m_fade = FindGO<Fade>("fade");
	//�t�F�[�h�C�����J�n����
	m_fade->StartFadeIn();

	//BGM���쐬����
	//m_bgm = NewGO<SoundSource>(0);
	//BGM������������
	//m_bgm->Init(BGM_NUMBER);
	//BGM���Đ�����i���[�v����j
	//m_bgm->Play(true);
	//BGM�̑傫����ݒ肷��
	//m_bgm->SetVolume(BGM_VOLUME);
	return true;
}

void Game::Update()
{
	GameTime();

	if (m_gameState == enGameState_End)
	{
		if (m_isFadeOutWait) {
			if (!m_fade->IsFade()) {
				m_result = NewGO<Result>(0, "result");
				m_result->SetScore(m_gameUi->GetScore());
				DeleteGO(this);
			}
		}
		else {
			m_isFadeOutWait = true;
			m_fade->StartFadeOut();
		}
	}
	m_levelRender.Update();
}

void Game::GameTime()
{
	m_gameTimer -= g_gameTime->GetFrameDeltaTime();
	
	if (m_gameTimer <= 0.0f)
	{
		m_gameTimer = 0.0f;
		//�Q�[���̏I����ʒm����
		NotifyGameEnd();
	}
}

void Game::NotifyGameEnd()
{
	m_gameState = enGameState_End;
}

void Game::Render(RenderContext& rc)
{
	m_levelRender.Draw(rc);
}