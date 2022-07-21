#include "stdafx.h"
#include "EffectSound.h"

EffectSound::EffectSound()
{}

EffectSound::~EffectSound()
{}

void EffectSound::LoadSound()
{
	//�G�l�~�[�T�E���h
	g_soundEngine->ResistWaveFileBank(5, "Assets/sound/enemy/enemydownsound.wav");
	g_soundEngine->ResistWaveFileBank(11, "Assets/sound/enemy/walksound2.wav");
	g_soundEngine->ResistWaveFileBank(22, "Assets/sound/enemy/pumch.wav");
}

void EffectSound::LoadEffect()
{
	EffectEngine::GetInstance()->ResistEffect(11, u"Assets/effect/efk/blood.efk");
}

bool EffectSound::Start()
{
	//�T�E���h�����[�h
	LoadSound();
	//�G�t�F�N�g�����[�h
	LoadEffect();

	return true;
}