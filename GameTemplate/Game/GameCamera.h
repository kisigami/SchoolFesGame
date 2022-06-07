#pragma once

//クラス宣言。
class Player;

class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();

	const Vector3& GetTargetPosition() const
	{
		return m_targetPosition;
	}

private:
	Player* m_player = nullptr;		//プレイヤー。
	Vector3 m_toCameraPos = Vector3::One;
	Vector3 m_targetPosition;
};

