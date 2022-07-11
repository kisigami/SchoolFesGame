#pragma once

//�N���X�錾
class Player;   //�v���C���[�N���X

/// <summary>
/// �Q�[���J�����N���X
/// </summary>
class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();

	/// <summary>
	/// �����_�̍��W���擾
	/// </summary>
	/// <returns>���W</returns>
	const Vector3& GetTargetPosition() const
	{
		return m_targetPosition;
	}
	/// <summary>
	/// ���_���璍���_�̃x�N�g�����擾
	/// </summary>
	/// <returns>�x�N�g��</returns>
	const Vector3& GetToCameraPos() const
	{
		return m_toCameraPos;
	}
	/// <summary>
	/// ���_�̍��W���擾
	/// </summary>
	/// <returns>���W</returns>
	const Vector3& GetCameraPosition() const
	{
		return m_cameraPosition;
	}

private:
	//�N���X���`����
	Player*       m_player = nullptr;  //�v���C���[

	Quaternion    m_rotation;	       //��]
	Vector3       m_toCameraPos;       //���_���璍���_�ւ̃x�N�g��
	Vector3       m_targetPosition;    //�����_�̍��W
	Vector3       m_cameraPosition;    //���_�̍��W
};

