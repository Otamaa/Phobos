#pragma once
#include <TechnoClass.h>
#include <BulletClass.h>
#include <Utilities/Constructs.h>
#include "RadialFire.h"
#include "EffectHelpers.h"
//#include <Misc/Otamaa/Delegates.h>

#include <unordered_set>
//typedef Delegate<BulletClass*> FoundBullet;
//typedef Delegate<TechnoClass*> FoundTechno;
//typedef Delegate<AircraftClass*> FoundAircraft;

struct Helpers_DP
{
	static bool DamageMe(TechnoClass* pThis, int damage, int distanceFromEpicenter, WarheadTypeClass* warheadType, int& realDamage, bool effectsRequireDamage = false)
	{
		// 计算实际伤害
		if (damage > 0)
		{
			realDamage = MapClass::GetTotalDamage(damage, warheadType, pThis->GetType()->Armor, distanceFromEpicenter);
		}
		else
		{
			realDamage = -MapClass::GetTotalDamage(-damage, warheadType, pThis->GetType()->Armor, distanceFromEpicenter);
		}
		/*
		if (true)
		{
			if (damage == 0)
			{
				return warheadTypeExt.Ares.AllowZeroDamage;
			}
			else
			{
				if (warheadTypeExt.Ares.EffectsRequireVerses)
				{
					// 必须要可以造成伤害
					if (MapClass.GetTotalDamage(RulesClass.Global().MaxDamage, warheadTypeExt.OwnerObject, OwnerObject.Ref.Base.Type.Ref.Armor, 0) == 0)
					{
						// 弹头无法对该类型护甲造成伤害
						return false;
					}
					// 伤害非零，当EffectsRequireDamage=yes时，必须至少造成1点实际伤害
					if (effectsRequireDamage || warheadTypeExt.Ares.EffectsRequireDamage)
					{
						// Logger.Log("{0} 收到伤害 {1}, 弹头 {2}, 爆心距离{3}, 实际伤害{4}", OwnerObject.Ref.Type.Ref.Base.Base.ID, damage, warheadTypeExt.OwnerObject.Ref.Base.ID, distanceFromEpicenter, realDamage);
						return realDamage != 0;
					}
				}
			}
		}*/
		return true;
	}

	static CoordStruct OneCellOffsetToTarget(CoordStruct sourcePos, CoordStruct targetPos)
	{
		double angle = Math::atan2((double)(targetPos.Y - sourcePos.Y), (double)(targetPos.X - sourcePos.X));
		int y = (int)(256 * Math::tan(angle));
		int x = (int)(256 / Math::tan(angle));
		CoordStruct offset = CoordStruct::Empty;
		if (y == 0)
		{
			offset.Y = 0;
			if (angle < Math::Pi)
			{
				offset.X = 256;
			}
			else
			{
				offset.X = -256;
			}
		}
		else if (x == 0)
		{
			offset.X = 0;
			if (angle < 0)
			{
				offset.Y = -256;
			}
			else
			{
				offset.Y = 256;
			}
		}
		else
		{
			if (abs(x) <= 256)
			{
				offset.X = x;
				if (angle > 0)
				{
					offset.Y = 256;
				}
				else
				{
					offset.X = -offset.X;
					offset.Y = -256;
				}
			}
			else
			{
				offset.Y = y;
				if (abs(angle) < 0.5 * Math::Pi)
				{
					offset.X = 256;
				}
				else
				{
					offset.X = -256;
					offset.Y = -offset.Y;
				}
			}
		}
		return offset;
	}

	static int ColorAdd2RGB565(ColorStruct colorAdd)
	{
		return ((((colorAdd.R + 4)) / 255) << 5) +
			((((colorAdd.G + 2)) / 255) << 6) +
			((((colorAdd.B + 4)) / 255) << 5);
	}

	static int Dir2FacingIndex(DirStruct dir, int facing)
	{
		size_t bits = (size_t)std::round(std::sqrt(facing));
		double face = (double)dir.GetValue(bits);
		double x = (face / (1 << (int)bits)) * facing;
		int index = (int)std::round(x);
		return index;
	}

	static double GetROFMult(TechnoClass* pTechno)
	{
		bool rofAbility = false;
		if (pTechno->Veterancy.IsElite())
		{
			rofAbility = pTechno->GetTechnoType()->VeteranAbilities.ROF
				|| pTechno->GetTechnoType()->EliteAbilities.ROF;
		}
		else if (pTechno->Veterancy.IsVeteran())
		{
			rofAbility = pTechno->GetTechnoType()->VeteranAbilities.ROF;
		}
		return !rofAbility ? 1.0 : RulesGlobal->VeteranROF * ((!pTechno->Owner || !pTechno->Owner->Type) ? 1.0 : pTechno->Owner->Type->ROFMult);
	}

	static DirStruct DirNormalized(int index, int facing)
	{
		double radians = Math::deg2rad_Alternate((-360 / facing * index));
		return DirStruct((short)(radians / Math::BINARY_ANGLE_MAGIC_ALTERNATE));
	}

	static DirStruct Radians2Dir(double radians)
	{
		return DirStruct((short)(radians / Math::BINARY_ANGLE_MAGIC_ALTERNATE));
	}

	static bool IsDead(TechnoClass* pTechno)
	{
		return !pTechno || IsDead((ObjectClass*)pTechno) || pTechno->IsCrashing || pTechno->IsSinking;
	}

	static bool IsDead(ObjectClass* pObject)
	{
		return !pObject || pObject->Health <= 0 || !pObject->IsAlive;
	}

	static bool IsInvisible(TechnoClass* pTechno)
	{
		return !pTechno || IsInvisible((ObjectClass*)pTechno)
			|| pTechno->CloakState == CloakState::Cloaked
			|| pTechno->CloakState == CloakState::Cloaking;
	}

	static bool IsInvisible(ObjectClass* pObject)
	{
		return pObject || pObject->InLimbo;
	}

	static bool IsDeadOrInvisible(TechnoClass* pTarget)
	{
		return IsDead(pTarget) || IsInvisible(pTarget);
	}

	static bool IsDeadOrInvisible(BulletClass* pBullet)
	{
		ObjectClass* pObject = (ObjectClass*)pBullet;
		return IsDead(pObject) || IsInvisible(pObject);
	}

	static bool IsDeadOrStand(TechnoClass* pTarget)
	{
		// 检查死亡和发射者
		if (!pTarget || IsDeadOrInvisible(pTarget) || pTarget->IsImmobilized)
			return true;

		// 过滤掉替身 , ignore stand
		//TechnoExt targetExt = TechnoExt.ExtMap.Find(pTarget);
		//if (null == targetExt || !targetExt.MyMaster.IsNull)
		//	return true;

		return false;
	}

	static CoordStruct GetFLHAbsoluteCoords(TechnoClass* pTechno, CoordStruct flh, bool isOnTurret = true, int flipY = 1, bool nextFrame = true);

	static CoordStruct GetFLH(CoordStruct source, CoordStruct flh, DirStruct dir, bool flip = false)
	{
		CoordStruct res = source;

		if (flh)
		{
			double radians = dir.radians();

			double rF = flh.X;
			double xF = rF * Math::cos(-radians);
			double yF = rF * Math::sin(-radians);
			CoordStruct offsetF = { (int)xF, (int)yF, 0 };

			double rL = flip ? flh.Y : -flh.Y;
			double xL = rL * Math::sin(radians);
			double yL = rL * Math::cos(radians);
			CoordStruct offsetL = { (int)xL, (int)yL, 0 };

			CoordStruct nZFLHBuff { 0, 0, flh.Z };
			res = source + offsetF + offsetL + nZFLHBuff;
		}

		return res;
	}

	static Vector3D<float> GetForwardVector(TechnoClass* pTechno, bool getTurret = false)
	{
		FacingStruct facing = getTurret ? pTechno->SecondaryFacing : pTechno->PrimaryFacing;
		auto nDir = facing.current();
		return ToVector3D(nDir);
	}

	static Vector3D<float> ToVector3D(DirStruct& dir)
	{
		double rad = -dir.radians();
		auto nRet = Vector3D<float> { (float)Math::cos(rad), (float)Math::sin(rad), 0.0f };
		return nRet;
	}

	static Matrix3D GetMatrix3D(TechnoClass* pTechno)
	{
		// Step 1: get body transform matrix
		Matrix3D matrix3D;

		bool bFound = false;
		if (auto const pFoot = abstract_cast<FootClass*>(pTechno))
		{
			if (auto const pLoco = pFoot->Locomotor.get())
			{
				bFound = true;
				matrix3D = pLoco->Draw_Matrix(nullptr);
			}
		}

		if (!bFound)
			matrix3D.MakeIdentity();

		return matrix3D;
	}

	static Matrix3D RotateMatrix3D(Matrix3D& matrix3D, TechnoClass* pTechno, bool isOnTurret, bool nextFrame)
	{
		if (isOnTurret)
		{
			if (pTechno->HasTurret())
			{
				DirStruct turretDir = nextFrame ? pTechno->SecondaryFacing.next() : pTechno->SecondaryFacing.current();

				if (pTechno->WhatAmI() == AbstractType::Building)
				{
					double turretRad = turretDir.radians();
					matrix3D.RotateZ((float)turretRad);
				}
				else
				{
					matrix3D.RotateZ(-matrix3D.GetZRotation());
					matrix3D.RotateZ((float)turretDir.radians());
				}
			}
		}
		else if (nextFrame)
		{
			matrix3D.RotateZ(-matrix3D.GetZRotation());
			matrix3D.RotateZ((float)pTechno->PrimaryFacing.next().radians());
		}
		return matrix3D;
	}

	static Vector3D<float> GetFLHOffset(Matrix3D& matrix3D, CoordStruct flh)
	{
		matrix3D.Translate((float)flh.X, (float)flh.Y, (float)flh.Z);
		Vector3D<float> result = Matrix3D::MatrixMultiply(matrix3D, Vector3D<float>{0.0f, 0.0f, 0.0f });
		result.Y *= -1;
		return result;
	}

	static CoordStruct GetFLHAbsoluteCoords(TechnoClass* pTechno, CoordStruct flh, bool isOnTurret, int flipY, CoordStruct turretOffset, bool nextFrame)
	{
		auto const nCoord = pTechno->GetCoords();
		Vector3D<float> res = { (float)nCoord.X, (float)nCoord.Y, (float)nCoord.Z };

		CoordStruct sourceOffset = turretOffset;

		if (nextFrame && pTechno->WhatAmI() != AbstractType::Building)
		{
			if (FootClass* pFoot = (FootClass*)pTechno)
			{
				CoordStruct nBuffer { 0,0,0 };
				int speed = 0;
				if (pFoot->Locomotor->Is_Moving() && (speed = pFoot->GetCurrentSpeed()) > 0)
				{
					nBuffer.X = speed;
					sourceOffset += nBuffer;
				}
			}
		}

		if (flh)
		{
			Matrix3D matrix3D = GetMatrix3D(pTechno);
			matrix3D.Translate((float)turretOffset.X, (float)turretOffset.Y, (float)turretOffset.Z);
			RotateMatrix3D(matrix3D, pTechno, isOnTurret, nextFrame);

			CoordStruct tempFLH = flh;
			if (pTechno->WhatAmI() == AbstractType::Building)
			{
				tempFLH.Z += Unsorted::LevelHeight;
			}

			tempFLH.Y *= flipY;
			Vector3D<float> offset = GetFLHOffset(matrix3D, tempFLH);
			// Step 5: offset techno location
			res += offset;
		}
		return { (int)res.X, (int)res.Y, (int)res.Z };
	}

	static DirStruct Point2Dir(CoordStruct sourcePos, CoordStruct targetPos)
	{
		// get angle
		double radians = Math::atan2((double)(sourcePos.Y - targetPos.Y), (double)(targetPos.X - sourcePos.X));
		// Magic form tomsons26
		radians -= Math::deg2rad_Alternate(90);
		return DirStruct((short)(radians / Math::BINARY_ANGLE_MAGIC_ALTERNATE));
	}

	static Vector3D<float> GetFLHAbsoluteOffset(CoordStruct flh, DirStruct dir, CoordStruct turretOffset)
	{
		Vector3D<float> offset = Vector3D<float>::Empty;
		if (flh)
		{
			Matrix3D matrix3D;
			matrix3D.MakeIdentity();
			matrix3D.Translate((float)turretOffset.X, (float)turretOffset.Y, (float)turretOffset.Z);
			matrix3D.RotateZ((float)dir.radians());
			offset = GetFLHOffset(matrix3D, flh);
		}
		return offset;
	}

	static BulletVelocity GetBulletVelocity(CoordStruct sourcePos, CoordStruct targetPos)
	{
		BulletVelocity nRet = BulletVelocity::Empty;
		CoordStruct bulletFLH { 1, 0, 0 };
		DirStruct bulletDir = Point2Dir(sourcePos, targetPos);
		Vector3D<float> bulletV = GetFLHAbsoluteOffset(bulletFLH, bulletDir, CoordStruct::Empty);
		nRet = { (double)bulletV.X , (double)bulletV.Y , (double)bulletV.Z };
		return nRet;
	}

	static CoordStruct GetFLHAbsoluteCoords(CoordStruct source, CoordStruct flh, DirStruct dir, CoordStruct turretOffset = CoordStruct::Empty)
	{
		CoordStruct res = source;
		if (flh)
		{
			Vector3D<float> offset = GetFLHAbsoluteOffset(flh, dir, turretOffset);
			res += CoordStruct { (int)offset.X,(int)offset.Y,(int)offset.Z };
		}
		return res;
	}

	/*
	static void FindBulletTargetHouse(TechnoClass* pTechno, FoundBullet& func, bool allied = true)
	{
		func.BindLambda([pTechno, &allied]()
		{
			auto bullets = *BulletClass::Array();
			for (int i = bullets.Count - 1; i >= 0; i--)
			{
				auto const pBullet = bullets.GetItem(i);
				if (IsDeadOrInvisible(pBullet)
					|| pBullet->Type->Inviso
					|| !pBullet->Owner || pBullet->Owner->Owner == pTechno->Owner
					|| (allied && pBullet->Owner->Owner->IsAlliedWith(pTechno->Owner)))
				{
					continue;
				}
				else
					return pBullet;
			}

			return (BulletClass*)nullptr;
		});
	}

	static void FindBulletTargetMe(TechnoClass* pTechno, FoundBullet& func, bool allied = true)
	{
		FindBulletTargetHouse(pTechno, func, allied);
		if (auto const pBullet = func.Execute())
		{
			func.BindLambda([pBullet, pTechno, &allied]()
			{
				if (pBullet->Target == pTechno)
					return pBullet;
				else
					return (BulletClass*)nullptr;
			});
		}
	}

	static void FindTechno(HouseClass* pHouse, FoundTechno& func, bool owner = true, bool allied = false, bool enemies = false, bool civilian = false)
	{
		func.BindLambda([pHouse, &owner, &allied, &enemies, &civilian]()
		{
			auto technos = *TechnoClass::Array();
			for (int i = technos.Count - 1; i >= 0; i--)
			{
				auto pTechno = technos.GetItem(i);
				if (IsDeadOrInvisible(pTechno)
					|| !pTechno->Owner
					|| (pTechno->Owner == pHouse ? !owner : (pTechno->Owner->IsAlliedWith(pHouse) ? !allied : !enemies)))
				{
					continue;
				}
				else
					return pTechno;
			}

			return (TechnoClass*) nullptr;
		});
	}

	static void FindOwnerTechno(HouseClass* pHouse, FoundTechno& func, bool allied = false, bool enemies = false)
	{
		FindTechno(pHouse, func, true, allied, enemies);
		func.Execute();
	}

	static void FindAircraft(HouseClass* pHouse, FoundAircraft& func, bool owner = true, bool allied = false, bool enemies = false, bool civilian = false)
	{
		func.BindLambda([pHouse, &owner, &allied, &enemies, &civilian]()
		{
			auto const aircrafts = *AircraftClass::Array();
			for (int i = aircrafts.Count - 1; i >= 0; i--)
			{
				auto pAircraft = aircrafts.GetItem(i);
				if (IsDeadOrInvisible(pAircraft)
					|| !pAircraft->Owner
					|| (pAircraft->Owner == pHouse ? !owner : (pAircraft->Owner->IsAlliedWith(pHouse) ? !allied : !enemies)))
				{
					continue;
				}
				else
					return pAircraft;
			}

			return (AircraftClass*) nullptr;
		});
	}*/

	static void FireWeaponTo(TechnoClass* pShooter, TechnoClass* pAttacker, AbstractClass* pTarget, WeaponTypeClass* pWeapon, CoordStruct flh, CoordStruct bulletSourcePos = CoordStruct::Empty, bool radialFire = false, int splitAngle = 180);

	static double GetDamageMult(TechnoClass* pTechno)
	{
		if (!pTechno || !pTechno->IsAlive)
			return 1.0;

		bool firepower = false;
		if (pTechno->Veterancy.IsElite())
		{
			firepower = pTechno->GetTechnoType()->VeteranAbilities.FIREPOWER || pTechno->GetTechnoType()->EliteAbilities.FIREPOWER;
		}
		else if (pTechno->Veterancy.IsVeteran())
		{
			firepower = pTechno->GetTechnoType()->VeteranAbilities.FIREPOWER;
		}
		return (!firepower ? 1.0 : RulesClass::Instance->VeteranCombat) * pTechno->FirepowerMultiplier * ((!pTechno->Owner || !pTechno->Owner->Type) ? 1.0 : pTechno->Owner->Type->FirepowerMult);
	}

	static void DrawBulletEffect(WeaponTypeClass* pWeapon, CoordStruct sourcePos, CoordStruct targetPos, TechnoClass* pAttacker, AbstractClass* pTarget)
	{
		// IsLaser
		if (pWeapon->IsLaser)
		{
			LaserType laserType = LaserType(false);
			ColorStruct houseColor = ColorStruct::Empty;
			if (pWeapon->IsHouseColor && pAttacker)
				houseColor = pAttacker->Owner->LaserColor;

			laserType.InnerColor = pWeapon->LaserInnerColor;
			laserType.OuterColor = pWeapon->LaserOuterColor;
			laserType.OuterSpread = pWeapon->LaserOuterSpread;
			laserType.IsHouseColor = pWeapon->IsHouseColor;
			laserType.Duration = pWeapon->LaserDuration;
			/*
			WeaponTypeExt ext = WeaponTypeExt.ExtMap.Find(pWeapon);
			if (null != ext)
			{
				if (ext.LaserThickness > 0)
				{
					laserType.Thickness = ext.LaserThickness;
				}
				laserType.Fade = ext.LaserFade;
				laserType.IsSupported = ext.IsSupported;
			}*/

			EffectHelpers::DrawLine(sourcePos, targetPos, laserType, houseColor);
		}

		// IsRadBeam
		if (pWeapon->IsRadBeam)
		{
			RadBeamType radBeamType = RadBeamType::RadBeam;
			if (pWeapon->Warhead && pWeapon->Warhead->Temporal)
				radBeamType = RadBeamType::Temporal;

			BeamType beamType = BeamType(radBeamType);
			EffectHelpers::DrawBeam(sourcePos, targetPos, beamType);
		}

		//IsElectricBolt
		if (pWeapon->IsElectricBolt)
		{
			if (pAttacker && pTarget)
			{
				EffectHelpers::DrawBolt(pAttacker, pTarget, pWeapon, sourcePos);
			}
			else
			{
				EffectHelpers::DrawBolt(sourcePos, targetPos, pWeapon->IsAlternateColor);
			}
		}
	}

	static void AttachedParticleSystem(WeaponTypeClass* pWeapon, CoordStruct sourcePos, AbstractClass* pTarget, TechnoClass* pAttacker, CoordStruct targetPos)
	{
		//ParticleSystem
		if (auto const psType = pWeapon->AttachedParticleSystem)
			EffectHelpers::DrawParticle(psType, sourcePos, pTarget, pAttacker, targetPos, nullptr);
	}

	static void PlayReportSound(WeaponTypeClass* pWeapon, CoordStruct sourcePos)
	{
		if (pWeapon->Report.Count > 0)
		{
			int index = ScenarioGlobal->Random.RandomRanged(0, pWeapon->Report.Count - 1);
			int soundIndex = pWeapon->Report.GetItem(index);
			if (soundIndex != -1)
			{
				VocClass::PlayAt(soundIndex, sourcePos, nullptr);
			}
		}
	}

	static void DrawWeaponAnim(WeaponTypeClass* pWeapon, CoordStruct sourcePos, CoordStruct targetPos)
	{
		// Anim
		if (pWeapon->Anim.Count > 0)
		{
			int facing = pWeapon->Anim.Count;
			int index = 0;
			if (facing % 8 == 0)
			{
				index = Dir2FacingIndex(Point2Dir(sourcePos, targetPos), facing);
				index = (int)(facing / 8) + index;
				if (index >= facing)
				{
					index = 0;
				}
			}

			if (auto pAnimType = pWeapon->Anim.GetItem(index))
				GameCreate<AnimClass>(pAnimType, sourcePos);
		}
	}

	static BulletClass* FireBulletTo(TechnoClass* pAttacker, AbstractClass* pTarget, WeaponTypeClass* pWeapon, CoordStruct sourcePos, CoordStruct targetPos, BulletVelocity bulletVelocity);
	static BulletClass* FireBullet(TechnoClass* pAttacker, AbstractClass* pTarget, WeaponTypeClass* pWeapon, CoordStruct sourcePos, CoordStruct targetPos, BulletVelocity bulletVelocity);

	static TechnoClass* CreateAndPutTechno(TechnoTypeClass* pType, HouseClass* pHouse, CoordStruct location, CellClass* pCell = nullptr)
	{
		if (pType)
		{
			auto const pTechno = generic_cast<TechnoClass*>(pType->CreateObject(pHouse));
			bool UnlimboSuccess = false;

			if (!pCell && location != CoordStruct::Empty)
				pCell = Map[location];

			if (pCell)
			{
				auto occFlags = pCell->OccupationFlags;
				pTechno->OnBridge = pCell->ContainsBridge();
				++Unsorted::IKnowWhatImDoing;
				UnlimboSuccess = pTechno->Unlimbo(pCell->GetCoordsWithBridge(), Direction::E);
				--Unsorted::IKnowWhatImDoing;

				if (UnlimboSuccess)
				{
					pCell->OccupationFlags = occFlags;
					pTechno->SetLocation(location);
					return pTechno;
				}
				else
				{
					if (pTechno)
					{
						pTechno->UnInit();
						//	GameDelete(pTechno);
					}
				}
			}
		}

		return nullptr;
	}

	static ValueableVector<BulletClass*> GetCellSpreadBullets(CoordStruct location, double spread)
	{
		ValueableVector<BulletClass*> pBulletSet { };

		double dist = (spread <= 0 ? 1 : std::ceil(spread)) * 256;

		auto bullets = *BulletClass::Array();

		for (int i = bullets.Count - 1; i >= 0; i--)
		{
			if (auto const pBullet = bullets.GetItem(i))
			{
				CoordStruct targetLocation = pBullet->GetCoords();

				if (targetLocation.DistanceFrom(location) <= dist)
					if (!pBulletSet.Contains(pBullet))
						pBulletSet.push_back(pBullet);
			}
		}

		return pBulletSet;
	}

	static int CountAircraft(HouseClass* pHouse, Iterator<AircraftTypeClass*> padList)
	{
		//bool owner = true;
		//bool allied = false;
		//bool enemies = false;

		auto const pAircraftArr = *AircraftClass::Array();
		int count = std::count_if(pAircraftArr.begin(),pAircraftArr.end() , [pHouse,&padList](AircraftClass* const pAircraft){

			if (!IsDeadOrInvisible(pAircraft) && pAircraft->Owner && pAircraft->Owner == pHouse && pAircraft->Type->AirportBound)
				return padList.contains(pAircraft->Type);

			return false;
		});

		/*
		for (auto const& pAircraft : *AircraftClass::Array())
		{
			if (IsDeadOrInvisible(pAircraft)
				|| !pAircraft->Owner
				|| (pAircraft->Owner == pHouse ? !owner : (pAircraft->Owner->IsAlliedWith(pHouse) ? !allied : !enemies)))
			{ continue; }

			if (padList.contains(pAircraft->Type) && pAircraft->Type->AirportBound)
				count++;
		}*/

		return count;
	}

};