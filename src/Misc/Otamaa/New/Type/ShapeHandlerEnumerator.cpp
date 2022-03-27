#include "ShapeHandlerEnumerator.h"

//TODO: Clear handle ? , better way to load ?
Enumerable<ShapeHandlerEnumerator>::container_t Enumerable<ShapeHandlerEnumerator>::Array;

void ShapeHandlerEnumerator::FetchSHP()
{
	const char* pName = this->Name.data();

	if (GeneralUtils::IsValidString(pName))
	{
		if (!LoadedShape)
		{
			char flag[256];
			_snprintf_s(flag, 255, "%s.shp", pName);
			if (auto const pImage = FileSystem::LoadSHPFile(flag))
			{
				if (!pImage->AsReference())
					Debug::Log("SHP With name [%s] is not a reference ! \n", flag);

				LoadedShape = pImage;
			}
			else
			{ Debug::Log("Failed to find file [%s] !\n", flag); }
		}
		//else
		//{ Debug::Log("[%s] Shape Already Loaded !  \n", pName); }
	}
	else
	{ Debug::Log("[%s] Is a Invalid Name !  \n", pName); }
}