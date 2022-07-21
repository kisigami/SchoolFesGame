#pragma once
class EffectSound:public IGameObject
{
public:
	EffectSound();
	~EffectSound();
	bool Start();

private:
	void LoadSound();
	void LoadEffect();
};

