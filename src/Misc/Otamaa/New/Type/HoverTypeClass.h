#pragma once

#include <Utilities/Enumerable.h>
#include <Utilities/TemplateDef.h>
#include <Utilities/GeneralUtils.h>

class HoverTypeClass final : public Enumerable<HoverTypeClass>
{
public:

	PhobosFixedString<32> AboveWaterAnim;
	NullableIdx<VocClass> ScoldSound;
	Valueable<int> HoverHeight;
	Valueable<double> HoverBob;
	Valueable<double> HoverDampen;
	Valueable<double> HoverAcceleration;
	Valueable<double> HoverBrake;
	Valueable<double> HoverBoost;

	HoverTypeClass(const char* const pTitle) : Enumerable<HoverTypeClass>(pTitle)
		, AboveWaterAnim { }
		, ScoldSound { }
		, HoverHeight { }
		, HoverBob { }
		, HoverDampen { }
		, HoverAcceleration { }
		, HoverBrake { }
		, HoverBoost { }
	{ }

	virtual ~HoverTypeClass() override = default;

	AnimTypeClass* GetAboveWaterAnim() const
	{
		AnimTypeClass* pAnim = nullptr;
		if (this->AboveWaterAnim)
			pAnim = AnimTypeClass::Find(this->AboveWaterAnim.data());

		return !pAnim ? (RulesClass::Instance->Wake) : pAnim;
	}

	int GetScoldSound()
	{
		return ScoldSound.Get(RulesClass::Instance->ScoldSound);
	}

	int GetHeight()
	{
		return abs(HoverHeight.Get()) > 0 ? abs(HoverHeight.Get()) : RulesClass::Instance->HoverHeight;
	}

	double GetBob()
	{
		return abs(HoverBob.Get()) > 0.0 ? abs(HoverBob.Get()) : RulesClass::Instance->HoverBob;
	}

	double GetDampen()
	{
		return abs(HoverDampen.Get()) > 0.0 ? abs(HoverDampen.Get()) : RulesClass::Instance->HoverDampen;
	}

	double GetAccel()
	{
		return abs(HoverAcceleration.Get()) > 0.0 ? abs(HoverAcceleration.Get()) : RulesClass::Instance->HoverAcceleration;
	}

	double GetBrake()
	{
		return abs(HoverBrake.Get()) > 0.0 ? abs(HoverBrake.Get()) : RulesClass::Instance->HoverBrake;
	}

	double GetBoost()
	{
		return abs(HoverBoost.Get()) > 0.0 ? abs(HoverBoost.Get()) : RulesClass::Instance->HoverBoost;
	}

	static void AddDefaults()
	{
		FindOrAllocate(NONE_STR);
	}

	static const HoverTypeClass* GetMyHover(int nIdx)
	{ return HoverTypeClass::FindFromIndex(nIdx); }

	virtual void LoadFromINI(CCINIClass* pINI) override;
	virtual void LoadFromStream(PhobosStreamReader& Stm) override;
	virtual void SaveToStream(PhobosStreamWriter& Stm) override;

private:
	template <typename T>
	void Serialize(T& Stm);
};