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

#include "ConsoleFixer.hpp"
#include "VbrFixer.hpp"
#include "FixerSettings.hpp"
#include "CommandReader.hpp"
#include "FileDataSource.hpp" // Necessario per creare la sorgente dati

#include <iostream>
#include <fstream>          // Necessario per l'output stream
#include <memory>
#include <stdexcept>

namespace
{
	std::string gConsoleVersionString = "-0";
}

ConsoleFixer::~ConsoleFixer() = default;

ConsoleFixer::ConsoleFixer(const CommandList& args)
	: m_Args(args)
{
}

void ConsoleFixer::addLogMessage(const Log::LogItem sMsg)
{
	std::cout << sMsg.GetText() << std::endl;
}

bool ConsoleFixer::Run( )
{
	const std::string ConsoleVersion = VbrFixer::GetFixerVersion() + gConsoleVersionString;
	std::cout << "Vbrfix Console version " << ConsoleVersion << std::endl;
	FixerSettings settings;
	VbrFixer fixer(*this, settings);
	CommandReader cmdReader(m_Args);
	if ((cmdReader.GetParameterList().size() == 2) &&
	    GetFixerSettingsFromOptions(settings, cmdReader.GetOptionList()))
	{
		const std::string& inFile = cmdReader.GetParameterList().front();
		const std::string& outFile = cmdReader.GetParameterList().back();

		std::cout << "Fixing " << inFile << "->" << outFile << std::endl;

		try
		{
			// 1. Preparazione Input (RAII)
			// FileDataSource lancia std::runtime_error se il file non può essere aperto
			auto source = std::make_unique<FileDataSource>(inFile);

			// 2. Preparazione Output
			std::ofstream outStream(outFile, std::ios::out | std::ios::binary);
			if (!outStream.is_open())
			{
				std::cerr << "Error: Could not open output file '" << outFile << "' for writing." << std::endl;
				return false;
			}

			// 3. Esecuzione Fix
			// Passiamo la ownership della sorgente e il riferimento allo stream di output
			fixer.Fix(std::move(source), outStream);

			std::cout << "Finished Fixing" << std::endl;
		}
		catch (const std::exception& e)
		{
			// Catturiamo errori di apertura file o altri errori runtime non gestiti internamente da Fixer
			std::cerr << "Fatal Error: " << e.what() << std::endl;
			return false;
		}
	}
	else
	{
		std::cout << "Usage :" << std::endl;
		std::cout << "./vbrfix [--option] [--option] in.mp3 out.mp3" << std::endl;
		std::cout <<
			"options (case sensitive):" << std::endl <<
			"--removeId3v1" << std::endl <<
			"--removeId3v2" << std::endl <<
			"--removeUnknown" << std::endl <<
			"--removeLame" << std::endl <<
			"--keepLame" << std::endl <<
			"--keepLameUpdateCrc" << std::endl <<
			"--XingFrameCrcProtectIfCan" << std::endl;
	}
	return true;
}

bool ConsoleFixer::GetFixerSettingsFromOptions(FixerSettings &settings,
					       const CommandReader::OptionList &optionList)
{
	for (auto iter = optionList.begin();
	     iter != optionList.end(); ++iter)
	{
		const std::string & option = *iter;
		if (option == "removedId3v1")
		{
			settings.SetRemoveType(Mp3ObjectType::ID3V1_TAG, true);
			std::cout << "Remove Id3v1 on" << std::endl;
		}
		else if (option == "removeId3v2")
		{
			settings.SetRemoveType(Mp3ObjectType::ID3V2_TAG, true);
			std::cout << "Remove Id3v2 on" << std::endl;
		}
		else if (option == "removeUnknown")
		{
			settings.SetRemoveType(Mp3ObjectType::UNKNOWN_DATA, true);
			std::cout << "Remove Unknown on" << std::endl;
		}
		else if (option == "removeLame")
		{
			settings.SetLameInfoOption(FixerSettings::LAME_REMOVE);
			std::cout << "Remove Lame on" << std::endl;
		}
		else if (option == "keepLame")
		{
			settings.SetLameInfoOption(FixerSettings::LAME_KEEP);
			std::cout << "KeepLame on" << std::endl;
		}
		else if (option == "keepLameUpdateCrc")
		{
			settings.SetLameInfoOption(FixerSettings::LAME_KEEP_CALC_TAG_CRC);
			std::cout << "Keep Lame Update Crc on" << std::endl;
		}
		else if (option == "XingFrameCrcProtectIfCan")
		{
			settings.setXingFrameCrcOption(FixerSettings::CRC_KEEP_IF_CAN);
			std::cout << "Xing Frame Crc protection if possible is on" << std::endl;
		}
		else
		{
			std::cout << "Option <" << option << "> not understood" << std::endl;
			return false;
		}
	}
	return true;
}