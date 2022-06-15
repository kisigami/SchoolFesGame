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
	/// ���W��ݒ肷��B
	/// </summary>
	/// <param name="position">���W�B</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// ��]��ݒ肷��B
	/// </summary>
	/// <param name="rotation">��]�B</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// �傫����ݒ肷��B
	/// </summary>
	/// <param name="scale">�傫���B</param>
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

