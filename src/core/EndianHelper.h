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

#ifndef ENDIANHELPER_H
#define ENDIANHELPER_H

#include <cstdint>
#include <vector>
#include <ostream>

namespace EndianHelper
{
	inline uint32_t ConvertToNativeFromBigEndian(const uint8_t* pBuffer)
	{
		return (static_cast<uint32_t>(pBuffer[0]) << 24) |
			   (static_cast<uint32_t>(pBuffer[1]) << 16) |
			   (static_cast<uint32_t>(pBuffer[2]) << 8)  |
			   (static_cast<uint32_t>(pBuffer[3]));
	}

	inline uint32_t ConvertToNativeFromLittleEndian(const uint8_t* pBuffer)
	{
		return (static_cast<uint32_t>(pBuffer[3]) << 24) |
			   (static_cast<uint32_t>(pBuffer[2]) << 16) |
			   (static_cast<uint32_t>(pBuffer[1]) << 8)  |
			   (static_cast<uint32_t>(pBuffer[0]));
	}

	inline void WriteToFileAsBigEndian(std::ostream& file, const uint32_t uData)
	{
		const uint8_t bytes[4] = {
			static_cast<uint8_t>((uData >> 24) & 0xFF),
			static_cast<uint8_t>((uData >> 16) & 0xFF),
			static_cast<uint8_t>((uData >> 8)  & 0xFF),
			static_cast<uint8_t>(uData & 0xFF)
		};
		file.write(reinterpret_cast<const char*>(bytes), 4);
	}

	inline std::vector<uint8_t> ConvertToBigEndianBytes(const uint32_t uData)
	{
		return {
			static_cast<uint8_t>((uData >> 24) & 0xFF),
			static_cast<uint8_t>((uData >> 16) & 0xFF),
			static_cast<uint8_t>((uData >> 8)  & 0xFF),
			static_cast<uint8_t>(uData & 0xFF)
		};
	}
}

#endif // ENDIANHELPER_H
