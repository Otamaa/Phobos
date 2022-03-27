#pragma once
#include <Utilities/TemplateDef.h>

class TechnoTypeClass;
class GiftBoxData
{
public:

	Valueable<bool> Enable;
	ValueableVector<TechnoTypeClass*> Gifts;
	DynamicVectorClass<int> Nums;
	Valueable<bool> Remove;
	Valueable<bool> Destroy;
	Valueable<int> Delay;
	Valueable<int> DelayMin;
	Valueable<int> DelayMax;
	Valueable<int> RandomRange;
	Valueable<bool> EmptyCell;
	Valueable<bool> RandomType;
	Valueable<bool> OpenWhenDestoryed;
	Nullable<Point2D> RandomDelay;

	GiftBoxData()
		: Enable { false }
		, Gifts { }
		, Nums { }
		, Remove { true }
		, Destroy { false }
		, Delay { 0 }
		, DelayMin { 0 }
		, DelayMax { 0 }
		, RandomRange { 0 }
		, EmptyCell { false }
		, RandomType { false }
		, OpenWhenDestoryed { false }
		, RandomDelay { }
	{ }

	~GiftBoxData() = default;

	bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
	{ Debug::Log("Loading Element From GiftBoxData ! \n"); return Serialize(Stm); }

	bool Save(PhobosStreamWriter& Stm)
	{ return Serialize(Stm); }

	void Read(INI_EX& parser, const char* pSection)
	{
		Gifts.Read(parser, pSection, "GiftBox.Types");

		if (!Gifts.empty())
		{
			Enable = true;
			auto const nBaseSize = (int)Gifts.size();
			Nums.Clear();
			Nums.Reserve(nBaseSize);
			Nums.Count = nBaseSize;
			auto const pNumKey = "GiftBox.Nums";

			for (auto& nSpawnMult : Nums)
				nSpawnMult = 1;

			if (parser.ReadString(pSection, pNumKey))
			{
				int nCount = 0;
				char* context = nullptr;
				for (char* cur = strtok_s(parser.value(), Phobos::readDelims, &context); cur; cur = strtok_s(nullptr, Phobos::readDelims, &context))
				{
					if (Phobos::Config::MoreDetailSLDebugLog)
						Debug::Log("Parsing %d Size of [%s]%s=%s idx[%d] \n", nBaseSize, pSection, pNumKey, cur, nCount);

					int buffer = 1;
					if (Parser<int>::TryParse(cur, &buffer))
						Nums[nCount] = buffer;

					if (++nCount >= nBaseSize)
						break;
				}
			}

			Remove.Read(parser, pSection, "GiftBox.Remove");
			Destroy.Read(parser, pSection, "GiftBox.Explodes");
			Delay.Read(parser, pSection, "GiftBox.Delay");
			RandomDelay.Read(parser, pSection, "GiftBox.RandomDelay");

			if (RandomDelay.isset() && DelayMax > 0)
			{
				DelayMin = abs(RandomDelay.Get().X);
				DelayMax = abs(RandomDelay.Get().Y);

				if (DelayMax > DelayMin)
					std::swap(DelayMin, DelayMax);
			}

			RandomRange.Read(parser, pSection, "GiftBox.RandomRange");
			EmptyCell.Read(parser, pSection, "GiftBox.RandomToEmptyCell");
			RandomType.Read(parser, pSection, "GiftBox.RandomType");
			OpenWhenDestoryed.Read(parser, pSection, "GiftBox.OpenWhenDestoryed");
		}
	}

private:
	template <typename T>
	bool Serialize(T& Stm)
	{
		return Stm
			.Process(Enable)
			.Process(Gifts)
			.Process(Nums)
			.Process(Remove)
			.Process(Destroy)
			.Process(Delay)
			.Process(DelayMin)
			.Process(DelayMax)
			.Process(RandomRange)
			.Process(EmptyCell)
			.Process(RandomType)
			.Process(OpenWhenDestoryed)
			.Process(RandomDelay)
			.Success()
			;
	}
};