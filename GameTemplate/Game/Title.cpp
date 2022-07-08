#include "stdafx.h"
#include "Title.h"

#include "Game.h"
#include "Fade.h"

Title::Title()
{
}

Title::~Title()
{
}

bool Title::Start()
{
	m_spriteRender.Init("Assets/sprite/title/title.dds", 1920.0f, 1280.0f);

	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	return true;
}

void Title::Update()
{
	
	if (!m_fade->IsFade() && m_isWaitFadeout==true)
	{	
		m_game = NewGO<Game>(0, "game");
		DeleteGO(this);		
	}
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_isWaitFadeout = true;
		m_fade->StartFadeOut();
	}
	
	m_spriteRender.Update();
}

void Title::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}