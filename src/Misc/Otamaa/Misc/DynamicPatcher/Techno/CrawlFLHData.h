#pragma once

#include <Utilities/TemplateDef.h>

struct CrawlingFLHData
{
	Nullable<CoordStruct> PrimaryCrawlFLH;
	Nullable<CoordStruct> Elite_PrimaryCrawlFLH;
	Nullable<CoordStruct> SecondaryCrawlFLH;
	Nullable<CoordStruct> Elite_SecondaryCrawlFLH;

	void Read(INI_EX& nParser, const char* pSection)
	{
		PrimaryCrawlFLH.Read(nParser, pSection, "PrimaryCrawlingFLH");
		Elite_PrimaryCrawlFLH.Read(nParser, pSection, "ElitePrimaryCrawlingFLH");

		SecondaryCrawlFLH.Read(nParser, pSection, "SecondaryCrawlingFLH");
		Elite_SecondaryCrawlFLH.Read(nParser, pSection, "EliteSecondaryCrawlingFLH");
	}

	bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
	{ Debug::Log("Loading Element From CrawlingFLHData ! \n");  return Serialize(Stm); }

	bool Save(PhobosStreamWriter& Stm)
	{ return Serialize(Stm); }

private:
	template <typename T>
	bool Serialize(T& Stm)
	{
		return Stm
			.Process(PrimaryCrawlFLH)
			.Process(Elite_PrimaryCrawlFLH)
			.Process(SecondaryCrawlFLH)
			.Process(Elite_SecondaryCrawlFLH)
			.Success()
			;
	}
};