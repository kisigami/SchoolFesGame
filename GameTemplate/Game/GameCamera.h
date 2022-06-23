#pragma once

//クラス宣言
class Player;   //プレイヤークラス

/// <summary>
/// ゲームカメラクラス
/// </summary>
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
	/// <summary>
	/// 視点から注視点のベクトルを取得
	/// </summary>
	/// <returns>ベクトル</returns>
	const Vector3& GetToCameraPos() const
	{
		return m_toCameraPos;
	}
	/// <summary>
	/// 視点の座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetCameraPosition() const
	{
		return m_cameraPosition;
	}

	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}

private:
	ModelRender   m_modelRender;
	Quaternion    m_rotation;
	Player*       m_player = nullptr;		        //プレイヤー。
	Vector3       m_toCameraPos;     //視点から注視点へのベクトル
	Vector3       m_targetPosition;                 //注視点の座標
	Vector3       m_cameraPosition;                 //視点の座標
	SpriteRender  m_spriteRender;                   //スプライトレンダー
};

