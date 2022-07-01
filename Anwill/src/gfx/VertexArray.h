#pragma once

#include <memory>
#include <vector>

#include "gfx/VertexBuffer.h"

namespace Anwill {

    enum class ShaderDataType
    {
        Float = 1, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
    };

    /**
     * An element that belongs to a layout. Has a
     * - data type
     * - size of the data type
     * - offset from the first element in the layout
     * - number of components (count) of the data type
     * - if the data should be normalized or when sent
     */
    struct BufferElement
    {
    public:
        ShaderDataType type;
        unsigned int size, offset, count;
        bool normalized;

        BufferElement(ShaderDataType type, bool normalized = false);

        /**
         * Size of the data type in bytes.
         */
        static unsigned int GetSizeOfType(ShaderDataType type);
        /**
         * Number of components in the type. For example, float2 has two components.
         */
        static unsigned int GetCountOfType(ShaderDataType type);
    };

    /**
     * The layout of a VertexBuffer that is fed to the vertex shader.
     */
    class BufferLayout
    {
    public:
        BufferLayout(std::vector<BufferElement>&& elements);

        const std::vector<BufferElement> GetElements() const;
        const unsigned int GetStride() const;

    private:
        std::vector<BufferElement> m_Elements;
        unsigned int m_Stride;

        void CalcStrideAndOffset();
    };

    class VertexArray
    {
    public:
        /**
         * @return new and "empty" Vertex Array. Use AddBuffer().
         */
        static std::shared_ptr<VertexArray> Create();

        virtual void AddBuffer(const VertexBuffer& vertexBuffer, const BufferLayout& layout) = 0;
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
    };

}