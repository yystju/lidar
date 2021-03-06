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

#ifndef XSRAWGPSSVINFO_H
#define XSRAWGPSSVINFO_H

#include "pstdint.h"

#ifdef _MSC_VER
#pragma pack(push, 1)
#endif

#ifndef __cplusplus
#define XSSVINFO_INITIALIZER	{ 0, 0, 0, 0, 0, 0, 0, 0 }
#define XSRAWGPSSVINFO_INITIALIZER	{ 0, XSSVINFO_INITIALIZER, XSSVINFO_INITIALIZER, XSSVINFO_INITIALIZER, XSSVINFO_INITIALIZER, XSSVINFO_INITIALIZER, XSSVINFO_INITIALIZER, XSSVINFO_INITIALIZER, XSSVINFO_INITIALIZER, XSSVINFO_INITIALIZER, XSSVINFO_INITIALIZER, XSSVINFO_INITIALIZER, XSSVINFO_INITIALIZER, XSSVINFO_INITIALIZER, XSSVINFO_INITIALIZER, XSSVINFO_INITIALIZER, XSSVINFO_INITIALIZER };
#endif

/*! \brief A container for information of one satellite
	\deprecated
*/
struct XsSvInfo
{
	uint8_t m_chn;		//!< channel number, range 0..NCH-1
	uint8_t m_svid;		//!< Satellite ID
	uint8_t m_flags;	/*!< Bitmask, made up of the following bit values
							0x01 = SV is used for navigation
							0x02 = Differential correction data is available for this SV
							0x04 = Orbit information is available for this SV (Ephemeris or Almanach)
							0x08 = Orbit information is Ephemeris
							0x10 = SV is unhealthy / shall not be used
							0x20 = reserved
							0x40 = reserved
							0x80 = reserved
						*/
	int8_t  m_qi;		/*!< Signal Quality indicator (range 0..7). The following list shows the meaning of the different QI values:
							0: This channel is idle
							1, 2: Channel is searching
							3: Signal detected but unusable
							4: Code Lock on Signal
							5, 6: Code and Carrier locked
							7: Code and Carrier locked, receiving 50bps data
						*/
	uint8_t	m_cno;		//!< Carrier to Noise Ratio (Signal Strength) (dbHz)
	int8_t	m_elev;		//!< Elevation in integer (deg)
	int16_t	m_azim;		//!< Azimuth in integer (deg)
	int32_t m_prres;	//!< Pseudo range residual (cm)
};
typedef struct XsSvInfo XsSvInfo;

/*! \brief A container for NAV-SVINFO data
*/
struct XsRawGpsSvInfo
{
	uint32_t m_itow;		//!< Gps time of week (ms)
	uint8_t  m_nch;			//!< Number of channels range 0..16
	uint8_t  m_res1;		//!< Reserved
	uint16_t m_res2;		//!< Reserved
	XsSvInfo m_svInfos[16];	//!< The information of all satellites, maxmimum 16
};
typedef struct XsRawGpsSvInfo XsRawGpsSvInfo;

#ifdef _MSC_VER
#pragma pack(pop)
#endif

#ifdef __cplusplus
inline bool operator == (XsRawGpsSvInfo const& a, XsRawGpsSvInfo const& b)
{
	if (a.m_nch != b.m_nch)
		return false;

	return memcmp(&a, &b, a.m_nch * sizeof(XsSvInfo) + offsetof(XsRawGpsSvInfo, m_svInfos)) == 0;
}
#endif

#endif // file guard
