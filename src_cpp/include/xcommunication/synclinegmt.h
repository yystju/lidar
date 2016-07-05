/*	Copyright (c) 2003-2015 Xsens Technologies B.V. or subsidiaries worldwide.
	All rights reserved.

	Redistribution and use in source and binary forms, with or without modification,
	are permitted provided that the following conditions are met:

	1.	Redistributions of source code must retain the above copyright notice,
		this list of conditions and the following disclaimer.

	2.	Redistributions in binary form must reproduce the above copyright notice,
		this list of conditions and the following disclaimer in the documentation
		and/or other materials provided with the distribution.

	3.	Neither the names of the copyright holders nor the names of their contributors
		may be used to endorse or promote products derived from this software without
		specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
	MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
	THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
	OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
	HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY OR
	TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef XSSYNCLINEGMT_H
#define XSSYNCLINEGMT_H

#include <xsens/xssyncline.h>

/*!	\addtogroup enums Global enumerations
	@{
*/
/*! \brief Synchronization line identifiers for the generic motion tracker (GMT) devices, only to be used directly in Xbus messages
	Applies to these devices:
		* MTx2
		* MTw2
		* MTi-1 series
		* FMT1000 series
*/
enum SyncLineGmt
{
	XSLGMT_In			= 0,	//!< Sync in 1 \ref XSL_In1
	XSLGMT_Out			= 1,	//!< Sync out 1 \ref XSL_Out1
	XSLGMT_GpsClockIn	= 2,	//!< GPS clock sync \ref XSL_GpsClockIn
	XSLGMT_ClockIn		= 3,	//!< External clock sync \ref XSL_ClockIn
	XSLGMT_BiIn			= 4,	//!< Bidirectional sync line, configured as input \ref XSL_Bi1In
	XSLGMT_BiOut		= 5,	//!< Bidirectional sync line, configured as output \ref XSL_Bi1Out
	XSLGMT_ReqData		= 6,	//!< Serial data sync option, use XMID_ReqData message id for this \ref XSL_ReqData

	XSLGMT_Invalid
};
/*! @} */
typedef enum SyncLineGmt SyncLineGmt;

#ifdef __cplusplus
extern "C" {
#endif

XsSyncLine xslgmtToXsl(SyncLineGmt mk4Line);
SyncLineGmt xslToXslgmt(XsSyncLine line);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // file guard
