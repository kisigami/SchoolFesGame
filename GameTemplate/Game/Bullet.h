#pragma once

class GameCamera;
class Player;

class Bullet:public IGameObject
{
public:
	enum EnBullet
	{
		enBullet_Normal,
		enBullet_ShotGun,
	};

	Bullet();
	~Bullet();
	bool Start();
	void Update();

	/// <summary>
	/// 座標を設定する。
	/// </summary>
	/// <param name="position">座標。</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 回転を設定する。
	/// </summary>
	/// <param name="rotation">回転。</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// 大きさを設定する。
	/// </summary>
	/// <param name="scale">大きさ。</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

	EnBullet m_bulletName = enBullet_ShotGun;
	GameCamera* m_gameCamera = nullptr;
	CollisionObject* m_collisionObject;
	float m_timer = 0.0f;
	Vector3 m_position;
	Vector3 m_moveSpeed;
	Quaternion m_rotation;
	Vector3 m_scale = Vector3::One;
	Player* m_player = nullptr;
};

