#pragma once
#include <GeneralStructures.h>

class GiftBoxData;
class GiftBox
{
public:

	TechnoClass* OwnerObject;
	bool IsOpen;
	int Delay;
	TimerStruct DelayTimer;

	GiftBox() :
		OwnerObject { nullptr }
		, IsOpen { false }
		, Delay { 0 }
		, DelayTimer { }
	{ }

	GiftBox(TechnoClass* pOwner , int delay) :
		OwnerObject { pOwner }
		, IsOpen { false }
		, Delay { delay }
		, DelayTimer { }
	{
		if (delay > 0)
			DelayTimer.Start(delay);
	}

	bool CanOpen()
	{
		return !IsOpen && Timeup();
	}

	bool Timeup()
	{
		if (Delay <= 0 || DelayTimer.Expired())
		{
			IsOpen = true;
			return true;
		}
		return false;
	}

	void Reset(int nDelay)
	{
		IsOpen = false;
		Delay = nDelay;

		if (Delay > 0)
			DelayTimer.Start(nDelay);

	}

	void Release(GiftBoxData& nData);
	CellClass* GetCell(CellClass* pIn , CoordStruct& InOut, size_t nSpread, bool EmptyCell);

	bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
	{ Debug::Log("Loading Element From GiftBox ! \n"); return Serialize(Stm); }

	bool Save(PhobosStreamWriter& Stm)
	{ return Serialize(Stm); }

private:
	template <typename T>
	bool Serialize(T& Stm)
	{
		return Stm
			.Process(OwnerObject)
			.Process(IsOpen)
			.Process(Delay)
			.Process(DelayTimer)
			.Success()
			;
	}
};