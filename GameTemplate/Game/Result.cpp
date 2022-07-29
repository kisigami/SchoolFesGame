#include "stdafx.h"
#include "Result.h"

#include "Fade.h"
#include "Title.h"
#include "GameUi.h"

Result::Result()
{
}

Result::~Result()
{
	DeleteGO(m_bgm);
}

bool Result::Start()
{
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/result.wav");
	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(7);
	m_bgm->SetVolume(0.2f);
	m_bgm->Play(true);

	m_resultBottomRender.Init("Assets/sprite/result/resultbottom.dds", 1920.0f, 1080.0f);
	m_titleFontRender.Init("Assets/sprite/result/titlefont.dds", 1920.0f, 1080.0f);

	m_fade = FindGO<Fade>("fade");
	m_gameUi = FindGO<GameUi>("gameui");
	m_fade->StartFadeIn();
	return true;
}

void Result::Update()
{
	if (m_isWaitFadeout)
	{
		m_alpha += g_gameTime->GetFrameDeltaTime() * 20.0f;
	}
	else
	{
		m_alpha += g_gameTime->GetFrameDeltaTime() * 5.0f;
	}

	m_titleFontRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
	m_titleFontRender.Update();

	wchar_t text[256];
	swprintf_s(text, 256, L"%04d", m_score);
	//表示するテキストを設定。
	m_fontRender.SetText(text);
	m_fontRender.SetPosition(-245.0f,100.0f,0.0f);
	m_fontRender.SetScale(5.0f);
	m_fontRender.SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	if (!m_fade->IsFade() && m_isWaitFadeout == true)
	{
		m_title = NewGO<Title>(0, "title");
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
}

void Result::Render(RenderContext& rc)
{
	m_resultBottomRender.Draw(rc);
	m_titleFontRender.Draw(rc);
	m_fontRender.Draw(rc);
}
