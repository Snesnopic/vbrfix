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

#include "VbrFixer.hpp"
#include "Mp3Reader.hpp"
#include "Mp3Header.hpp"
#include "FileBuffer.hpp"
#include "Mp3FileObject.hpp"
#include "FeedBackInterface.hpp"
#include "FixerSettings.hpp"
#include "XingFrame.hpp"
#include <memory>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <map>

namespace
{
	class IsOfMp3ObjectType
	{
		const Mp3ObjectType::Set& m_Types;
			Mp3ObjectType::Set m_IntTypes;
		public:
			bool operator () (const Mp3Object *pObject) const {
				return (m_Types.contains(pObject->GetObjectType()));
			}
			explicit IsOfMp3ObjectType(const Mp3ObjectType::Set& types)
				: m_Types(types) {}

			explicit IsOfMp3ObjectType(const Mp3ObjectType::ObjectId type)
				: m_Types(m_IntTypes)
			{
				m_IntTypes.insert(type);
			}
	};

	class FindLameInfoFrame
	{
		public:
			bool operator () (const Mp3Object *pObject) const {
				if(pObject->GetObjectType().IsTypeOfFrame())
				{
					assert(dynamic_cast<const Mp3Frame*>(pObject));
					if(dynamic_cast<const Mp3Frame*>(pObject)->HasLameInfo())
					{
						return true;
					}
				}
				return false;
			}
	};
}

VbrFixer::VbrFixer( FeedBackInterface & rFeedBackInterface, const FixerSettings & fixerSettings )
	: m_rFeedBackInterface(rFeedBackInterface)
	, m_rFixerSettings(fixerSettings)
{
}

VbrFixer::~VbrFixer() = default;

struct ConsistencyChecker
{
	ConsistencyChecker() : mostPopularFrameHeader(0){}
	Mp3Header mostPopularFrameHeader;

	void workOn(Mp3Reader::ConstMp3ObjectList& objects, bool remove, FeedBackInterface & feedBack)
	{
		threshold = objects.size() / 100;
		readMode = true;
		for(const auto & object : objects)
			(*this)(object);

		std::stringstream headersText; headersText << "Found MP3 headers: ";
		int maxCount = 0;
		for(auto & header : headers)
		{
			int count = header.second;
			headersText << std::dec <<"[x" << count << "]0x" << std::hex << std::uppercase << header.first.GetHeader() << " ";
			maxCount = std::max(count, maxCount);
			if(maxCount == count) mostPopularFrameHeader = header.first;
		}
		feedBack.addLogMessage(Log::LOG_DETAIL, headersText.str());
		if(layers.size() > 1)
			feedBack.addLogMessage(Log::LOG_WARNING, "Found inconsistent frame layer versions");
		if(versions.size() > 1)
			feedBack.addLogMessage(Log::LOG_WARNING, "Found inconsistent frame MPEG versions");
		if(frequencies.size() > 1)
			feedBack.addLogMessage(Log::LOG_WARNING, "Found inconsistent frame Sample Frequencies");

		if(remove)
		{
			readMode = false;
			objects.erase(std::remove_if(objects.begin(), objects.end(), *this), objects.end());
		}
	}

	std::map<Mp3Header, int> headers;
	std::map<int, int> frequencies;
	std::map<Mp3Header::MpegLayerVersion, int> layers;
	std::map<Mp3Header::MpegVersion, int> versions;
	bool readMode;
	int threshold;

	bool operator()(const Mp3Object* obj)
	{
		if(obj->GetObjectType().IsTypeOfFrame())
		{
			const Mp3Header header = dynamic_cast<const Mp3Frame&>(*obj).GetMp3Header();
			if(readMode)
			{
				headers[header]++;
				frequencies[header.GetSampleFrequency()]++;
				layers[header.GetLayerVersion()]++;
				versions[header.GetMpegVersion()]++;
			}
			else
			{
				if(frequencies[header.GetSampleFrequency()] < threshold) return true;
				if(layers[header.GetLayerVersion()] < threshold) return true;
				if(versions[header.GetMpegVersion()] < threshold) return true;
			}
		}
		return false;
	}
};

void VbrFixer::Fix(std::unique_ptr<IDataSource> pSource, std::ostream & output)
{
	try
	{
		m_rFeedBackInterface.addLogMessage(Log::LOG_INFO, "Starting Fix");

		FileBuffer inFile(std::move(pSource));

		m_ProgressDetails.SetState(FixState::READING);
		Mp3Reader mp3Reader(inFile, m_rFeedBackInterface, m_ProgressDetails, m_rFixerSettings);
		mp3Reader.ReadMp3();

		m_rFeedBackInterface.addLogMessage(Log::LOG_INFO, "Finished Reading Mp3 Structure");
		m_ProgressDetails.SetState(FixState::PROCESSING);

		const Mp3Reader::ConstMp3ObjectList& OriginalMp3Objects = mp3Reader.GetMp3Objects();

		std::vector<const Mp3Object*> Mp3Objects(OriginalMp3Objects.begin(), OriginalMp3Objects.end());

		ConsistencyChecker consistencyChecker;

		Mp3Reader::ConstMp3ObjectList Mp3ObjectsList = OriginalMp3Objects;

		consistencyChecker.workOn(Mp3ObjectsList, m_rFixerSettings.removeInconsistentFrames(), m_rFeedBackInterface);

		m_ProgressDetails.setPercentOfProcessing(20);
		m_rFeedBackInterface.update();

		// Check if we should Skip this Mp3
		if(ShouldSkipMp3(Mp3ObjectsList))
		{
			m_ProgressDetails.SetState(FixState::SKIPPED);
			return;
		}

		if(m_rFeedBackInterface.HasUserCancelled())
		{
			m_ProgressDetails.SetState(FixState::CANCELLED);
			return;
		}

		m_ProgressDetails.setPercentOfProcessing(40);
		m_rFeedBackInterface.update();

		// remove unwanted objects
		if(!m_rFixerSettings.GetRemovingDataTypes().empty())
		{
			Mp3ObjectsList.erase(std::remove_if(
				Mp3ObjectsList.begin(),
				Mp3ObjectsList.end(),
				IsOfMp3ObjectType(m_rFixerSettings.GetRemovingDataTypes())
			), Mp3ObjectsList.end());
		}

		// check lyrics tag
		{
			Mp3ObjectType::Set objTypes;
			std::ranges::transform(Mp3ObjectsList,
			                       std::inserter(objTypes, objTypes.end()), [](const Mp3Object* obj){ return obj->GetObjectType(); });

			if(objTypes.contains(Mp3ObjectType::LYRICS_TAG)
				&& !objTypes.contains(Mp3ObjectType::ID3V1_TAG))
			{
				throw "Resultant mp3 has a Lyrics3 tag but no Id3v1 tag. Please add an Id3v1 tag or choose to remove the Lyrics tag";
			}
		}

		m_ProgressDetails.setPercentOfProcessing(60);
		m_rFeedBackInterface.update();

		if(m_rFeedBackInterface.HasUserCancelled())
		{
			m_ProgressDetails.SetState(FixState::CANCELLED);
			return;
		}

		// insert a vbr header object if needed before the 1st frame
		std::unique_ptr<XingFrame> xingFrame;
		if(m_ProgressDetails.IsVbr())
		{
			auto firstFrame = std::find_if(Mp3ObjectsList.begin(), Mp3ObjectsList.end(), IsOfMp3ObjectType(Mp3ObjectType::GetFrameTypes()));
			assert(firstFrame != Mp3ObjectsList.end());
			xingFrame = std::make_unique<XingFrame>(consistencyChecker.mostPopularFrameHeader);
			Mp3ObjectsList.insert(firstFrame, xingFrame.get());
		}

		if(m_rFeedBackInterface.HasUserCancelled())
		{
			m_ProgressDetails.SetState(FixState::CANCELLED);
			return;
		}

		m_ProgressDetails.setPercentOfProcessing(100);
		m_rFeedBackInterface.update();

		inFile.reset();

		if(xingFrame)
		{
			const XingFrame * pOriginalFrame = nullptr;
			if(m_rFixerSettings.KeepLameInfo())
			{
				auto lameFrameIter = std::find_if(OriginalMp3Objects.begin(), OriginalMp3Objects.end(), FindLameInfoFrame());
				if(lameFrameIter != OriginalMp3Objects.end())
				{
					pOriginalFrame = dynamic_cast<const XingFrame* >(*lameFrameIter);
				}
			}
			if(!pOriginalFrame)
			{
				auto xingFrameIter = std::find_if(OriginalMp3Objects.begin(), OriginalMp3Objects.end(), IsOfMp3ObjectType(Mp3ObjectType::XING_FRAME));
				if(xingFrameIter != OriginalMp3Objects.end())
				{
					pOriginalFrame = dynamic_cast<const XingFrame* >(*xingFrameIter);
				}
			}

			xingFrame->Setup(Mp3ObjectsList, pOriginalFrame, m_rFixerSettings, inFile);
			if(pOriginalFrame && m_rFixerSettings.skipIfXingTagLooksGood())
			{
				if(xingFrame->isOriginalCorrect(pOriginalFrame))
				{
					m_ProgressDetails.SetState(FixState::SKIPPED);
					m_rFeedBackInterface.addLogMessage( Log::LOG_INFO, "Original Xing Frame looks OK - skipping");
					return;
				}
			}
		}

		m_ProgressDetails.SetState(FixState::WRITING);
		unsigned long iFileSizeTotal = 0;
		unsigned long iStreamSize = 0;
		int iObjectsWritten = 0;
		const size_t iTotalObjects = Mp3ObjectsList.size();

		for(auto & Mp3Object : Mp3ObjectsList)
		{
			Mp3Object->write(inFile, output);
			iFileSizeTotal += Mp3Object->size();

			if(Mp3Object->GetObjectType().IsTypeOfFrame())
			{
				iStreamSize += Mp3Object->size();
			}

			if(m_rFeedBackInterface.HasUserCancelled())
			{
				m_ProgressDetails.SetState(FixState::CANCELLED);
				return;
			}
			m_ProgressDetails.setPercentOfWriting( ++iObjectsWritten / iTotalObjects );
			m_rFeedBackInterface.update();
		}

		if(m_rFeedBackInterface.HasUserCancelled())
		{
			m_ProgressDetails.SetState(FixState::CANCELLED);
			return;
		}

		std::stringstream ss;
		ss << "New File Size = " << iFileSizeTotal << ", Audio Stream Size = " << iStreamSize;
		m_rFeedBackInterface.addLogMessage(Log::LOG_INFO, ss.str());

		m_rFeedBackInterface.addLogMessage(Log::LOG_INFO, "Finished Fix");
		m_ProgressDetails.SetState(FixState::FIXED);
		m_ProgressDetails.setPercentOfWriting(100);
		m_rFeedBackInterface.update();
	}
	catch(const char* pMsg)
	{
		m_ProgressDetails.SetState(FixState::ERROR);
		m_rFeedBackInterface.addLogMessage( Log::LOG_ERROR, pMsg);
	}
	catch(std::string& s)
	{
		m_ProgressDetails.SetState(FixState::ERROR);
		m_rFeedBackInterface.addLogMessage( Log::LOG_ERROR, s);
	}
	catch(std::exception& e)
	{
		m_ProgressDetails.SetState(FixState::ERROR);
		m_rFeedBackInterface.addLogMessage( Log::LOG_ERROR, std::string("Error: ") + e.what());
	}
	catch(...)
	{
		m_ProgressDetails.SetState(FixState::ERROR);
		m_rFeedBackInterface.addLogMessage( Log::LOG_ERROR, "Unknown Error fixing mp3");
	}
}

VbrFixer::ProgressDetails::ProgressDetails( )
	: m_iPercentOfWrite(0)
	, m_iPercentOfProcessing(0)
	, m_eState(FixState::INITIALISING)
{
}

int VbrFixer::ProgressDetails::GetTotalPercent( ) const
{
	return ((m_iPercentOfRead * 7 + m_iPercentOfProcessing * 1 + m_iPercentOfWrite * 2) / 10);
}

bool VbrFixer::ShouldSkipMp3( const Mp3Reader::ConstMp3ObjectList & /*frames*/ ) const
{
	// TODO ensure that if any LOG_ERROR messages were logged then the mp3 will fail
	
	if(m_rFixerSettings.MinimumPercentUnderStood() > m_ProgressDetails.GetPercentUnderstood())
	{
		std::stringstream ss;
		ss << "Only " << m_ProgressDetails.GetPercentUnderstood() << "% understood, minimum allowed is " << m_rFixerSettings.MinimumPercentUnderStood() << "%; skipping";
		m_rFeedBackInterface.addLogMessage( Log::LOG_ERROR, ss.str());
		return true;
	}
	if(m_rFixerSettings.AlwaysSkip())
	{
		m_rFeedBackInterface.addLogMessage( Log::LOG_INFO, "Always Skip is on; skipping file");
		return true;
	}
	if(m_rFixerSettings.skippingNonVbr() && !m_ProgressDetails.IsVbr())
	{
		m_rFeedBackInterface.addLogMessage( Log::LOG_INFO, "Skipping as the file is not vbr");
		return true;
	}
	if(m_ProgressDetails.GetFrameCount() == 0)
	{
		m_rFeedBackInterface.addLogMessage( Log::LOG_ERROR, "The mp3 has no identified frames ; skipping");
		return true;
	}
	return false;
}

void VbrFixer::ProgressDetails::SetState( FixState::State eState )
{
	m_eState = eState;
}

FixState::State VbrFixer::ProgressDetails::GetState( ) const
{
	return m_eState;
}





