//
// Created by Matt Blair on 4/2/16.
//
#pragma once

#include <string>
#include <vector>

namespace stock {

class UniformLocation {

public:
    UniformLocation(const std::string& name) : name(name) {}

private:
    const std::string name;

    mutable int location = -1;
    mutable int generation = -1;

    friend class ShaderProgram;
};

class ShaderUniform {

public:

    ShaderUniform();
    ~ShaderUniform();

    ShaderUniform(const ShaderUniform& other);
    ShaderUniform(ShaderUniform&& other);

    // Update the uniform to store the given value; returns 'true' if the new
    // value is different from the old value.
    bool update(int value);
    bool update(float value);
    bool update(int* array, size_t count);
    bool update(float* array, size_t count);

    // If the uniform is an array, returns the number of elements; otherwise
    // returns zero.
    size_t count();

private:
    enum class ValueType : uint8_t {
        SCALAR_INT,
        SCALAR_FLOAT,
        ARRAY_INT,
        ARRAY_FLOAT,
    };

    union {
        int m_scalarInt = 0;
        float m_scalarFloat;
        int* m_arrayInt;
        float* m_arrayFloat;
    };

    size_t m_count = 0;

    ValueType m_type = ValueType::SCALAR_INT;

    void deleteArrays();

    size_t arrayByteCount();

    friend class ShaderProgram;
};

} // namespace stock
