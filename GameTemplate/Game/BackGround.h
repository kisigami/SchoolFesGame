#pragma once

class BackGround:public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	PhysicsStaticObject m_physicsStaticObject;
	ModelRender m_modelRender;
	Vector3 m_position;
};

