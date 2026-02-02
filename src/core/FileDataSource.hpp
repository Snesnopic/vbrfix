#ifndef FILEDATASOURCE_H
#define FILEDATASOURCE_H

#include <fstream>
#include <string>
#include <stdexcept>

#include "IDataSource.hpp"

class FileDataSource : public IDataSource {
public:
    explicit FileDataSource(const std::string& filename) {
        m_stream.open(filename, std::ios::in | std::ios::binary);
        if (!m_stream.is_open()) {
            throw std::runtime_error("Failed To Open File: " + filename);
        }
        m_stream.seekg(0, std::ios::end);
        m_length = m_stream.tellg();
        m_stream.seekg(0, std::ios::beg);
    }

    bool read(uint8_t* dest, size_t size) override {
        m_stream.read(reinterpret_cast<char*>(dest), size);
        return m_stream.good() || m_stream.eof(); // eof is acceptable if we read partially
    }

    bool seek(size_t position) override {
        m_stream.clear(); // clear potential eof flags
        m_stream.seekg(position, std::ios::beg);
        return m_stream.good();
    }

    size_t position() const override {
        return static_cast<size_t>(const_cast<std::ifstream&>(m_stream).tellg());
    }

    size_t length() const override { return m_length; }

    bool good() const override { return m_stream.good(); }

private:
    std::ifstream m_stream;
    size_t m_length = 0;
};

#endif