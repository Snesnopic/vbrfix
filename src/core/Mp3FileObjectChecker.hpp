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

#ifndef MP3FILEOBJECTCHECKER_HPP
#define MP3FILEOBJECTCHECKER_HPP

class Mp3Object; class FileBuffer; class FeedBackInterface; class CheckParameters;

class Mp3ObjectCheckerInterface
{
	public:
		virtual Mp3Object* Check(CheckParameters &rParams) const = 0;
		virtual ~Mp3ObjectCheckerInterface() = default;
};

template < typename T_Mp3ObjectType>
class Mp3ObjectChecker : public Mp3ObjectCheckerInterface
{
	public:
		Mp3Object* Check(CheckParameters &rParams) const override
		{
			return T_Mp3ObjectType::Check(rParams);
		}
};

#endif
