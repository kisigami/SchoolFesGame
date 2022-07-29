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
	//モデルの描画
	m_modelRender.Init("Assets/modelData/background/background.tkm");
	
	m_modelRender.Update();
	
	//当たり判定
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	
	//m_physicsStaticObject.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);
	//当たり判定の可視化
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	
	return true;
}

void BackGround::Update()
{
	//モデルの更新
	m_modelRender.Update();
}

void BackGround::Render(RenderContext& rc)
{
	//モデルの描画
	m_modelRender.Draw(rc);
}