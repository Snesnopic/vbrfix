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

#ifndef FIXERSETTINGS_HPP
#define FIXERSETTINGS_HPP

#include <string>
#include <vector>
#include <set>
#include "Mp3FileObject.hpp"
#include "ReadSettings.hpp"

// TODO Make it so we have a Setting Class and Settings returns a list of settings
// with descriptions of there ranges so the gui can generate from the options.

class FixerSettings : public ReadSettings
{
	public:
		FixerSettings();
		~FixerSettings() override = default;

		[[nodiscard]] bool RemoveType(Mp3ObjectType eType) const;
		void SetRemoveType(Mp3ObjectType eType, bool bMarkForRemoval);
		[[nodiscard]] const Mp3ObjectType::Set & GetRemovingDataTypes() const;

		[[nodiscard]] bool AlwaysSkip() const;
		void SetAlwaysSkip(bool bAlwaysSkip);

		enum LameOption { LAME_REMOVE, LAME_KEEP, LAME_KEEP_CALC_TAG_CRC, LAME_KEEP_CALC_BOTH_CRCS };

		[[nodiscard]] LameOption LameInfoOption() const {return m_LameInfoOption;}
		void SetLameInfoOption(const LameOption lameInfo) {m_LameInfoOption = lameInfo;}

		[[nodiscard]] bool KeepLameInfo() const {return m_LameInfoOption != LAME_REMOVE;}

		[[nodiscard]] int MinimumPercentUnderStood() const;
		void SetMinimumPercentUnderStood(int iMinPercentUnderStood);

		[[nodiscard]] bool loggingDetail() const {return m_bLogDetail;}
		void setLoggingDetail(bool value) {m_bLogDetail = value;}

		[[nodiscard]] bool recalculateLameTagHeaderCrc() const {return (m_LameInfoOption == LAME_KEEP_CALC_TAG_CRC) || (m_LameInfoOption == LAME_KEEP_CALC_BOTH_CRCS);}
		[[nodiscard]] bool recalculateLameTagHeaderCrcMusic() const {return m_LameInfoOption == LAME_KEEP_CALC_BOTH_CRCS;}

		void Defaults();

		[[nodiscard]] bool skippingNonVbr() const { return m_bSkipNonVbr;}
		void setSkippingNonVbr(const bool bSkip) { m_bSkipNonVbr = bSkip;}
		enum XingFrameCrcOption { CRC_REMOVE, CRC_KEEP_IF_CAN, CRC_KEEP};
		[[nodiscard]] XingFrameCrcOption GetXingFrameCrcOption() const {return m_XingFrameCrcOption;}
		void setXingFrameCrcOption(const XingFrameCrcOption e) {m_XingFrameCrcOption = e;}

		[[nodiscard]] bool skipIfXingTagLooksGood() const { return m_bSkipIfXingTagLooksGood;}
		void setSkipIfXingTagLooksGood(const bool bSkip) { m_bSkipIfXingTagLooksGood = bSkip;}

		[[nodiscard]] bool removeInconsistentFrames() const { return m_bRemoveInconsistentFrames;}
		void setRemoveInconsistentFrames(const bool b){ m_bRemoveInconsistentFrames = b;}
		
	protected:
		Mp3ObjectType::Set m_RemoveTypes;
		bool m_AlwaysSkip;
		int m_MinPercentUnderstood;
		bool m_bLogDetail;
		LameOption m_LameInfoOption;
		bool m_bSkipNonVbr;
		bool m_bSkipIfXingTagLooksGood;
		XingFrameCrcOption m_XingFrameCrcOption;
		bool m_bRemoveInconsistentFrames;
		
};

#endif
