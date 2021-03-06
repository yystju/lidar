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

#ifndef XSUSHORTVECTOR_H
#define XSUSHORTVECTOR_H

#ifndef __cplusplus
#define XSUSHORTVECTOR_INITIALIZER {{0,0,0}}
#else
#include "xstypedefs.h"
#endif

/*! \brief A vector containing 3 short values. */
struct XsUShortVector {
#ifdef __cplusplus
	//! \brief Constructor that creates the vector with all components set to 0
	inline XsUShortVector()
	{
		m_data[0] = 0;
		m_data[1] = 0;
		m_data[2] = 0;
	}

	//! \brief Constructor that creates the vector with all components set to given values \a v1 \a v2 and \a v3
	inline XsUShortVector(unsigned short v1, unsigned short v2, unsigned short v3)
	{
		m_data[0] = v1;
		m_data[1] = v2;
		m_data[2] = v3;
	}

	//! \brief Constructor that creates the vector with all components set to values in array \a a
	inline explicit XsUShortVector(const unsigned short* a)
	{
		m_data[0] = a[0];
		m_data[1] = a[1];
		m_data[2] = a[2];
	}

	//! \brief Constructor that creates the vector and initializes it with data from the \a other vector
	inline XsUShortVector(const XsUShortVector& other)
	{
		m_data[0] = other.m_data[0];
		m_data[1] = other.m_data[1];
		m_data[2] = other.m_data[2];
	}

	//! \brief Assignment operator copies the data from the \a other vector to this vector
	inline const XsUShortVector& operator = (const XsUShortVector& other)
	{
		if (this != &other)
		{
			m_data[0] = other.m_data[0];
			m_data[1] = other.m_data[1];
			m_data[2] = other.m_data[2];
		}
		return *this;
	}

	//! \brief Comparison operator, returns true if the contents of the \a other vector match those of this vector
	inline bool operator == (const XsUShortVector& other) const
	{
		return	m_data[0] == other.m_data[0] &&
				m_data[1] == other.m_data[1] &&
				m_data[2] == other.m_data[2];
	}

	//! \brief Return the size of the vector (always 3)
	inline XsSize size() const
	{
		return 3;
	}

	//! \brief Return a value from the vector (needed to allow generated C# access to these elements)
	inline unsigned short at(int index)
	{
		return m_data[index];
	}

	//! \brief Returns the \a index'th item in the vector
	inline unsigned short operator[](XsSize index) const
	{
		assert(index < 3);
		return m_data[index];
	}

	//! \brief Returns a reference the \a index'th item in the vector
	inline unsigned short& operator[](XsSize index)
	{
		assert(index < 3);
		return m_data[index];	//lint !e1536
	}

	//! \brief Returns the start of the internal data buffer
	inline unsigned short const* data() const
	{
		return m_data;
	}
private:
#endif

	unsigned short m_data[3];	//!< vector component storage
};

typedef struct XsUShortVector XsUShortVector;

#endif	// XSUSHORTVECTOR_H
