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

#ifndef MP3FRAME_HPP
#define MP3FRAME_HPP

#include "Mp3FileObject.hpp"
#include <memory>
#include "Mp3Header.hpp"

class Mp3Header;

class Mp3Frame : public Mp3Object
{
	public:
		
		explicit Mp3Frame(const Mp3Header &header);
		~Mp3Frame() override;

		static Mp3Frame* Check(CheckParameters & rParams);

		[[nodiscard]] unsigned long size() const override;

		[[nodiscard]] Mp3ObjectType GetObjectType() const override {return Mp3ObjectType(Mp3ObjectType::FRAME);}

		[[nodiscard]] const Mp3Header& GetMp3Header() const;
		Mp3Header& GetMp3Header();

		[[nodiscard]] virtual bool HasLameInfo() const {return false;} // this could change if we handle the info tag
		
	protected:
		Mp3Frame(unsigned long oldFilePosition, const Mp3Header &header);
		Mp3Header m_Header;

};

#endif
