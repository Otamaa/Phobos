#include "Body.h"

#include <Ext/BulletType/Body.h>

namespace HandleBounceWeapon
{
	void CreateBullet(BulletClass* pThis, CoordStruct nCoord)
	{
		if (auto pWeapon = WeaponTypeClass::FindOrAllocate("NukePayload"))
		{
			if (auto pBulletExt = BulletTypeExt::ExtMap.Find(pWeapon->Projectile))
			{
				if (auto pBullet = pBulletExt->CreateBullet(pThis->GetCell(), nullptr, pWeapon, pWeapon->Damage, true))
				{
					pBullet->SetLocation(nCoord);
					pBullet->Explode();
					pBullet->Limbo(); //Limbo
					pBullet->UnInit();
				}
			}
		}
	}

	void DetonateCurrentBullet(BulletClass* pThis , CoordStruct nObjectCoord)
	{
		pThis->SetLocation(nObjectCoord);
		pThis->Explode();
		pThis->Limbo(); //Limbo
		pThis->UnInit();
	}
};




DEFINE_HOOK(0x46794B, BulletClass_Update_CheckNearbyTechno, 0x6)
{
	GET(TechnoClass*, pCellTechno, ESI);
	GET(TechnoClass*, pTech, EAX);
	//GET(BulletClass*, pThis, EBX); //for the EXT
	bool bAllowbounce = false;

	if (pTech)
	{
		if (auto v5 = pTech->GetOwningHouse())
		{
			if (v5->IsAlliedWith(pTech))
			{
				bAllowbounce = 1;
			}
		}
	}

	int nBounceTimes = 10;
	if (nBounceTimes)
	{
		bool bBounceAlly = true;
		if (bAllowbounce && !bBounceAlly)
		{
			R->AL(0);
			return 0x467957;
		}

		bool bBounceInf = true;
		bool bBounceVehicle = true;
		auto v10 = pCellTechno->WhatAmI();
		if (bBounceInf && v10 == AbstractType::Infantry || bBounceVehicle && v10 == AbstractType::Unit)
		{
			bAllowbounce = 1;
		}
	}

	R->AL(bAllowbounce);
	return 0x467957;
}


DEFINE_HOOK(0x4678DC, BulletClass_Update_CrossingBuilding, 0x7)
{
	GET(BulletClass*, pThis, EBP);
	GET(TechnoClass*, pTechno, ESI);

	bool bAlly = false;

	if (auto pOwner = pThis->Owner)
	{
		if (pOwner == pTechno)
			return 0x4678F8;

		if (auto v5 = pTechno->Owner)
		{
			if (auto v6 = pOwner->GetOwningHouse())
			{
				bAlly = v6->IsAlliedWith(pOwner);
			}
		}
	}

	auto pExt = BulletExt::ExtMap.Find(pThis);
	if (!pExt->BounceTimes)
	{
		GET(TechnoClass*, pAnotherTechno, EAX);

		return  (pTechno == pAnotherTechno && pThis->GetHeight() < 208) ? 0x467879 : 0x4678F8;
	}

	bool bBounceBuilding = true;
	bool bBounceAlly = true;
	return (bBounceBuilding || (bAlly && bBounceAlly)) ? 0x4678F8 : 0x467879;
}
namespace HandleBounceWeapon
{
	void DetonateCurrentBullet(BulletClass* pThis, CoordStruct nObjectCoord)
	{
		pThis->SetLocation(nObjectCoord);
		pThis->Explode();
		pThis->Limbo(); //Limbo
		pThis->UnInit();
	}

	void CreateBullet(BulletClass* pThis, CoordStruct nCoord, AbstractClass* pTarget)
	{
		if (auto pWeapon = WeaponTypeClass::FindOrAllocate("NukePayload"))
			if (auto pBulletExt = BulletTypeExt::ExtMap.Find(pWeapon->Projectile))
				if (auto pBullet = pBulletExt->CreateBullet(pTarget, pThis->Owner, pWeapon, 1, true))
					DetonateCurrentBullet(pBullet, nCoord);
	}

};

DEFINE_HOOK(0x467BDB, BulletClass_Update_BounceOnSomething, 0x6)
{
	GET(BulletClass*, pThis, EBP);
	LEA_STACK(CoordStruct*, pCoord, 0x44);
	CoordStruct nCoord = *pCoord;
	auto const& pBulletExt = BulletExt::ExtMap.Find(pThis);

	GET(bool, bIsForceExplode, EBX);
	int nBounceTimes = 10;

	if (nBounceTimes)
	{
		int nY = 0;
		auto const pCell = Map.TryGetCellAt(nCoord);

		if (!pCell)
			return 0;

		auto pObject = pCell->GetSomeObject({ 0,0 }, false);

		if (!pObject)
		{
			if (pObject == pBulletExt->LastTarget)
				goto LABEL_53;

			pBulletExt->LastTarget = pObject;
			if (pObject->WhatAmI() == AbstractType::Building)
			{
				nY = pObject->GetCoords().Y;
				goto LABEL_36;
			}

			if ((!pBulletExt->LastTarget && pObject->WhatAmI() == AbstractType::Terrain)
				|| nCoord.DistanceFrom(pObject->Location) > 128.0)
			{
				nY = pObject->Location.Y;
			LABEL_53:

				if (!bIsForceExplode || pBulletExt->Bouncing && --pBulletExt->BounceTimes <= 0)
				{
					R->EBX(pBulletExt->Bouncing && pBulletExt->BounceTimes <= 0);
					return 0x0;
				}
			}
		}

	LABEL_36:
		if (nY < nCoord.Y)
			pThis->Velocity.Y = -pThis->Velocity.Y;
		else
			pThis->Velocity.X = -pThis->Velocity.X;
		//HandleBounceWeapon::CreateBullet(pThis, nCoord, pThis->GetCell());
		pThis->Velocity *= pThis->Type->Elasticity;

		if (!pBulletExt->Bouncing)
		{
			pBulletExt->BounceTimes = nBounceTimes;
			pBulletExt->Bouncing = true;
			R->EBX(false);
			return 0x467C0C;
		}

		if (pBulletExt->BounceTimes)
		{
			R->EBX(false);
			return 0x467C0C;
		}

		R->EBX(pBulletExt->Bouncing && pBulletExt->BounceTimes <= 0);
		return 0x0;
	}

	return 0;

}

DEFINE_HOOK(0x467609, BulletClass_Update_CheckBounce, 0x6)
{
	//	GET(BulletClass*, pThis, EBP);
	int nBounceTimes = 10;
	//(pThis->Type->Bouncy && nBounceTimes)
	return nBounceTimes ? 0x467615 : 0x46777A;
}

DEFINE_HOOK(0x4679CA, BulletClass_Update_CheckDistToObject, 0x5)
{
	GET(int, nDistance, EAX);
	R->AL(nDistance < 128);

	return 0x4679D7;
}

DEFINE_HOOK(0x46794B, BulletClass_Update_CheckNearbyTechno, 0x6)
{
	R->AL(true);
	return 0x467957;
}

DEFINE_HOOK(0x46786C, BulletClass_Update_ContactTarget, 0x6)
{
	//	GET(BulletClass*, pThis, EBP);
	GET(int, nCurrentHeight, EAX);
	int nBounceTimes = 10;
	//(pThis->Type->Bouncy && nBounceTimes)
	return  nBounceTimes || nCurrentHeight >= 208 ? 0x467890 : 0x467879;
}

DEFINE_HOOK(0x4678DC, BulletClass_Update_CrossingBuilding, 0x7)
{
	bool bounce = true;
	return bounce ? 0x4678F8 : 0x467879;
}

DEFINE_HOOK(0x46779B, BulletClass_Update_DetonateNow, 0x8)
{
	GET(BulletClass*, pThis, EBP);

	int nBounceTimes = 10; //bulletType
	auto pExt = BulletExt::ExtMap.Find(pThis);

	auto SetStackAndRet = [&R]()
	{
		R->Stack(0x20, 1);
		R->Stack(0x18, 1);
		return 0x4677A8;
	};

	if (!nBounceTimes)
	{
		return SetStackAndRet();
	}

	if (pExt->Bouncing)
	{
		if (--pExt->BounceTimes <= 0)
		{
			return SetStackAndRet();
		}
	}
	else
	{
		pExt->BounceTimes = 10;
		pExt->Bouncing = true;
	}

	/*
	GET_STACK(int, nCoord_Y, 0x28);
	GET_STACK(int, nCoord_X, 0x24);
	GET_STACK(int, nCoord_Z, 0x2C);

	CoordStruct nCoord { nCoord_X,nCoord_Y,nCoord_Z };

	auto pCell = MapClass::Instance->TryGetCellAt(nCoord);

	if (!pCell)
		Debug::FatalErrorAndExit(__FUNCTION__"Invalid Cell Pointer! \n");

	bool bCheckAlt = (static_cast<int>(pCell->Flags)) & 1;
	auto pObject = pCell->GetSomeObject({0,0}, bCheckAlt);
	auto pBulltTarget = pThis->Target;
	auto pBulletOwner = pThis->Owner;
	HouseClass* pObjectOwner = pObject ? pObject->GetOwningHouse() : nullptr;
	bool bIsObjectAlly = pObjectOwner ? pObjectOwner->IsAlliedWith(pBulletOwner) : false;

	if (!pObject || pObject != pBulltTarget || !bIsObjectAlly)
	{
		HandleBounceWeapon::CreateBullet(pThis, nCoord, pThis->GetCell());
		return 0x4677A8;
	}

	CoordStruct nObjectCoord = CoordStruct::Empty;

	if (auto pBuilding = specific_cast<BuildingClass*>(pObject))
		nObjectCoord = pBuilding->GetCoords();
	else
		nObjectCoord = pObject->GetCoords();
	*/

	HandleBounceWeapon::DetonateCurrentBullet(pThis, pThis->Location);

	return 0x467FEE;

}

/*

		//		GET_STACK(int, nCoord_Y, 0x48);
		//		GET_STACK(int, nCoord_X, 0x44);
		//		GET_STACK(int, nCoord_Z, 0x4C);
		//

		//		CellClass* pCell = nullptr;
		//		auto pOwner = pThis->Owner;
		//		HouseClass* pHouse = pOwner ? pOwner->Owner : nullptr;
		//		CoordStruct nCoord{ nCoord_X, nCoord_Y,nCoord_Z };
		//		bool BounceTerrain = true;
		//		bool bAlly = false;
		//		bool bBounceAlly = true;
		//		auto pExt = BulletExt::ExtMap.Find(pThis);
		//		pCell = MapClass::Instance->TryGetCellAt(nCoord);
		//		bool bCheckAlt = (static_cast<int>(pCell->Flags)) & 1;
		//		Point2D Offset{ nCoord_X , nCoord_Y };
		//		auto pObject = pCell->GetSomeObject(Offset, bCheckAlt);
		//		auto pObject_ = pObject;

if (bBounceAlly)
	goto LABEL_21;

if (pHouse)
{
	if (!pObject)
	{
	LABEL_21:
		bAlly = true;
	LABEL_22:
		if (!pObject || !bAlly || pObject == pExt->LastTarget)
		{
			if ((pExt->IsBouncer && (--pExt->BounceLeft, pExt->BounceLeft <= 0)) || !nBool)
			{
				R->EBX((pExt->IsBouncer &&pExt->BounceLeft <= 0));
				return 0;
			}
		}

		pExt->LastTarget = pObject;

		auto nObjectCoord = pObject_->GetCoords();

		if (auto pBuilding = specific_cast<BuildingClass*>(pObject))
		{
			nObjectCoord = pBuilding->GetCenterCoords(false);
			goto LABEL_36;
		}

		auto nDistance = nCoord.DistanceFromAutoMethod(nObjectCoord);

		if (!BounceTerrain && pObject->WhatAmI() == AbstractType::Terrain || nDistance >= 128.0)
		{
			if ((pExt->IsBouncer && (--pExt->BounceLeft, pExt->BounceLeft <= 0)) || !nBool)
			{
				R->EBX((pExt->IsBouncer &&pExt->BounceLeft <= 0));
				return 0;
			}
		}
	LABEL_36:
		if (nObjectCoord.Y < nCoord_Y)
			pThis->Velocity.Y = -pThis->Velocity.Y;
		else
			pThis->Velocity.X = -pThis->Velocity.X;

		HandleBounceWeapon::CreateBullet(pThis, nCoord);

		int nElasticity = 2;
		pThis->Velocity.X = pThis->Velocity.X * nElasticity;
		pThis->Velocity.Y = pThis->Velocity.Y * nElasticity;
		pThis->Velocity.Z = pThis->Velocity.Z * nElasticity;

		if (!pExt->IsBouncer)
		{
			pExt->BounceLeft = 0;
			pExt->IsBouncer = 1;
			R->EBX(0);
			return 0x467C0C;
		}

		if (pExt->BounceLeft)
		{
			R->EBX(0);
			return 0x467C0C;
		}

		R->EBX((pExt->IsBouncer &&pExt->BounceLeft <= 0));
		return 0;
	}

	auto v14 = pObject->GetOwningHouse();
	if (!v14)
	{
	LABEL_20:
		pObject = pObject_;
		goto LABEL_21;
	}
	if (v14 != pHouse)
	{
		auto v15 = v14->ArrayIndex;
		if (v15 != pHouse->ArrayIndex)
		{
			if (v15 != -1)
			{
				auto v16 = 1 << v15;
				pObject = pObject_;
				if ((v16 & pHouse->Allies) != 0)
				{
					bAlly = 0;
					goto LABEL_22;
				}
				goto LABEL_21;
			}
			goto LABEL_20;
		}
	}
	pObject = pObject_;
}
bAlly = 0;
goto LABEL_22;*/