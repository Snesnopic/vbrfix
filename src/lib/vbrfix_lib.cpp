#include "vbrfix/vbrfix.hpp"
#include "VbrFixer.hpp"
#include "FixerSettings.hpp"
#include "FeedBackInterface.hpp"
#include "FileDataSource.hpp"
#include "MemoryDataSource.hpp"
#include <sstream>
#include <stdexcept>
#include <iostream>

namespace vbrfix {

    namespace {
        class LibFeedback : public FeedBackInterface {
        public:
            void update() override {}
            
            void addLogMessage(Log::LogItem log) override {
                //TODO: accumulate logs
            }

            [[nodiscard]] bool HasUserCancelled() const override { return false; }
        };
    }

    std::vector<uint8_t> fix_mp3(const InputData& input, const FixParams& params) {
        FixerSettings settings;
        settings.Defaults();
        settings.SetAlwaysSkip(params.always_skip);
        
        if (params.remove_id3v1) settings.SetRemoveType(Mp3ObjectType::ID3V1_TAG, true);
        if (params.remove_id3v2) settings.SetRemoveType(Mp3ObjectType::ID3V2_TAG, true);
        if (params.remove_unknown) settings.SetRemoveType(Mp3ObjectType::UNKNOWN_DATA, true);
        
        if (params.remove_lame) {
            settings.SetLameInfoOption(FixerSettings::LAME_REMOVE);
        } else if (params.keep_lame) {
            settings.SetLameInfoOption(FixerSettings::LAME_KEEP);
        }

        LibFeedback feedback;
        VbrFixer fixer(feedback, settings);

        std::unique_ptr<IDataSource> source;
        try {
            if (std::holds_alternative<std::filesystem::path>(input)) {
                source = std::make_unique<FileDataSource>(std::get<std::filesystem::path>(input).string());
            } else {
                const auto& bytes = std::get<std::vector<uint8_t>>(input);
                source = std::make_unique<MemoryDataSource>(bytes);
            }
        } catch (const std::exception& e) {
            throw std::runtime_error(std::string("Input initialization failed: ") + e.what());
        }

        std::ostringstream output_stream(std::ios::binary);

        fixer.Fix(std::move(source), output_stream);

        const auto state = fixer.GetProgressDetails().GetState();
        if (state == FixState::ERROR) {
             throw std::runtime_error("VbrFix failed to process the file");
        }
        
        std::string str_out = output_stream.str();
        return std::vector<uint8_t>(str_out.begin(), str_out.end());
    }

}