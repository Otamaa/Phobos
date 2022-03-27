#include <IsometricTileTypeClass.h>
#include <CCFileClass.h>
#include <Utilities/Debug.h>
#include <Utilities\Macro.h>

namespace IsoTileType
{
	int __fastcall Safeguard(IsometricTileTypeClass* pThis)
	{
		bool available = false;
		off_t file_size = 0;
		auto Filename = pThis->ID;

		if (auto file = GameCreate<CCFileClass>(Filename))
		{
			available = file->Exists();
			file_size = file->GetFileSize();
			GameDelete(file);
		}

		if (!available)
		{
			Debug::Log("Isometric Tile %s is missing!\n", Filename);
			return 0;
		}

		if (file_size == 0)
		{
			Debug::Log("Isometric Tile %s is a empty file!\n", Filename);
			return 0;
		}

		off_t read_size = pThis->LoadTile();

		if (pThis->Image == nullptr)
		{
			Debug::Log("Failed to load image for Isometric Tile %s!\n", Filename);
			return 0;
		}

		if (read_size != file_size)
		{
			Debug::FatalErrorAndExit("Isometric Tile %s file size %d doesn't match read size!\n", file_size, read_size, Filename);
		}

		return read_size;

	}
}

DEFINE_POINTER_CALL(0x544C3F ,&IsoTileType::Safeguard);
DEFINE_POINTER_CALL(0x544C97, &IsoTileType::Safeguard);
DEFINE_POINTER_CALL(0x544CC9, &IsoTileType::Safeguard);
DEFINE_POINTER_CALL(0x546FCC, &IsoTileType::Safeguard);
DEFINE_POINTER_CALL(0x549AF7, &IsoTileType::Safeguard);
DEFINE_POINTER_CALL(0x549E67, &IsoTileType::Safeguard);