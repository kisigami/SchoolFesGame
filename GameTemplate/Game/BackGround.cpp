#include "stdafx.h"
#include "BackGround.h"

BackGround::BackGround()
{

}

BackGround::~BackGround()
{

}

bool BackGround::Start()
{
	//���f���̕`��
	m_modelRender.Init("Assets/modelData/background/background.tkm");
	//�����蔻��
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	//�����蔻��̉���
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	
	return true;
}

void BackGround::Update()
{
	//���f���̍X�V
	m_modelRender.Update();
}

void BackGround::Render(RenderContext& rc)
{
	//���f���̕`��
	m_modelRender.Draw(rc);
}