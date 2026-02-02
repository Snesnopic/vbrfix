#ifndef MEMORYDATASOURCE_HPP
#define MEMORYDATASOURCE_HPP

#include <vector>
#include <algorithm>
#include <cstring>
#include "IDataSource.hpp"

class MemoryDataSource : public IDataSource {
public:
    explicit MemoryDataSource(const std::vector<uint8_t>& data)
        : m_data(data), m_cursor(0) {}

    bool read(uint8_t* dest, const size_t size) override {
        if (m_cursor >= m_data.size()) return false;

        const size_t bytesToRead = std::min(size, m_data.size() - m_cursor);
        std::memcpy(dest, &m_data[m_cursor], bytesToRead);
        m_cursor += bytesToRead;
        return true;
    }

    bool seek(const size_t position) override {
        if (position > m_data.size()) return false;
        m_cursor = position;
        return true;
    }

    [[nodiscard]] size_t position() const override { return m_cursor; }

    [[nodiscard]] size_t length() const override { return m_data.size(); }

    [[nodiscard]] bool good() const override { return m_cursor <= m_data.size(); }

private:
    const std::vector<uint8_t>& m_data;
    size_t m_cursor;
};

#endif