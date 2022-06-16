#pragma once

//�N���X�錾
class GameCamera;    //�Q�[���J����
class Player;        //�v���C���[

/// <summary>
/// �e�N���X
/// </summary>
class Bullet:public IGameObject
{
public:

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

	GameCamera*        m_gameCamera = nullptr;        //�Q�[���J����
	CollisionObject*   m_collisionObject = nullptr;   //�R���W�����I�u�W�F�N�g
	Player*            m_player = nullptr;            //�v���C���[
	float              m_deleteTimer = 0.0f;          //�폜�^�C�}�[
	Vector3            m_position;                    //���W
	Vector3            m_moveSpeed;                   //�ړ����x
	Quaternion         m_rotation;                    //��]
	Vector3            m_scale = Vector3::One;        //�傫��
};

