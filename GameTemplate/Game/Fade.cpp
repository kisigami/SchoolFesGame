#include "stdafx.h"
#include "Fade.h"

namespace
{
	const float SPRITE_WIDTH = 1920.0f;            //画像の横幅
	const float SPRITE_HEIGHT = 1080.0f;           //画像の縦幅
	const float ALPHA_CHANGE_VALUE = 2.0f;         //アルファ値の変化値
	const float ALPHA_MAX_VALUE = 1.0f;            //アルファ値の最大値
	const float ALPHA_MIN_VALUE = 0.0f;            //アルファ値の最低値
	const float FADE_SPRITE_COLOR_R_VALUE = 1.0f;  //フェード画像のR成分の値
	const float FADE_SPRITE_COLOR_G_VALUE = 1.0f;  //フェード画像のG成分の値
	const float FADE_SPRITE_COLOR_B_VALUE = 1.0f;  //フェード画像のB成分の値
}
Fade::Fade()
{
}

Fade::~Fade()
{
}

bool Fade::Start()
{
	//フェード時に表示する画像を読み込む
	m_spriteRender.Init("Assets/sprite/fade/fade.dds", SPRITE_WIDTH, SPRITE_HEIGHT);
	return true;
}

void Fade::Update()
{
	switch (m_fadeState)
	{
		//待機ステートだった時
	case Fade::enFadeState_Idle:
		break;
		//フェードインステートだった時
	case Fade::enFadeState_FadeIn:
		//画像のアルファ値を下げる
		m_currentAlpha -= ALPHA_CHANGE_VALUE * g_gameTime->GetFrameDeltaTime();
		//画像のアルファ値がアルファ値の最低値より小さかったら
		if (m_currentAlpha <= ALPHA_MIN_VALUE)
		{
			//画像のアルファを最低値にする
			m_currentAlpha = ALPHA_MIN_VALUE;
			//待機ステートに移行する
			m_fadeState = enFadeState_Idle;
		}
		break;
		//フェードアウトステートだった時
	case Fade::enFadeState_FadeOut:
		//画像のアルファ値をあげる
		m_currentAlpha += ALPHA_CHANGE_VALUE * g_gameTime->GetFrameDeltaTime();
		//画像のアルファ値がアルファ値の最大値より大きかったら
		if (m_currentAlpha >= ALPHA_MAX_VALUE)
		{
			//画像のアルファ値を最大値にする
			m_currentAlpha = ALPHA_MAX_VALUE;
			//待機ステートに移行する
			m_fadeState = enFadeState_Idle;
		}
		break;
	}
}

void Fade::Render(RenderContext& rc)
{
	//画像のアルファ値がアルファ値の最低値より大きかったら
	if (m_currentAlpha > ALPHA_MIN_VALUE)
	{
		//画像の乗算カラーを設定する
		m_spriteRender.SetMulColor({ 
			FADE_SPRITE_COLOR_R_VALUE,
			FADE_SPRITE_COLOR_G_VALUE,
			FADE_SPRITE_COLOR_B_VALUE,
			m_currentAlpha });
		//画像を描画する
		m_spriteRender.Draw(rc);
	}
}