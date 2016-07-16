/*	WARNING: COPYRIGHT (C) 2015 XSENS TECHNOLOGIES OR SUBSIDIARIES WORLDWIDE. ALL RIGHTS RESERVED.
	THIS FILE AND THE SOURCE CODE IT CONTAINS (AND/OR THE BINARY CODE FILES FOUND IN THE SAME
	FOLDER THAT CONTAINS THIS FILE) AND ALL RELATED SOFTWARE (COLLECTIVELY, "CODE") ARE SUBJECT
	TO A RESTRICTED LICENSE AGREEMENT ("AGREEMENT") BETWEEN XSENS AS LICENSOR AND THE AUTHORIZED
	LICENSEE UNDER THE AGREEMENT. THE CODE MUST BE USED SOLELY WITH XSENS PRODUCTS INCORPORATED
	INTO LICENSEE PRODUCTS IN ACCORDANCE WITH THE AGREEMENT. ANY USE, MODIFICATION, COPYING OR
	DISTRIBUTION OF THE CODE IS STRICTLY PROHIBITED UNLESS EXPRESSLY AUTHORIZED BY THE AGREEMENT.
	IF YOU ARE NOT AN AUTHORIZED USER OF THE CODE IN ACCORDANCE WITH THE AGREEMENT, YOU MUST STOP
	USING OR VIEWING THE CODE NOW, REMOVE ANY COPIES OF THE CODE FROM YOUR COMPUTER AND NOTIFY
	XSENS IMMEDIATELY BY EMAIL TO INFO@XSENS.COM. ANY COPIES OR DERIVATIVES OF THE CODE (IN WHOLE
	OR IN PART) IN SOURCE CODE FORM THAT ARE PERMITTED BY THE AGREEMENT MUST RETAIN THE ABOVE
	COPYRIGHT NOTICE AND THIS PARAGRAPH IN ITS ENTIRETY, AS REQUIRED BY THE AGREEMENT.
*/

#ifndef XSAWINDASNAPSHOT_H
#define XSAWINDASNAPSHOT_H

#include "xstypesconfig.h"
#include "xsdeviceid.h"

struct XsAwindaSnapshot;

#ifdef __cplusplus
extern "C" {
#else
#define XSAWINDASNAPSHOT_INITIALIZER { XSDEVICEID_INITIALIZER, 0, 0,0,0, 0,0,0, 0,0,0, 0, 0, 0, 0 }
#endif

XSTYPES_DLL_API void XsAwindaSnapshot_construct(struct XsAwindaSnapshot* thisPtr);
XSTYPES_DLL_API void XsAwindaSnapshot_destruct(struct XsAwindaSnapshot* thisPtr);

#ifdef __cplusplus
} // extern "C"
#endif

struct XsAwindaSnapshot
{
	XsDeviceId m_deviceId;	/*!< \brief The ID of the device that created the data */
	uint32_t m_frameNumber;	/*!< \brief The frame number */
	int32_t m_iQ[3];		/*!< \brief The integrated orientation */
	int32_t m_iV[3];		/*!< \brief The integrated velocity */
	int16_t m_mag[3];		/*!< \brief The magnetic field */
	int32_t m_baro;			/*!< \brief The barometric pressure */
	uint16_t m_status;		/*!< \brief The clipping flags of the latest interval  */
	uint8_t m_accClippingCounter;	/*!< \brief The clipping event counter for the Acc */
	uint8_t m_gyrClippingCounter;	/*!< \brief The clipping event counter for the Gyr */

#ifdef __cplusplus
	/*! \brief Returns true if all fields of this and \a other are exactly identical */
	inline bool operator == (const XsAwindaSnapshot& other) const
	{
		if (m_deviceId != other.m_deviceId ||
			m_frameNumber != other.m_frameNumber ||
			m_baro != other.m_baro ||
			m_status != other.m_status ||
			m_accClippingCounter != other.m_accClippingCounter ||
			m_gyrClippingCounter != other.m_gyrClippingCounter)
			return false;

		for (int i = 0; i < 3; ++i)
		{
			if (m_iQ[i] != other.m_iQ[i] ||
				m_iV[i] != other.m_iV[i] ||
				m_mag[i] != other.m_mag[i])
				return false;
		}
		return true;
	}
#endif
};
typedef struct XsAwindaSnapshot XsAwindaSnapshot;

/*! \brief Status flag definitions for XsAwindaSnapshot status field */
enum AwindaSnapshotStatusFlag
{
	ASSF_ClipAccX		= 0x0001,
	ASSF_ClipAccY		= 0x0002,
	ASSF_ClipAccZ		= 0x0004,
	ASSF_ClipGyrX		= 0x0008,
	ASSF_ClipGyrY		= 0x0010,
	ASSF_ClipGyrZ		= 0x0020,
	ASSF_ClipMagX		= 0x0040,
	ASSF_ClipMagY		= 0x0080,
	ASSF_ClipMagZ		= 0x0100,
	ASSF_MagIsNew		= 0x0200,
	ASSF_BaroIsNew		= 0x0400,
	ASSF_RotationMask	= 0x1800
};
typedef enum AwindaSnapshotStatusFlag AwindaSnapshotStatusFlag;

#define ASSF_RotationShift		11

#endif
