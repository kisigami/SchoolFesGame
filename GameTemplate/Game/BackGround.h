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
	/// ���W��ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// ��]��ݒ�
	/// </summary>
	/// <param name="rotation">��]</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

private:
	PhysicsStaticObject m_physicsStaticObject;   //�t�B�W�N�X�X�^�e�B�b�N�I�u�W�F�N�g
	ModelRender         m_modelRender;           //���f�������_�[
	Vector3             m_position;              //���W
	Quaternion          m_rotation;              //��]
	Vector3             m_scale;                 //�傫��
};

