#pragma once

#include "Level3DRender/LevelRender.h"   //���x�������_�[���C���N���[�h����

//�N���X�錾
class Player;        //�v���C���[�N���X
class BackGround;    //�w�i�N���X
class GameCamera;    //�Q�[���J�����N���X
class Fade;          //�t�F�[�h�N���X
class PlayerUi;      //�v���C���[�֘A��UI�N���X
class Enemy;         //�G�l�~�[�N���X
class SpeedEnemy;    //�X�s�[�h�G�l�~�[�N���X

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

private:
	//�N���X���`����
	SoundSource* m_bgm = nullptr;          //BGM
	Player*      m_player = nullptr;       //�v���C���[
	BackGround*  m_backGround = nullptr;   //�w�i
	GameCamera*  m_gameCamera = nullptr;   //�Q�[���J����
	Fade*        m_fade = nullptr;         //�t�F�[�h
	PlayerUi*    m_playerUi = nullptr;     //�v���C���[�֘A��UI
	Enemy*       m_enemy = nullptr;        //�G�l�~�[
	SpeedEnemy*  m_speedEnemy = nullptr;   //�X�s�[�h�G�l�~�[

	LevelRender  m_levelRender;            //���x�������_�[
};

