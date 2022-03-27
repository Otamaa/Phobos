#pragma once

#include <Utilities/Enumerable.h>
#include <Utilities/Template.h>

#include <IsometricTileTypeClass.h>

class AnimTrailTypeClass final : public Enumerable<AnimTrailTypeClass>
{
public:
	Valueable<AnimTypeClass*> Anim_While;
	Valueable<AnimTypeClass*> Anim_Start;
	Valueable<AnimTypeClass*> Anim_Stop;

	Valueable<bool> IgnoreVertical;
	Valueable<int> Distance;
	Valueable<int> InitialDelay;

	AnimTrailTypeClass(const char* pTitle) : Enumerable<AnimTrailTypeClass>(pTitle)
		, Anim_While { nullptr }
		, Anim_Start { nullptr }
		, Anim_Stop { nullptr }
		, IgnoreVertical { false }
		, Distance { 64 }
		, InitialDelay { 0 }
	{ }

	virtual ~AnimTrailTypeClass() override = default;

	virtual void LoadFromINI(CCINIClass* pINI) override;
	virtual void LoadFromStream(PhobosStreamReader& Stm);
	virtual void SaveToStream(PhobosStreamWriter& Stm);

private:
	template <typename T>
	void Serialize(T& Stm);
};