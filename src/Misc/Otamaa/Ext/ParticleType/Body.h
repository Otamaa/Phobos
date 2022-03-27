#pragma once
#include <ParticleTypeClass.h>

#include <Helpers/Macro.h>
#include <Utilities/Container.h>
#include <Utilities/TemplateDef.h>
#include <New/Type/LaserTrailTypeClass.h>

class ParticleTypeExt
{
public:
    using base_type = ParticleTypeClass;

    class ExtData final : public Extension<ParticleTypeClass>
    {
    public:

		//Valueable<bool> DeleteWhenReachWater;
		//Valueable<bool> ExpireAfterDamaging;
		//Valueable<AnimTypeClass*> DamagingAnim;
		//ValueableIdxVector<LaserTrailTypeClass> LaserTrail_Types;

        ExtData(ParticleTypeClass* OwnerObject) : Extension<ParticleTypeClass>(OwnerObject)
			//, DeleteWhenReachWater(false)
			//, ExpireAfterDamaging(false)
			//, DamagingAnim(nullptr)
			//, LaserTrail_Types()
        { }

        virtual ~ExtData() = default;
		virtual size_t Size() const { return sizeof(*this); };
        virtual void LoadFromINIFile(CCINIClass* pINI) override;
        virtual void InvalidatePointer(void *ptr, bool bRemoved) override {}
        virtual void LoadFromStream(PhobosStreamReader& Stm)override;
        virtual void SaveToStream(PhobosStreamWriter& Stm)override;

    private:
        template <typename T>
        void Serialize(T& Stm);
    };

    class ExtContainer final : public Container<ParticleTypeExt>
    {
    public:
        ExtContainer();
        ~ExtContainer();
		virtual void InvalidatePointer(void* ptr, bool bRemoved) override;
    };

    static ExtContainer ExtMap;

	static bool LoadGlobals(PhobosStreamReader& Stm);
	static bool SaveGlobals(PhobosStreamWriter& Stm);
};