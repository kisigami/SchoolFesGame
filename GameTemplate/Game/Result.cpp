#include "stdafx.h"
#include "Result.h"

#include "Fade.h"
#include "Title.h"
#include "GameUi.h"

Result::Result()
{}

Result::~Result()
{}

bool Result::Start()
{
	m_resultBottomRender.Init("Assets/sprite/result/resultbottom.dds", 1920.0f, 1080.0f);
	m_titleFontRender.Init("Assets/sprite/result/titlefont.dds", 1920.0f, 1080.0f);

	m_fade = FindGO<Fade>("fade");
	m_gameUi = FindGO<GameUi>("gameui");
	m_fade->StartFadeIn();
	return true;
}

void Result::Update()
{
	wchar_t text[256];
	swprintf_s(text, 256, L"%04d", m_score);
	//表示するテキストを設定。
	m_fontRender.SetText(text);
	m_fontRender.SetPosition(Vector3::Zero);
	m_fontRender.SetScale(5.0f);
	m_fontRender.SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	if (!m_fade->IsFade() && m_isWaitFadeout == true)
	{
		m_title = NewGO<Title>(0, "title");
		DeleteGO(this);
	}
	if (g_pad[0]->IsTrigger(enButtonA))
	{
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
