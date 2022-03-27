#include "Body.h"


namespace Log
{
	bool disalbed = !Phobos::Config::MoreDetailSLDebugLog;
}

DEFINE_HOOK(0x691518, ScriptClass_GetCurrentAction_extra, 0x7)
{
	GET(ScriptClass*, pThis, ECX);
	GET_STACK(ScriptActionNode*, pNode, 0x4);
	GET(int, nCurIndex, EDX);

	auto pTypeExt = ScriptTypeExt::ExtMap.Find(pThis->Type);

	pNode->Action = pTypeExt->PhobosNode[nCurIndex].Action;
	pNode->Argument = pTypeExt->PhobosNode[nCurIndex].Argument;

	/*
	if (nCurIndex <= 50)
	{
		pNode->Action = pThis->Type->ScriptActions[nCurIndex].Action;
		pNode->Argument = pThis->Type->ScriptActions[nCurIndex].Argument;
	}
	else
	{
		if (auto pTypeExt = ScriptTypeExt::ExtMap.Find(pThis->Type))
		{
			if (!pTypeExt->PhobosNode.empty())
			{
				nCurIndex -= 51;
				pNode->Action = pTypeExt->PhobosNode[nCurIndex].Action;
				pNode->Argument = pTypeExt->PhobosNode[nCurIndex].Argument;
			}
		}
	}*/

	R->EAX(pNode);
	return 0x691534;
}

DEFINE_HOOK(0x691566, ScriptClass_GetNextAction_extra, 0xB)
{
	GET(ScriptTypeClass*, pType, EDX);
	GET_STACK(ScriptActionNode*, pNode, STACK_OFFS(0x4, -0x4));
	GET(int, nCurIndex, ECX);
	auto pTypeExt = ScriptTypeExt::ExtMap.Find(pType);

	nCurIndex += 1;

	pNode->Action = pTypeExt->PhobosNode[nCurIndex].Action;
	pNode->Argument = pTypeExt->PhobosNode[nCurIndex].Argument;

	/*
	if (nCurIndex <= 50)
	{
		pNode->Action = pType->ScriptActions[nCurIndex].Action;
		pNode->Argument = pType->ScriptActions[nCurIndex].Argument;
	}
	else
	{
		if (auto pTypeExt = ScriptTypeExt::ExtMap.Find(pType))
		{
			if (!pTypeExt->PhobosNode.empty())
			{
				nCurIndex -= 51;
				pNode->Action = pTypeExt->PhobosNode[nCurIndex].Action;
				pNode->Argument = pTypeExt->PhobosNode[nCurIndex].Argument;
			}
		}
	}*/

	R->EAX(pNode);
	return 0x69157D;
}

DEFINE_HOOK(0x6918CA, ScriptTypeClass_LoadFromINI, 0x5)
{
	GET(ScriptTypeClass*, pThis, ESI);
	//GET_STACK(DWORD, nCaller, 0xA8);
	LEA_STACK(char*, pBuffer, STACK_OFFS(0xA0, 0x90));
	GET(CCINIClass*, pRules, EBP);

	auto pExt = ScriptTypeExt::ExtMap.Find(pThis);
	//Debug::Log_WithBool(Log::enabled, __FUNCTION__" pThis = %08X[%s], pTypeExt = %08X, caller = %08X\n", pThis, pThis->ID, pExt, nCaller);

	pThis->ActionsCount = 0;
	auto nCount = pRules->GetKeyCount(pThis->ID);

	if (nCount > 0)
		nCount -= 1; //name

	Debug::Log_WithBool(!Phobos::Config::MoreDetailSLDebugLog, __FUNCTION__" [%s] count after name = %d\n", pThis->ID, nCount);

	if (nCount > 0)
	{
		for (int i = 0; i < nCount; ++i)
		{
			CRT::sprintf(pBuffer, "%d", i);
			CRT::strtrim(pBuffer);
			if (pRules->ReadString(pThis->ID, pBuffer, "", Phobos::readBuffer))
			{
				ScriptActionNode nBuffer;
				nBuffer.FillIn(Phobos::readBuffer);
				//if (i <= 50)
				//{
				//	pThis->ScriptActions[i].Action = nBuffer.Action;
				//	pThis->ScriptActions[i].Argument = nBuffer.Argument;
				//}
				//else
				{
					pExt->PhobosNode.emplace_back(nBuffer);
				}

				//Debug::Log_WithBool(!Phobos::Config::MoreDetailSLDebugLog, __FUNCTION__" type[%s] parsing index[%d] \n", pThis->ID, i);
				Debug::Log(__FUNCTION__" type[%s] parsing index[%d] \n", pThis->ID, i);
				++pThis->ActionsCount;
			}
			else
			{
				Debug::Log_WithBool(!Phobos::Config::MoreDetailSLDebugLog, __FUNCTION__" type[%s] failed to parse action at index[%d] \n",
				pThis->ID,
				i);
			}
		}
	}

	pExt->LoadFromINI(pRules);

	return 0x691953;
}

/*
DEFINE_HOOK(0x691A06, ScriptTypeClass_LoadFromINIList_Debug, 0x6)
{
	GET(int, nIdx, ESI);

	auto pScript = ScriptTypeClass::Array.get()->GetItem(nIdx);
	Debug::Log_WithBool(Log::enabled," Index[%d] %08X [%s]\n", nIdx, pScript, pScript->ID);

	return 0;
}*/