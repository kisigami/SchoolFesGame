#pragma once

/// <summary>
/// フェードクラス
/// </summary>
class Fade:public IGameObject
{
public:
	Fade();
	~Fade();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// フェードインを開始する
	/// </summary>
	void StartFadeIn()
	{
		m_fadeState = enFadeState_FadeIn;
	}
	/// <summary>
	/// フェードアウトを開始する
	/// </summary>
	void StartFadeOut()
	{
		m_fadeState = enFadeState_FadeOut;
	}
	/// <summary>
	/// フェード中か？
	/// </summary>
	/// <returns>trueならフェード中</returns>
	const bool IsFade() const
	{
		return m_fadeState != enFadeState_Idle;
	}
	/// <summary>
	/// アルファ値を取得する
	/// </summary>
	/// <returns></returns>
	const float GetCurrentAlpha() const
	{
		return m_currentAlpha;
	}

private:
	/// <summary>
	/// フェードステート
	/// </summary>
	enum FadeState
	{
		enFadeState_Idle,    //待機ステート
		enFadeState_FadeIn,  //フェードインステート
		enFadeState_FadeOut  //フェードアウトステート
	};

	SpriteRender m_spriteRender;                   //スプライトレンダー
	FadeState    m_fadeState = enFadeState_Idle;   //フェードステート
	float        m_currentAlpha = 1.0f;            //アルファ値
};

