#pragma once
class Player:public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	ModelRender   m_modelRender;      //モデルレンダー
	Vector3       m_position;         //座標
	Vector3       m_scale;            //大きさ
	Quaternion    m_rotaition;        //回転

};

