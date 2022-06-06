#pragma once

class Player:public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// ��]����
	/// </summary>
	void Rotation();
	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();

	ModelRender          m_modelRender;                 //���f�������_�[
	CharacterController  m_charaCon;                    //�L�����N�^�[�R���g���[���[
	Vector3              m_position;                    //���W
	Vector3              m_scale;                       //�傫��
	Vector3              m_moveSpeed;                   //�ړ����x
	Vector3              m_forward = Vector3::AxisZ;    //�v���C���[�̑O�x�N�g��
	Quaternion           m_rotation;                    //��]

};

