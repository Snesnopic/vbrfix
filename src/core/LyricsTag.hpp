/*//////////////////////////////////////////////////////////////////////////////////
// copyright : (C) 2009  by William Pye
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

#ifndef LYRICSTAG_HPP
#define LYRICSTAG_HPP

#include "Mp3FileObject.hpp"

class Lyrics3Tag : public Mp3Object
{
	public:
		~Lyrics3Tag() override;
		static Lyrics3Tag* Check(const CheckParameters & rParams);

		[[nodiscard]] unsigned long size() const override;
		[[nodiscard]] Mp3ObjectType GetObjectType() const override {return Mp3ObjectType(Mp3ObjectType::LYRICS_TAG);}
	protected:
		Lyrics3Tag(unsigned long oldFilePosition, unsigned long Size);

	private:
		const unsigned long m_Size;
};

#endif
