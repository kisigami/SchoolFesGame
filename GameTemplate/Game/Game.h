#pragma once

#include "Level3DRender/LevelRender.h"   //���x�������_�[���C���N���[�h����

//�N���X�錾
class Player;        //�v���C���[�N���X
class BackGround;    //�w�i�N���X
class GameCamera;    //�Q�[���J�����N���X
class Fade;          //�t�F�[�h�N���X
class PlayerUi;      //�v���C���[�֘A��UI�N���X
class SpeedEnemy;    //�X�s�[�h�G�l�~�[�N���X
class SpawnEnemy;
class GameUi;
class Result;

/// <summary>
/// �Q�[���N���X
/// </summary>
class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	const float& GetGameTime() const
	{
		return m_gameTimer;
	}
private:
	enum GameState
	{
		enGameState_End,
		enGameState_Idle,
	};
	void GameTime();
	void NotifyGameEnd();
	//�N���X���`����
	GameState m_gameState = enGameState_Idle;
	SoundSource* m_bgm = nullptr;          //BGM
	Player*      m_player = nullptr;       //�v���C���[
	BackGround*  m_backGround = nullptr;   //�w�i
	GameCamera*  m_gameCamera = nullptr;   //�Q�[���J����
	Fade*        m_fade = nullptr;         //�t�F�[�h
	PlayerUi*    m_playerUi = nullptr;     //�v���C���[�֘A��UI
	SpeedEnemy*  m_speedEnemy = nullptr;   //�X�s�[�h�G�l�~�[
	SkyCube*     m_skyCube = nullptr;
	SpawnEnemy*  m_spawnEnemy = nullptr;
	GameUi* m_gameUi = nullptr;
	Result* m_result = nullptr;

	LevelRender  m_levelRender;            //���x�������_�[
	FPSLimitter m_fpsLimitter;

	FontRender m_fontRender;
	FontRender m_fontRender2;
	float m_gameTimer = 60.0f;
	bool m_isFadeOutWait = false;
};

