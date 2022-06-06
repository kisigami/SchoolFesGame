#pragma once
class Player:public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	ModelRender   m_modelRender;      //���f�������_�[
	Vector3       m_position;         //���W
	Vector3       m_scale;            //�傫��
	Quaternion    m_rotaition;        //��]

};

