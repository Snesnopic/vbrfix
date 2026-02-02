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

#ifndef CONSOLEFIXER_HPP
#define CONSOLEFIXER_HPP

#include <list>
#include <string>

#include "FeedBackInterface.hpp"
#include "CommandReader.hpp"

class FixerSettings;

class ConsoleFixer : public FeedBackInterface
{
	public:
		typedef std::list< std::string > CommandList;
		explicit ConsoleFixer(const CommandList& args);

		~ConsoleFixer() override;

		bool Run();

		void update() override {}

		void addLogMessage(Log::LogItem sMsg) override;

		[[nodiscard]] bool HasUserCancelled() const override {return false;}
	private:
		const CommandList& m_Args;

		bool GetFixerSettingsFromOptions(FixerSettings & settings, const CommandReader::OptionList& optionList);
};

#endif
