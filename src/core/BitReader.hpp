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

#ifndef BITREADER_HPP
#define BITREADER_HPP

#include <cstdint>

class IndexMask
{
	using _ul32 = uint32_t;
	static _ul32 MakeMask(const _ul32 iIndex, const _ul32 iBits = 1)
	{
		_ul32 mask = 0;
		for(unsigned int i = 0; i < iBits; ++i)
		{
			if(i > 0) mask = (mask << 1);
			mask |= 1;
		}
		mask = (mask << iIndex);
		return mask;
	}

	public:
		explicit IndexMask(const _ul32 iIndex, const _ul32 iBits = 1)
			: m_mask(MakeMask(iIndex, iBits))
			, m_maskShift(iIndex)
		{
		}
		const _ul32 m_mask;
		const _ul32 m_maskShift;
		[[nodiscard]] _ul32 GetIndex(const _ul32 bits) const
		{
			return ((bits & m_mask) >> m_maskShift);
		}
		[[nodiscard]] bool IsOn(const _ul32 bits) const
		{
			return ((bits & m_mask) != 0);
		}
		[[nodiscard]] bool areAllOn(const _ul32 bits) const
		{
			return ((bits & m_mask) == m_mask);
		}
		void SetIndex(_ul32 &bits, const int iIndex) const
		{
			bits &= (~m_mask); // remove current value
			bits |= (m_mask & (iIndex << m_maskShift));
		}
		void SetOn(_ul32 &bits, const bool bValue) const
		{
			SetIndex(bits, bValue ? 1 : 0);
		}
};
#endif
