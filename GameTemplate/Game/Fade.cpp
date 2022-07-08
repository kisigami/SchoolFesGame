#include "stdafx.h"
#include "Fade.h"

Fade::Fade()
{
}

Fade::~Fade()
{
}

bool Fade::Start()
{
	m_spriteRender.Init("Assets/sprite/fade/fade.dds", 1920.0f, 1280.0f);
	return true;
}

void Fade::Update()
{
	switch (m_fadeState)
	{
	case Fade::enFadeState_Idle:
		break;
	case Fade::enFadeState_FadeIn:
		m_currentAlpha -= 2.0f * g_gameTime->GetFrameDeltaTime();
		if (m_currentAlpha <= 0.0f)
		{
			m_currentAlpha = 0.0f;
			m_fadeState = enFadeState_Idle;
		}
		break;
	case Fade::enFadeState_FadeOut:
		m_currentAlpha += 2.0f * g_gameTime->GetFrameDeltaTime();
		if (m_currentAlpha >= 1.0f)
		{
			m_currentAlpha = 1.0f;
			m_fadeState = enFadeState_Idle;
		}
		break;
	}
}

void Fade::Render(RenderContext& rc)
{
	if (m_currentAlpha > 0.0)
	{
		m_spriteRender.SetMulColor({ 1.0f,1.0f,1.0f,m_currentAlpha });
		m_spriteRender.Draw(rc);
	}
}