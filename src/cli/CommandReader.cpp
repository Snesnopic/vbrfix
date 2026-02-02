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

#include "CommandReader.hpp"
#include <cassert>

namespace
{
	bool bFirstCommandIsTheProgramName = true;
}

CommandReader::CommandReader(const ArgList& originalArgs)
{
	ArgList args = originalArgs;
	if(bFirstCommandIsTheProgramName)
	{
		if(args.empty())
		{
			throw ("Unexpected program arguments");
		}
		args.pop_front();
	}
	for(const auto & arg : args)
	{
		ProcessArg( arg );
	}
}


CommandReader::~CommandReader() = default;

void CommandReader::ProcessArg( const std::string & arg )
{
	if(!arg.empty())
	{
		if(arg.substr(0, 2) == "--")
		{
			m_Options.push_back(arg.substr(2));
		}
		else if(arg[0] == '-')
		{
			m_Options.push_back(arg.substr(1));
		}
		else
		{
			m_Parameters.push_back(arg);
		}
	}
	else
	{
		assert(0);
		throw ("Unexpected Error");
	}
}


