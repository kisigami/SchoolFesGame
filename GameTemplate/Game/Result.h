#pragma once

class Title;
class Fade;
class GameUi;

class Result:public IGameObject
{
public:
	Result();
	~Result();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void SetScore(const int& score)
	{
		m_score = score;
	}
private:
	int m_score = 0;
	FontRender   m_fontRender;
	SpriteRender m_resultBottomRender;
	SpriteRender m_titleFontRender;
	Title* m_title = nullptr;
	Fade* m_fade = nullptr;
	GameUi* m_gameUi = nullptr;
	bool m_isWaitFadeout = false;
	SoundSource* m_bgm = nullptr;
	float m_alpha = 0.0f;
	bool a = false;

};

