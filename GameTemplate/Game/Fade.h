#pragma once

class Fade:public IGameObject
{
public:
	Fade();
	~Fade();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void StartFadeIn()
	{
		m_fadeState = enFadeState_FadeIn;
	}
	void StartFadeOut()
	{
		m_fadeState = enFadeState_FadeOut;
	}
	const bool IsFade() const
	{
		return m_fadeState != enFadeState_Idle;
	}
	const float GetCurrentAlpha() const
	{
		return m_currentAlpha;
	}
private:
	enum FadeState
	{
		enFadeState_Idle,
		enFadeState_FadeIn,
		enFadeState_FadeOut
	};

	SpriteRender m_spriteRender;
	FadeState    m_fadeState = enFadeState_Idle;
	float        m_currentAlpha = 1.0f;
};

