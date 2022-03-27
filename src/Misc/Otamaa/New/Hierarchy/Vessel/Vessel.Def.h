#pragma once
#include <Helpers/EnumFlags.h>
#include <GeneralDefinitions.h>
#include <LocomotionClass.h>

/**
 *  CLSID's for all classes.
 */
#define		CLSID_VESSEL			"EBE80B85-EED2-4DEF-92CA-BC0C99AF4A50"
#define		CLSID_VESSEL_TYPE		"EBE80B85-EED2-4DEF-92CA-BC0C99AF4A51"


 /**
  *  The naval vessels are enumerated below.
  */
typedef enum VesselType
{
	VESSEL_NONE = -1,
	VESSEL_FIRST = 0

}VesselType;
DEFINE_ENUMERATION_OPERATORS(VesselType);

typedef enum ExtRTTIFlags
{
	EXT_RTTIF_PAD = AbstractFlags::Foot,

	EXT_RTTIF_VESSEL,

	EXT_RTTIF_COUNT

}ExtRTTIFlags;
DEFINE_ENUMERATION_OPERATORS(ExtRTTIFlags);

/**
 *  This is the extended enumeration for RTTIType.
 */
typedef enum ExtRTTIType
{
	/**
	 *  This offsets the new RTTIType enum so they are correctly numbered.
	 */
	EXT_RTTI_PAD = AbstractType::DiskLaser, // The last RTTIType

	/**
	 *  Add new RTTITypes from here, do not reorder these!
	 */

	 EXT_RTTI_VESSEL,
	 EXT_RTTI_VESSELTYPE,

	 /**
	  *  The new total RTTIType count.
	  */
	  EXT_RTTI_COUNT

} NewRTTIType;
DEFINE_ENUMERATION_OPERATORS(NewRTTIType);

/**
 *  These defines ensure that comparison checks and argument usage pass.
 */
#define RTTI_VESSEL         AbstractType(EXT_RTTI_VESSEL)
#define RTTI_VESSELTYPE     AbstractType(EXT_RTTI_VESSELTYPE)
#define RTTIF_VESSEL		AbstractFlags(EXT_RTTIF_VESSEL)