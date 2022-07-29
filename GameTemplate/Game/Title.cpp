#include "stdafx.h"
#include "Title.h"

#include "Game.h"
#include "Fade.h"

Title::Title()
{
}

Title::~Title()
{
	DeleteGO(bgm);
}

bool Title::Start()
{
	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/title.wav");
	g_soundEngine->ResistWaveFileBank(21, "Assets/sound/kettei.wav");
	bgm = NewGO<SoundSource>(0);
	bgm->Init(6);
	bgm->SetVolume(0.2f);
	bgm->Play(true);

	m_titleBottomRender.Init("Assets/sprite/title/titlebottom.dds", 1920.0f, 1080.0f);
	m_titleFontRender.Init("Assets/sprite/title/titlefont.dds", 1920.0f, 1080.0f);
	m_startFontRender.Init("Assets/sprite/title/startfont.dds", 1920.0f, 1080.0f);
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
		if (a == false)
		{
			SoundSource* se;
			se = NewGO<SoundSource>(0);
			se->Init(21);
			se->SetVolume(0.2f);
			se->Play(false);
			a = true;
		}
			m_isWaitFadeout = true;
			m_fade->StartFadeOut();
		
	}

	if (m_isWaitFadeout)
	{
		m_alpha += g_gameTime->GetFrameDeltaTime() * 20.0f;
		m_alpha2 += g_gameTime->GetFrameDeltaTime() * 20.0f;
	}
	else
	{
		m_alpha += g_gameTime->GetFrameDeltaTime() * 0.5f;
		m_alpha2 += g_gameTime->GetFrameDeltaTime() * 5.0f;
	}

	m_titleFontRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
	m_startFontRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha2))));
	m_titleFontRender.Update();
	m_startFontRender.Update();
}

void Title::Render(RenderContext& rc)
{
	m_titleBottomRender.Draw(rc);
	//m_titleFontRender.Draw(rc);
	m_startFontRender.Draw(rc);
}