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

#include "Mp3FileObject.hpp"
#include "FileBuffer.hpp"
#include <algorithm>
#include <functional>
#include <cassert>
#include <ostream>
#include <vector>

void Mp3Object::write(const FileBuffer & originalFile, std::ostream & rOutStream) const
{
	assert(IsFromFile());
	if(IsFromFile())
	{
		const size_t iObjectSize = size();
		const size_t iObjectStartPos = getOldFilePosition();

		std::vector<unsigned char> buffer(iObjectSize);

		originalFile.setPosition(iObjectStartPos);
		originalFile.readIntoBuffer(buffer.data(), iObjectSize);

		rOutStream.write(reinterpret_cast<const char*>(buffer.data()), iObjectSize);
	}
}

Mp3Object::Mp3Object()
	: m_IsFromFile(false)
	, m_OldFilePosition(0)
{
}

Mp3Object::Mp3Object(const unsigned long iFromFilePosition)
	: m_IsFromFile(true)
	, m_OldFilePosition(iFromFilePosition)
{
}

bool Mp3Object::IsFromFile() const
{
	return m_IsFromFile;
}

unsigned long Mp3Object::getOldFilePosition() const
{
	assert(IsFromFile());
	return m_OldFilePosition;
}

unsigned long Mp3Object::getOldEndOfObjectFilePosition() const
{
	return getOldFilePosition() + size();
}

Mp3Object::~Mp3Object() = default;

bool Mp3ObjectType::operator<(const Mp3ObjectType & rOther) const
{
	return m_Type < rOther.m_Type;
}

Mp3ObjectType::Mp3ObjectType(const ObjectId type)
	: m_Type(type)
{
}

bool Mp3ObjectType::IsTypeOfFrame() const
{
	return (m_Type == FRAME || m_Type == XING_FRAME || m_Type == VBRI_FRAME);
}

bool Mp3ObjectType::IsTypeOfTag() const
{
	return (m_Type == ID3V1_TAG || m_Type == ID3V2_TAG || m_Type == APE_TAG || m_Type == LYRICS_TAG);
}

bool Mp3ObjectType::IsUnknown() const
{
	return (m_Type == UNKNOWN_DATA);
}

const Mp3ObjectType::Set & Mp3ObjectType::GetFrameTypes()
{
	static Set types;
	if(types.empty())
	{
		const Set& allTypes = GetTypes();
		std::ranges::copy_if(allTypes,
		                     std::inserter(types, types.begin()),
		                     [](const Mp3ObjectType& obj){ return obj.IsTypeOfFrame(); });
	}
	return types;
}

const Mp3ObjectType::Set & Mp3ObjectType::GetTagTypes()
{
	static Set types;
	if(types.empty())
	{
		const Set& allTypes = GetTypes();
		std::ranges::copy_if(allTypes,
		                     std::inserter(types, types.begin()),
		                     [](const Mp3ObjectType& obj){ return obj.IsTypeOfTag(); });
	}
	return types;
}

const Mp3ObjectType::Set & Mp3ObjectType::GetTypes()
{
	static Set types;
	if(types.empty())
	{
		types.insert(FRAME);
		types.insert(ID3V1_TAG);
		types.insert(ID3V2_TAG);
		types.insert(UNKNOWN_DATA);
		types.insert(VBRI_FRAME);
		types.insert(XING_FRAME);
		types.insert(LYRICS_TAG);
		types.insert(APE_TAG);
	}
	return types;
}
