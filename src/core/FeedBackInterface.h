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

#ifndef FEEDBACKINTERFACE_H
#define FEEDBACKINTERFACE_H

#include "Mp3FileObject.h"
#include <string>
#include <utility>

namespace Log
{
	enum Importance{LOG_DETAIL, LOG_INFO, LOG_WARNING, LOG_ERROR};
	class LogItem
	{
		public:
			[[nodiscard]] std::string GetText() const {return m_Text;}
			[[nodiscard]] Importance GetImportance() const {return m_Importance;}
			LogItem() {}
			LogItem(const Importance importance, std::string sText)
				: m_Text(std::move(sText)) , m_Importance(importance){}
		private:
			std::string m_Text;
			Importance m_Importance;
	};
}

class FeedBackInterface
{
	public:
		FeedBackInterface() = default;

		virtual ~FeedBackInterface() = default;

		virtual void update() = 0;

		virtual void addLogMessage(Log::LogItem log) = 0;
		void addLogMessage(const Log::Importance i, const std::string& text)
		{
			addLogMessage(Log::LogItem(i, text));
		}

		[[nodiscard]] virtual bool HasUserCancelled() const = 0;
};

#endif
