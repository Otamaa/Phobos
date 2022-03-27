#pragma once

#include <Utilities/Enumerable.h>
#include <Utilities/Template.h>
#include <Utilities/GeneralUtils.h>

enum CumulativeMode : int
{ NO = 0, YES = 1, ATTACKER = 2 };

class AttachEffectTypeClass final : public Enumerable<AttachEffectTypeClass>
{
public:

	ValueableVector<PhobosFixedString<0x18>> AffectTypes; // 可影响的单位
	Valueable<int> Duration; // 持续时间
	Valueable< int >Delay; // 不可获得同名的延迟
	Valueable< bool> RandomDelay; // 随机延迟
	Valueable< int> MinDelay; // 随机最小值
	Valueable< int >MaxDelay; // 随机最大值
	Valueable< int >InitialDelay; // 生效前的初始延迟
	Valueable< bool> InitialRandomDelay; // 随机初始延迟
	Valueable< int >InitialMinDelay; // 随机最小值
	Valueable< int >InitialMaxDelay; // 随机最大值
	Valueable< bool >DiscardOnEntry; // 离开地图则失效
	Valueable< bool> PenetratesIronCurtain; // 弹头附加，影响铁幕
	CumulativeMode Cumulative; // 可叠加

	//none yet
	Valueable< int> Group; // 分组，同一个分组的效果互相影响，削减或增加持续时间
	Valueable< bool> OverrideSameGroup; // 是否覆盖同一个分组
	
	PhobosFixedString<0x18> Next; // 结束后播放下一个AE
	Valueable<bool> TemporalHidesAnim;
	Valueable<bool>	TemporalUpdateAE;
	Valueable<bool> LimboUpdateAe;
	Valueable<bool> AnimResetOnReapply;
	Valueable<AnimTypeClass*> AnimType;
	Valueable<bool> AppliedOnlyOnce;
	Valueable<bool> ForceDecloak;

	Valueable<double> FirepowerMultiplier;
	Valueable<double> SpeedMultiplier;
	Valueable<double> ArmorMultiplier;
	Valueable<bool> Cloakable;

private:
	//	template <typename T>
	//	void Serialize(T& Stm);
public:
	AttachEffectTypeClass(const char* const pTitle) : Enumerable<AttachEffectTypeClass>(pTitle)
		, AffectTypes() 
		, Duration()
		, Delay()
		, RandomDelay()
		, MinDelay()
		, MaxDelay()
		, InitialDelay()
		, InitialRandomDelay()
		, InitialMinDelay()
		, InitialMaxDelay()
		, DiscardOnEntry()
		, PenetratesIronCurtain()
		, Cumulative()

		//none yet
		, Group()
		, OverrideSameGroup()

		, Next()
		, TemporalHidesAnim()
		, AnimType()
		, AppliedOnlyOnce()
		, AnimResetOnReapply()
		, LimboUpdateAe()
		, TemporalUpdateAE()
		, ForceDecloak()
	{ }

	virtual ~AttachEffectTypeClass() override = default;
//	virtual void LoadFromINI(CCINIClass* pINI) override;
	virtual void LoadFromStream(PhobosStreamReader& Stm) override
	{

	}

	virtual void SaveToStream(PhobosStreamWriter& Stm) override
	{

	}

	static void AddDefault()
	{
		if (auto pDef = FindOrAllocate("DefaultAE"))
		{
			Debug::Log("Allocated DefaultAE[%x] !\n", pDef);
			pDef->Delay = 200;
			pDef->InitialDelay = 200;
			pDef->Duration = -1;
			pDef->AnimType = AnimTypeClass::FindOrAllocate("DISKRAY");
			pDef->Cumulative = CumulativeMode::NO;
		}
	}


};