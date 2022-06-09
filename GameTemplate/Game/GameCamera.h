#pragma once

//�N���X�錾
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
	/// �����_�̍��W���擾
	/// </summary>
	/// <returns>���W</returns>
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
	Player*       m_player = nullptr;		        //�v���C���[�B
	Vector3       m_toCameraPos = Vector3::One;     //�����_���王�_�ւ̃x�N�g��
	Vector3       m_targetPosition;                 //�����_�̍��W
	SpriteRender  m_spriteRender;
};

