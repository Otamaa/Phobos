#pragma once

#include <Utilities/TemplateDef.h>
#include <Utilities/Container.h>

#include <LaserDrawClass.h>
#include <TechnoClass.h>

struct LaserDrawExt
{
	using base_type = LaserDrawClass;

	class ExtData final : public Extension<base_type>
	{
	public:
		AbstractClass* Target;
		TechnoClass* Owner;

		bool Laser_MultiColors; //0xE
		bool Laser_RealTimePosition; //0xC
		bool Laser_ApplyColorFix;
		CoordStruct Overrider;
		CoordStruct FLH;
		int FramePasses;

		ExtData(base_type* pOwnerObject) : Extension<base_type>(pOwnerObject)
			, Target { nullptr }
			, Owner { nullptr }
			, Laser_MultiColors { false }
			, Laser_RealTimePosition { false }
			, Laser_ApplyColorFix { false }
			, Overrider { CoordStruct::Empty }
			, FLH { CoordStruct::Empty }
			, FramePasses { -1 }
		{}

		virtual ~ExtData() override { }

		virtual void InvalidatePointer(void* ptr, bool bRemoved) override;
		virtual size_t Size() const { return sizeof(*this); }
		virtual void LoadFromStream(PhobosStreamReader& Stm) override;
		virtual void SaveToStream(PhobosStreamWriter& Stm) override;

	private:
		template <typename T>
		void Serialize(T& Stm)
		{
			Stm
				.Process(Target)
				.Process(Owner)
				.Process(Laser_MultiColors)
				.Process(Laser_RealTimePosition)
				.Process(Laser_ApplyColorFix)
				.Process(Overrider)
				.Process(FLH)
				;
		}
	};

	class ExtContainer final : public Container<LaserDrawExt>
	{
	public:
		ExtContainer();
		~ExtContainer();
	};

	static ExtContainer ExtMap;
	static std::map<TechnoClass*, LaserDrawClass*> WhoCalledMe;

	bool LoadGlobals(PhobosStreamReader& Stm);
	bool SaveGlobals(PhobosStreamWriter& Stm);

	static double CalculateColor(int nPass, int nFrame);
	static void Exec(LaserDrawClass* pLaser);
};

class LaserDrawClassCopy final : public LaserDrawClass
{
public:

	LaserDrawClass* _LaserDrawClass_CTOR
	(
		CoordStruct source,
		CoordStruct target,
		int zAdjust,
		BYTE unknown,
		ColorStruct innerColor,
		ColorStruct outerColor,
		ColorStruct outerSpread,
		int duration,
		bool blinks,
		bool fades,
		float startIntensity,
		float endIntensity
	)
	{
		JMP_THIS(0x54FE60);
	}
};