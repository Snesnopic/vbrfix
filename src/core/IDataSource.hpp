#ifndef IDATASOURCE_HPP
#define IDATASOURCE_HPP

#include <cstdint>
#include <cstddef>

class IDataSource {
public:
    virtual ~IDataSource() = default;

    virtual bool read(uint8_t* dest, size_t size) = 0;

    virtual bool seek(size_t position) = 0;

    [[nodiscard]] virtual size_t position() const = 0;

    [[nodiscard]] virtual size_t length() const = 0;

    [[nodiscard]] virtual bool good() const = 0;
};

#endif