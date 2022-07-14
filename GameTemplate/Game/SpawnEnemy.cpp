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
	//�|�C���g�̓ǂݍ���
	LoadPoint();
	m_speedEnemy = FindGO<SpeedEnemy>("speedenemy");
	return true;
}

void SpawnEnemy::Update()
{
	m_spawnTimer += g_gameTime->GetFrameDeltaTime();
	if (m_spawnTimer >= SPAWN_TIME)
	{
		//�G�l�~�[���ďo��������
		NewGoEnemy();
		m_spawnTimer = INIT_TIME;
	}

	//���Ƃł���
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"%d", int());

	//�\������e�L�X�g��ݒ�B
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
	//�����Ń|�C���g��Ԃ�
	Point* point = &m_pointMap[0];
	m_num = m_pointMap.size();
	int randm = rand() % m_num;
	m_num = randm;
	point = &m_pointMap[m_num];
	return point;
}

void SpawnEnemy::NewGoEnemy()
{
	//�|�C���g���擾
	m_point = RandomPoint();
	//��A�N�e�B�u�A��ԎႢ�G�l�~�[�ԍ����擾
	int num = MinNumber();
	const auto& speedEnemys = FindGOs<SpeedEnemy>("speedenemy");
	for (auto speedenemy : speedEnemys)
	{
		//��A�N�e�B�u��������
		if (speedenemy->GetMyNumber() == num &&
			speedenemy->GetActiveFlag() == false)
		{
			//�|�C���g�̍��W����
			speedenemy->SetPosition(m_point->s_position);
			//�A�N�e�B�u�ɂ���
			speedenemy->SetActiveFlag(true);
		}
	}
}

const int SpawnEnemy::MinNumber()const
{
	//��A�N�e�B�u�ň�ԎႢ�G�l�~�[�ԍ���Ԃ�
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

