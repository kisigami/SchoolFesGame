#pragma once

class Game;
class Fade;

class Title:public IGameObject
{
public:
	Title();
	~Title();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	float m_timer = 0.0f;
	float m_alpha = 0.0f;
	float m_alpha2 = 0.0f;
	bool         m_isWaitFadeout = false;
	Game*        m_game = nullptr;
	Fade*        m_fade = nullptr;
	SpriteRender m_titleBottomRender;
	SpriteRender m_titleFontRender;
	SpriteRender m_startFontRender;
	SpriteRender m_aFontRender;
};

