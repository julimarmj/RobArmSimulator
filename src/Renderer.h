#pragma once

#include <GL/glew.h>

#include "Debug.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "glm/glm.hpp"

class Renderer
{
    public:
        void Clear() const;
        void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
        void DrawLines(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
        void DrawLineStrip(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};
