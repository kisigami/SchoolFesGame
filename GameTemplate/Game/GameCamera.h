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
	Player*       m_player = nullptr;		        //�v���C���[�B
	Vector3       m_toCameraPos = Vector3::One;     //�����_���王�_�ւ̃x�N�g��
	Vector3       m_targetPosition;                 //�����_�̍��W
	Vector3       m_cameraPosition;
	SpriteRender  m_spriteRender;
};

