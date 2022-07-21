#include "stdafx.h"
#include "GameUi.h"
#include "Game.h"
#include "Player.h"

GameUi::GameUi()
{
}

GameUi::~GameUi()
{
}

bool GameUi::Start()
{
	m_game = FindGO<Game>("game");
	m_player = FindGO<Player>("player");

	m_timeFontRender.Init("Assets/sprite/game/timefont.dds", 1920.0f, 1080.0f);
	m_timeButtonRender.Init("Assets/sprite/game/timebottom.dds", 1920.0f, 1080.0f);
	m_scoreFontRender.Init("Assets/sprite/game/scorefont.dds", 1920.0f, 1080.0f);
	m_scoreBottomRender.Init("Assets/sprite/game/scorebottom.dds", 1920.0f, 1080.0f);
	m_bulletNumRender.Init("Assets/sprite/game/bulletnum.dds", 1920.0f, 1080.0f);
	return true;
}

void GameUi::Update()
{
	wchar_t time[256];
	//int minute = (int)m_game->GetGameTime()/60;
	//int sec = (int)m_game->GetGameTime() % 60;
	//swprintf_s(time, 256, L"%02d:%02d", minute, sec);
	int sec = (int)m_game->GetGameTime() % 60;
	swprintf_s(time, 256, L"%02d秒", sec);
	//表示するテキストを設定。
	m_fontRender.SetText(time);
	m_fontRender.SetPosition(-722.0f, 514.0f, 0.0f);
	m_fontRender.SetScale(1.5f);
	m_fontRender.SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	m_score = m_player->GetKillEnemyCount()*100;

	wchar_t text[256];
	swprintf_s(text, 256, L"%04d", m_score);
	//表示するテキストを設定。
	m_fontRender2.SetText(text);
	m_fontRender2.SetPosition(-722.0f,430.0f,0.0f);
	m_fontRender2.SetScale(1.5f);
	m_fontRender2.SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	wchar_t text2[256];
	swprintf_s(text2, 256, L"%02d", m_player->GetBulletNum());
	//表示するテキストを設定。
	m_fontRender3.SetText(text2);
	m_fontRender3.SetPosition(770.0f, -420.0f, 0.0f);
	m_fontRender3.SetScale(2.0f);
	m_fontRender3.SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));

}

void GameUi::Render(RenderContext& rc)
{
	m_timeButtonRender.Draw(rc);
	m_timeFontRender.Draw(rc);
	m_scoreBottomRender.Draw(rc);
	m_scoreFontRender.Draw(rc);
	m_fontRender.Draw(rc);
	m_bulletNumRender.Draw(rc);
	m_fontRender2.Draw(rc);
	m_fontRender3.Draw(rc);
}