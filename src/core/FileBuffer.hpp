/*//////////////////////////////////////////////////////////////////////////////////
// copyright : (C) 2006  by William Pye
// contact   : www.willwap.co.uk
///////////////////////////////////////////////////////////////////////////////////
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
///////////////////////////////////////////////////////////////////////////////////*/

#ifndef FILEBUFFER_HPP
#define FILEBUFFER_HPP

#include "IDataSource.hpp"
#include <memory>
#include <deque>
#include <string>
#include <vector>

class FileBuffer
{
public:
	using pos_type = size_t;
	using off_type = size_t;

	explicit FileBuffer(std::unique_ptr<IDataSource> source);

	virtual ~FileBuffer();

	unsigned char operator [] (off_type i) const;

	unsigned long GetFromBigEndianToNative(off_type iStartingfromByte = 0) const;
	unsigned long GetFromLitleEndianToNative( off_type iStartingfromByte = 0) const;

	void getData(std::vector<unsigned char>& dest, off_type iStartingfromByte = 0) const;

	bool CanRead(off_type iCount) const;

	bool proceed(off_type i) const;

	bool setPosition(const pos_type &iPos) const;
	bool readIntoBuffer(unsigned char * pBuffer, off_type iSize) const;

	bool DoesSay(const std::string& sText, off_type iStartingfromByte = 0) const;

	bool isDataLeft() const;

	void reset() const;

	pos_type position() const;

	pos_type GetLength() const { return m_source->length(); }

private:
	std::unique_ptr<IDataSource> m_source;
	mutable std::deque<unsigned char> m_InternalBuffer;
};

#endif