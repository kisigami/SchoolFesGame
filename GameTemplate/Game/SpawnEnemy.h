#pragma once

class Enemy;
class SpeedEnemy;

struct Point
{
	Vector3 s_position = Vector3::Zero;
	int     s_number = 0;
};

class SpawnEnemy:public IGameObject
{
public:
	SpawnEnemy();
	~SpawnEnemy();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	const Vector3& GetStayPoint() const
	{
		return m_stayPosition;
	}

private:
	void LoadPoint();
	const Point* RandomPoint();
	void NewGoEnemy();

	const int MinNumber()const;

	Vector3 m_stayPosition = Vector3::Zero;
	const Point* m_point = nullptr;
	int num = 0;
	FontRender m_fontRender;
	Enemy* m_enemy = nullptr;
	SpeedEnemy* m_speedEnemy = nullptr;
	std::map<const int, Point> m_pointMap;
	int m_countEnemy = 0;
	int m_max = 0;
	int n = 0;
	float time = 0.0f;
};

