#pragma once
#include <GL/glew.h>
class IndexBuffer
{
private:
    unsigned int m_RendererID;
    unsigned int m_Count;

public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    IndexBuffer();
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;
    void AddData(const unsigned int* data, unsigned int count);

    inline unsigned int GetCount() const { return m_Count; }
    inline void setCount(unsigned int count) { m_Count = count; }
};
