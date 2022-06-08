#pragma once

class BackGround :public IGameObject
{
public:
	BackGround();
	~BackGround();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// 回転を設定
	/// </summary>
	/// <param name="rotation">回転</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

private:
	PhysicsStaticObject m_physicsStaticObject;   //フィジクススタティックオブジェクト
	ModelRender         m_modelRender;           //モデルレンダー
	Vector3             m_position;              //座標
	Quaternion          m_rotation;              //回転
	Vector3             m_scale;                 //大きさ
};

