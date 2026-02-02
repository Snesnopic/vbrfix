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

#ifndef XINGFRAME_HPP
#define XINGFRAME_HPP

#include "Mp3Frame.hpp"
#include <memory>
#include <cstdint>
#include <vector>
#include <list>

class FixerSettings;

class XingFrame : public Mp3Frame
{
	using Mp3ObjectList = std::list<const Mp3Object*>;

public:
	explicit XingFrame(const Mp3Header & header);

	void Setup(const Mp3ObjectList &finalObjectList, const XingFrame *pOriginalFrame, const FixerSettings &rFixerSettings, const
			   FileBuffer &mp3FileBuffer);
	bool isOriginalCorrect(const XingFrame* originalFrame);

	// Updated signature
	void write(const FileBuffer & originalFile, std::ostream & rOutStream) const override;

	[[nodiscard]] Mp3ObjectType GetObjectType() const override {return Mp3ObjectType(Mp3ObjectType::XING_FRAME);}

	static XingFrame * Check(CheckParameters & rParams);

	[[nodiscard]] bool HasLameInfo() const override;

	[[nodiscard]] uint32_t GetQuality() const {return m_VbrScale;}
	void SetQuality(int quality);

	void SetLameData(const std::vector<unsigned char> &lameData);
	[[nodiscard]] const std::vector<unsigned char>& GetLameData() const;

	static bool IsCrcUpdateSupported(const Mp3Header & header);

protected:
	enum {NO_QUALITY = 0};
	XingFrame(unsigned long iOldFilePos, const Mp3Header & header);

private:
	using _ul32 = uint32_t;
	std::vector<unsigned char> m_Toc;
	_ul32 m_Flags;
	_ul32 m_FrameCount;
	_ul32 m_StreamSize;
	_ul32 m_VbrScale;

	std::vector<unsigned char> m_LameData;

	bool m_bReCalculateLameCrc;
	bool m_bReCalculateLameMusicCrc;
	int m_musicCRC;
};

#endif