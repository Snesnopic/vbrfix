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

#include "FileBuffer.hpp"
#include "EndianHelper.hpp"
#include <cassert>
#include <utility>
#include <stdexcept>

FileBuffer::FileBuffer(std::unique_ptr<IDataSource> source)
	: m_source(std::move(source))
{
	if(!m_source) {
		throw std::runtime_error("FileBuffer initialized with null source");
	}
	if(!m_source->good()) {
		throw std::runtime_error("DataSource is invalid at initialization");
	}
}

FileBuffer::~FileBuffer() = default;

unsigned char FileBuffer::operator [ ]( off_type i ) const
{
	const size_t iCurrentBufferSize = m_InternalBuffer.size();

	if(i >= iCurrentBufferSize)
	{
		const size_t size = i - iCurrentBufferSize + 1;

		std::vector<unsigned char> tempBuffer(size);

		if (!m_source->read(tempBuffer.data(), size)) {
			throw std::runtime_error("File Read error or Unexpected EOF");
		}

		m_InternalBuffer.insert(m_InternalBuffer.end(), tempBuffer.begin(), tempBuffer.end());
	}
	return m_InternalBuffer[i];
}

bool FileBuffer::proceed(const off_type i ) const {
	const size_t iCurrentBufferSize = m_InternalBuffer.size();

	if(i < iCurrentBufferSize)
	{
		m_InternalBuffer.erase(m_InternalBuffer.begin(), m_InternalBuffer.begin() + i);
	}
	else
	{
		m_InternalBuffer.clear();

		const size_t skipAmount = i - iCurrentBufferSize;
		size_t currentPos = m_source->position();

		if (!m_source->seek(currentPos + skipAmount)) {
			return false;
		}
	}
	return true;
}


FileBuffer::pos_type FileBuffer::position( ) const
{
	return (m_source->position() - m_InternalBuffer.size());
}

bool FileBuffer::isDataLeft( ) const
{
	if (!m_InternalBuffer.empty()) return true;
	return (m_source->position() < m_source->length());
}

bool FileBuffer::setPosition(const pos_type &iPos ) const {
	if (!m_source->seek(iPos)) return false;
	m_InternalBuffer.clear();
	return true;
}

bool FileBuffer::readIntoBuffer( unsigned char * pBuffer, const off_type iSize ) const {
	assert(pBuffer);
	return m_source->read(pBuffer, iSize);
}

void FileBuffer::reset( ) const {
	m_source->seek(0);
	m_InternalBuffer.clear();
}

bool FileBuffer::CanRead(const off_type iCount ) const
{
	return (m_source->length() >= (position() + iCount));
}

bool FileBuffer::DoesSay(const std::string_view& sText, const off_type iStartingfromByte) const
{
	assert(!sText.empty());
	if(!CanRead(iStartingfromByte + sText.size()))
		return false;
	for(size_t i = 0; i < sText.size(); ++i)
	{
		if(sText[i] != (*this)[i + iStartingfromByte]) return false;
	}
	return true;
}

void FileBuffer::getData(std::vector<unsigned char>& dest, const off_type iStartingfromByte) const
{
	for(size_t i = 0; i < dest.size(); ++i)
	{
		dest[i] = (*this)[i + iStartingfromByte];
	}
}

unsigned long FileBuffer::GetFromBigEndianToNative(const off_type iStartingfromByte ) const
{
	unsigned char buffer[4];
	for(size_t i = 0; i < sizeof(buffer); ++i)
	{
		buffer[i] = (*this)[iStartingfromByte + i];
	}
	return EndianHelper::ConvertToNativeFromBigEndian(buffer);
}

unsigned long FileBuffer::GetFromLitleEndianToNative(const off_type iStartingfromByte ) const
{
	unsigned char buffer[4];
	for(size_t i = 0; i < sizeof(buffer); ++i)
	{
		buffer[i] = (*this)[iStartingfromByte + i];
	}
	return EndianHelper::ConvertToNativeFromLittleEndian(buffer);
}