#pragma once

//クラス宣言
class Player;

class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// 注視点の座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetTargetPosition() const
	{
		return m_targetPosition;
	}
	const Vector3& GetCameraPos() const
	{
		return m_toCameraPos;
	}
private:
	FontRender    m_fontRender;
	ModelRender   m_modelRender;
	Player*       m_player = nullptr;		        //プレイヤー。
	Vector3       m_toCameraPos = Vector3::One;     //注視点から視点へのベクトル
	Vector3       m_targetPosition;                 //注視点の座標
	SpriteRender  m_spriteRender;
};

