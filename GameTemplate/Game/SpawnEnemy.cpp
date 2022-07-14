#include "stdafx.h"
#include "SpawnEnemy.h"

#include "level3DRender/LevelRender.h"

#include "SpeedEnemy.h"

namespace
{
	const float SPAWN_TIME = 5.0f;
	const float INIT_TIME = 0.0f;
	const int SPAWN_ENEMY_NUM = 3;
}

SpawnEnemy::SpawnEnemy()
{
}

SpawnEnemy::~SpawnEnemy()
{
	const auto& speedEnemys = FindGOs<SpeedEnemy>("speedenemy");
	for (auto speedenemy : speedEnemys)
	{
		DeleteGO(speedenemy);
	}
}

bool SpawnEnemy::Start()
{
	//ポイントの読み込み
	LoadPoint();
	m_speedEnemy = FindGO<SpeedEnemy>("speedenemy");
	return true;
}

void SpawnEnemy::Update()
{
	m_spawnTimer += g_gameTime->GetFrameDeltaTime();
	if (m_spawnTimer >= SPAWN_TIME)
	{
		//エネミーを再出現させる
		NewGoEnemy();
		m_spawnTimer = INIT_TIME;
	}

	//あとでけす
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"%d", int());

	//表示するテキストを設定。
	m_fontRender.SetText(wcsbuf);
	m_fontRender.SetPosition(Vector3::Zero);
	m_fontRender.SetScale(2.3f);
	m_fontRender.SetColor(Vector4(1.0f, 1.0f, 0.0f, 1.0f));
}

void SpawnEnemy::LoadPoint()
{
	LevelRender m_levelRender;
	m_levelRender.Init("Assets/level3D/point.tkl", [&](LevelObjectData& objData)
	{
		if (objData.ForwardMatchName(L"Point") == true)
		{
			Point point;
			int number = _wtoi(&objData.name[5]);
			point.s_number = number;
			point.s_position = objData.position;
			m_pointMap[number - 1] = point;
			return true;
		}

		else if (objData.ForwardMatchName(L"StayPoint") == true)
		{
			m_stayPosition = objData.position;
			for (int i = 1; i <= SPAWN_ENEMY_NUM; i++)
			{
				auto speedEnemys = NewGO<SpeedEnemy>(0, "speedenemy");
				speedEnemys->SetPosition(m_stayPosition);
				speedEnemys->SetMyNumber(m_countEnemy);
				speedEnemys->SetActiveFlag(false);
				m_countEnemy++;
			}
			return true;
		}
		return true;
	});
}

const Point* SpawnEnemy::RandomPoint()
{
	//乱数でポイントを返す
	Point* point = &m_pointMap[0];
	m_num = m_pointMap.size();
	int randm = rand() % m_num;
	m_num = randm;
	point = &m_pointMap[m_num];
	return point;
}

void SpawnEnemy::NewGoEnemy()
{
	//ポイントを取得
	m_point = RandomPoint();
	//非アクティブ、一番若いエネミー番号を取得
	int num = MinNumber();
	const auto& speedEnemys = FindGOs<SpeedEnemy>("speedenemy");
	for (auto speedenemy : speedEnemys)
	{
		//非アクティブだったら
		if (speedenemy->GetMyNumber() == num &&
			speedenemy->GetActiveFlag() == false)
		{
			//ポイントの座標を代入
			speedenemy->SetPosition(m_point->s_position);
			//アクティブにする
			speedenemy->SetActiveFlag(true);
		}
	}
}

const int SpawnEnemy::MinNumber()const
{
	//非アクティブで一番若いエネミー番号を返す
	const auto& speedEnemys = FindGOs<SpeedEnemy>("speedenemy");
	for (auto speedenemy : speedEnemys)
	{
		
		int j = m_countEnemy - 1;

		for (int i = j; i >= 0; i--)
		{
			if (speedenemy->GetMyNumber() == i &&
				speedenemy->GetActiveFlag() == false)
			{
				if (j > i)
				{
					j = i;
				}
			}
		}

		if (speedenemy->GetMyNumber() == j &&
			speedenemy->GetActiveFlag() == false)
		{
			return j;
		}
	}
}


void SpawnEnemy::Render(RenderContext& rc)
{
	//m_fontRender.Draw(rc);
}

