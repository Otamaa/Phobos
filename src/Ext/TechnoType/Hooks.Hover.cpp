#include "Body.h"

#include <LocomotionClass.h>
#include <AnimClass.h>
#include <FootClass.h>

//same stuffs as TeleportLoco , but change it to fit every Locomotor that avail around 
//i do put this as macro on my offline branch 
#define GET_LOCO(reg_Loco) \
GET(ILocomotion * const, Loco, reg_Loco);\
auto const pLocomotor = static_cast<LocomotionClass*>(Loco);\
auto const Linked = pLocomotor->LinkedTo;\
auto const pType = Linked->GetTechnoType(); \
auto const pExt = TechnoTypeExt::ExtMap.Find(pType);

//End macro area

DEFINE_HOOK(513DD6, HoverLocomotionClass_513D20_HoverHeight1, 6)
{
    GET_LOCO(ESI);
    GET_STACK(int, heightGet, STACK_OFFS(0x2C, 0x18));

    auto height = pExt->HoverHeight.Get(RulesClass::Instance->HoverHeight);
    //    Debug::Log(__FUNCTION__" called.heig  [%d] , get [%d]  \n", height, heightGet);
    R->EAX(heightGet - height);

    return 0x513DDC;
}

DEFINE_HOOK(513E8F, HoverLocomotionClass_513D20_HoverHeight2, 6)
{
    GET_LOCO(ESI);
    GET_STACK(int, comparator, STACK_OFFS(0x2C, 0x14));

    auto height = pExt->HoverHeight.Get(RulesClass::Instance->HoverHeight);
    //   Debug::Log(__FUNCTION__" called.heig [%d] , comp [%d]  \n", height, comparator);

    return comparator < height ? 0x513E9D : 0x513F0E;
}

//513EAA
DEFINE_HOOK(513EAA, HoverLocomotionClass_513D20_HoverHeight3, 5)
{
    GET_LOCO(ESI);

    R->EAX(RulesClass::Instance);

    auto height = pExt->HoverHeight.Get(RulesClass::Instance->HoverHeight);
    //   Debug::Log(__FUNCTION__" called.heig [%d] \n", height);

    _asm fmul dword ptr[height];

    return 0x513EB5;
}

DEFINE_HOOK(513ECD, HoverLocomotionClass_513D20_HoverHeight4, 6)
{
    GET_LOCO(ESI);

    auto height = pExt->HoverHeight.Get(RulesClass::Instance->HoverHeight);

    R->ECX(RulesClass::Instance);
    R->EAX(height);
    //  Debug::Log(__FUNCTION__" called.heig [%d] \n", height);

    return 0x513ED9;
}

DEFINE_HOOK(513F1B, HoverLocomotionClass_513D20_HoverDampen, 6)
{
    GET_LOCO(ESI);

    auto dampen = pExt->HoverDampen.Get(RulesClass::Instance->HoverDampen);
    //   Debug::Log(__FUNCTION__" called.dampen [%fl] \n", dampen);

   _asm fmul qword ptr[dampen];

    return 0x513F27;
}

DEFINE_HOOK(513E14, HoverLocomotionClass_513D20_HoverBob, 6)
{
    R->ECX(Unsorted::CurrentFrame); //Uhh ...
    GET_LOCO(ESI);

    auto bob = pExt->HoverBob.Get(RulesClass::Instance->HoverBob);
    // Debug::Log(__FUNCTION__" called.bob [%fl] \n", bob);

   _asm fmul qword ptr[bob];

    return 0x513E20;
}

DEFINE_HOOK(514A65, HoverLocomotionClass_513D20_AboveWaterWater, B)
{
    GET_LOCO(ESI);

    //  Debug::Log(__FUNCTION__" called.Type [%s] \n", pType->ID);
    if (Linked)
    {
        auto Anim = pExt->AboveWaterAnim.Get(RulesClass::Instance->Wake);
        // Debug::Log(__FUNCTION__" called.Anim [%s] \n", Anim->ID);

        //are you guys want to hide them when cloaked ?
        if (Anim /*&& (Linked->CloakState != CloakState::Cloaked || Linked->CloakState != CloakState::Cloaking)*/)
        {
            auto pAnim = GameCreate<AnimClass>(Anim, Linked->GetCoords(), 0, 1, 0x600, false, false);
            pAnim->Owner = Linked->Owner; //special XD
        }
    }

    return 0x514AC8;
}

//============================================================================================

DEFINE_HOOK(515D55, HoverLocomotionClass_514F70_BlockedPathDelay, 6)
{
    GET_LOCO(EBX);
    // GET(UnitClass*, pUnit, EAX);

    auto PathDelay = pExt->BlockPathDelay.Get(RulesClass::Instance->BlockagePathDelay);
    //  Debug::Log(__FUNCTION__" called.PathDelay [%d] \n", PathDelay);
    R->ECX(PathDelay);

    return 0x515D5B;
}

DEFINE_HOOK(515870, HoverLocomotionClass_514F70_CloseEnough, 6)
{
    GET_LOCO(EBX);
    GET(int, comparator, EAX);

    auto Rules = RulesClass::Instance->CloseEnough;

    if (pExt->CloseEnough.isset())
        Rules = static_cast<int>(pExt->CloseEnough.Get() * 256.0);
    //    Debug::Log(__FUNCTION__" called.close [%d] , comp [%d]  \n", close, comparator);

    return comparator < Rules ? 0x51587C : 0x515902;
}

//============================================================================================

DEFINE_HOOK(51676F, HoverLocomotionClass_5164D0_CloseEnough_B, 6)
{
    GET_LOCO(ESI);
    GET(int, comparator, EAX);

    auto Rules = RulesClass::Instance->CloseEnough;

    if (pExt->CloseEnough.isset())
        Rules = static_cast<int>(pExt->CloseEnough.Get() * 256.0);
    //   Debug::Log(__FUNCTION__" called.closeB [%d] , comp [%d]  \n", close, comparator);

    return comparator < Rules ? 0x51677D : 0x5167BF;
}

DEFINE_HOOK(516179, HoverLocomotionClass_515ED0_HoverAccel, 6)
{
    GET_LOCO(ESI);

    auto accel = pExt->HoverAcceleration.Get(RulesClass::Instance->HoverAcceleration);
    // Debug::Log(__FUNCTION__" called.accel [%fl] \n", accel);

    _asm fld qword ptr[accel];

    return 0x516185;
}


DEFINE_HOOK(5161B1, HoverLocomotionClass_515ED0_HoverBrake, 6)
{
    GET_LOCO(ESI);

    auto brake = pExt->HoverBrake.Get(RulesClass::Instance->HoverBrake);
    //    Debug::Log(__FUNCTION__" called.brake [%fl] \n", brake);

    _asm fld qword ptr[brake];

    return 0x5161BD;
}

DEFINE_HOOK(516690, HoverLocomotionClass_515ED0_HoverPathDelay, 6)
{
    R->EBX(Unsorted::CurrentFrame); //uhh,..
    GET_LOCO(ESI);

    auto pDelay = pExt->PathDelay.Get(RulesClass::Instance->PathDelay);
    //  Debug::Log(__FUNCTION__" called.pDelay [%fl] \n", pDelay);

    _asm fld qword ptr[pDelay];

    return 0x51669C;
}

DEFINE_HOOK(5167FC, HoverLocomotionClass_515ED0_ScoldSound, 5)
{
    GET_LOCO(ESI);

    auto sound = pExt->ScoldSound.Get(RulesClass::Instance->ScoldSound);
    // Debug::Log(__FUNCTION__" called.SoundIDx [%d] \n", sound);

    if (sound)
        VocClass::PlayGlobal(sound, 0x2000, 1.0, nullptr);

    return 0x516818;
}

/*Disabled , Reason : Require Spesific macros or HoverLoco Class to be defined
//skip boost
DEFINE_LJMP(0x516147, 0x51614C);

//set boost
DEFINE_HOOK(51614F, HoverLocomotionClass_515ED0_HoverBoost,6)
{
    GET_LOCO(ESI);
    GET_SPESIFICFROMESI(double, boost, 0x58); //try get ESI+offs same like GET/LEA STACK
                                              //since HoverLoco Class is not defined yet
                                              //Require custom macros on Syringe.h

    auto vboost = pExt->HoverBoost.Get(RulesClass::Instance->HoverBoost);
    boost += vboost;

    SET_SPESIFICFROMESI(boost, 0x58); //set boost here
                                     //try set ESI+offs same like SET STACK do
                                     //since HoverLoco Class is not defined yet
                                     //Require custom macros on Syringe.h
   // SET_FLD_B(boost);

  //  return 0x516155;
    return 0;
}
*/

#undef GET_LOCO