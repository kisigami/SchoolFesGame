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

	Enemy*                     m_enemy = nullptr;
	SpeedEnemy*                m_speedEnemy = nullptr;
	const Point*               m_point = nullptr;
	std::map<const int, Point> m_pointMap;
	FontRender                 m_fontRender;
	Vector3                    m_stayPosition = Vector3::Zero;
	
	int                        m_countEnemy = 0;
	int                        m_num = 0;
	int                        m_max = 0;
	float                      m_spawnTimer = 0.0f;
};

