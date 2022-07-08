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
	bool         m_isWaitFadeout = false;
	Game*        m_game = nullptr;
	Fade*        m_fade = nullptr;
	SpriteRender m_spriteRender;
};

