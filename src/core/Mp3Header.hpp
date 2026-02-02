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

#ifndef MP3HEADER_HPP
#define MP3HEADER_HPP
#include <cstdint>

class Mp3Header
{
	using _ul32 = uint32_t;
	
	public:
		enum MpegVersion { MPEG_VERSION_RESERVED, MPEG_VERSION_1, MPEG_VERSION_2, MPEG_VERSION_2_5};
		enum MpegLayerVersion { LAYER_VERSION_RESERVED, LAYER_VERSION_1, LAYER_VERSION_2, LAYER_VERSION_3};
		enum Emphasis {EMPHASIS_NONE, EMPHASIS_50_15_MS, EMPHASIS_RESERVED, EMPHASIS_CCIT_J_17};
		enum ChannelMode {CHAN_STEREO, CHAN_JOINT_STEREO, CHAN_DUAL_CHANNEL, CHAN_MONO};
		enum ModeExtension { BANDS4to31, BANDS8to31, BANDS12to31, BANDS16to3 };
		
		enum {
			BITRATE_FREE = 0,
			BITRATE_ERRROR = -1,
			SAMPLE_FREQ_ERROR = -2,
			FRAME_SIZE_ERROR = -3,
			HEADER_SIZE_IN_BYTES = 4
		};
		
		explicit Mp3Header(_ul32 header);
		Mp3Header(const Mp3Header& rOther);
		[[nodiscard]] bool IsValid() const;
		[[nodiscard]] MpegVersion GetMpegVersion() const;
		[[nodiscard]] MpegLayerVersion GetLayerVersion() const;
		[[nodiscard]] int GetBitRate() const;
		[[nodiscard]] int GetKBitRate() const;
		[[nodiscard]] int GetSampleFrequency() const;
		[[nodiscard]] int GetFrameSize() const;

		[[nodiscard]] bool IsProtectedByCrc() const;
		[[nodiscard]] bool IsPadded() const;
		[[nodiscard]] bool IsPrivate() const;
		[[nodiscard]] bool IsCopyRighted() const;
		[[nodiscard]] bool IsOriginal() const;
		[[nodiscard]] bool IsFreeBitrate() const;

		[[nodiscard]] ChannelMode GetChannelMode() const;
		[[nodiscard]] ModeExtension GetModeExtension() const; // layer I & II only
		[[nodiscard]] bool IsModeExtIntensityStereo() const; // layer III only
		[[nodiscard]] bool IsModeExtMsStereo() const; // layer III only

		[[nodiscard]] Emphasis GetEmphasis() const;

		[[nodiscard]] _ul32 GetHeader() const {return m_Header;}

		bool IncreaseBitrate();
		
		void RemoveCrcProtection();

		bool operator < (const Mp3Header& rOther) const
		{
			return m_Header < rOther.m_Header;
		}

	private:
		enum {HEADER_BITS = 32};
		_ul32 m_Header;

		[[nodiscard]] int GetSizeCoef2() const;
		[[nodiscard]] int GetSizeCoef1() const;
};

#endif
