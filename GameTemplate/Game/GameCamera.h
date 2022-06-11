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
	const Vector3& GetToCameraPos() const
	{
		return m_toCameraPos;
	}
	const Vector3& GetCameraPosition() const
	{
		return m_cameraPosition;
	}

private:
	ModelRender    m_modelRender;
	FontRender    m_fontRender;
	Player*       m_player = nullptr;		        //プレイヤー。
	Vector3       m_toCameraPos = Vector3::One;     //注視点から視点へのベクトル
	Vector3       m_targetPosition;                 //注視点の座標
	Vector3       m_cameraPosition;
	SpriteRender  m_spriteRender;
};

