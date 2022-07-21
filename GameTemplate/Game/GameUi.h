#pragma once

class Game;
class Player;

class GameUi:public IGameObject
{
public:
	GameUi();
	~GameUi();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	const int& GetScore() const
	{
		return m_score;
	}
private:
	int m_score = 0;
	Game* m_game = nullptr;
	Player* m_player = nullptr;
	FontRender m_fontRender;
	FontRender m_fontRender2;
	FontRender m_fontRender3;
	SpriteRender m_timeFontRender;
	SpriteRender m_timeButtonRender;
	SpriteRender m_scoreFontRender;
	SpriteRender m_scoreBottomRender;
	SpriteRender m_bulletNumRender;
};

