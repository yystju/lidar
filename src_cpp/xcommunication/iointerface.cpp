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

#include <xsens/xsthread.h>
#include "iointerface.h"
#include <errno.h>
#ifndef _WIN32
#	include <unistd.h>		// close
#	include <sys/ioctl.h>	// ioctl
#	include <fcntl.h>		// open, O_RDWR
#	include <string.h>		// strcpy
#	include <sys/param.h>
#	include <stdarg.h>
#else
#	include <winbase.h>
#   include <io.h>
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
#	define _CRT_SECURE_NO_DEPRECATE
#	ifdef _WIN32
#		pragma warning(disable:4996)
#	endif
#endif

/*! \class IoInterface
	\brief An abstract IO interface
	\details An %IoInterface provides a generic interface for dealing with an interface device such
	as a file, a com port or a USB port.
	\note The class is not thread safe.
*/

// IOInterface functions
/*! \copydoc SerialInterface::open(const XsPortInfo&, uint32_t, uint32_t, PortOptions) */
XsResultValue IoInterface::open ( const XsPortInfo&, uint32_t, uint32_t, PortOptions)
{
	return XRV_INVALIDOPERATION;
}
/*! \copydoc SerialInterface::setTimeout(uint32_t) */
XsResultValue IoInterface::setTimeout (uint32_t ms)
{
	(void) ms;
	return XRV_INVALIDOPERATION;
}
/*! \copydoc SerialInterface::waitForData(XsSize, XsByteArray&) */
XsResultValue IoInterface::waitForData (XsSize maxLength, XsByteArray& data)
{
	(void) maxLength;
	(void) data;
	return XRV_INVALIDOPERATION;
}
/*! \copydoc SerialInterface::cancelIo(void) const */
void IoInterface::cancelIo(void) const
{

}

// IOInterfaceFile functions
/*! \copydoc IoInterfaceFile::appendData() */
XsResultValue IoInterface::appendData(const XsByteArray& bdata)
{
	(void) bdata;
	return XRV_INVALIDOPERATION;
}
/*! \copydoc IoInterfaceFile::closeAndDelete() */
XsResultValue IoInterface::closeAndDelete(void)
{
	return XRV_INVALIDOPERATION;
}
/*! \copydoc IoInterfaceFile::create(const XsString&) */
XsResultValue IoInterface::create(const XsString& filename)
{
	(void) filename;
	return XRV_INVALIDOPERATION;
}
/*! \copydoc IoInterfaceFile::deleteData(XsFilePos, XsSize) */
XsResultValue IoInterface::deleteData(XsFilePos start, XsSize length)
{
	(void) start;
	(void) length;
	return XRV_INVALIDOPERATION;
}
/*! \copydoc IoInterfaceFile::find(const XsByteArray&, XsFilePos&) */
XsResultValue IoInterface::find(const XsByteArray& needleV, XsFilePos& pos)
{
	(void) needleV;
	(void) pos;
	return XRV_INVALIDOPERATION;
}
/*! \copydoc IoInterfaceFile::getFileSize() const */
XsFilePos IoInterface::getFileSize(void) const
{
	return 0;
}
/*! \copydoc IoInterfaceFile::getName(XsString&) const */
XsResultValue IoInterface::getName(XsString& filename) const
{
	(void) filename;
	return XRV_INVALIDOPERATION;
}
/*! \copydoc IoInterfaceFile::getReadPosition() const */
XsFilePos IoInterface::getReadPosition(void) const
{
	return 0;
}
/*! \copydoc IoInterfaceFile::getWritePosition() const */
XsFilePos IoInterface::getWritePosition(void) const
{
	return 0;
}
/*! \copydoc IoInterfaceFile::insertData(XsFilePos, const XsByteArray&) */
XsResultValue IoInterface::insertData(XsFilePos start, const XsByteArray& data)
{
	(void) start;
	(void) data;
	return XRV_INVALIDOPERATION;
}
/*! \copydoc IoInterfaceFile::isReadOnly() const */
bool IoInterface::isReadOnly(void) const
{
	return true;
}
/*! \copydoc IoInterfaceFile::open(const XsString&, bool, bool) */
XsResultValue IoInterface::open(const XsString& filename, bool createNew, bool readOnly)
{
	(void) filename;
	(void) createNew;
	(void) readOnly;
	return XRV_INVALIDOPERATION;
}
/*! \copydoc IoInterfaceFile::setReadPosition(XsFilePos) */
XsResultValue IoInterface::setReadPosition(XsFilePos pos)
{
	(void) pos;
	return XRV_INVALIDOPERATION;
}
/*! \copydoc IoInterfaceFile::setWritePosition(XsFilePos) */
XsResultValue IoInterface::setWritePosition(XsFilePos pos)
{
	(void) pos;
	return XRV_INVALIDOPERATION;
}
