#include "stdafx.h"
#include "DemoRender.h"
#include "Game.h"



bool DemoRender::Start()
{
	m_animationClip[enAnimClip_Idle].Load("Assets/animData/enemy/idle.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Down].Load("Assets/animData/enemy/down.tka");
	m_animationClip[enAnimClip_Down].SetLoopFlag(false);

	m_modelRender.Init(
		"Assets/modelData/enemy/enemy.tkm",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ,
		true,
		m_maxDemo
	);

	m_game = FindGO<Game>("game");
	return true;
}

void DemoRender::Update()
{
}

void DemoRender::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}