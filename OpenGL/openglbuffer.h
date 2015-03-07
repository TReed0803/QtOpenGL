#ifndef OPENGLBUFFER_H
#define OPENGLBUFFER_H OpenGLBuffer

#include <OpenGLCommon>
#include <QOpenGLBuffer>

// Register to check OpenGLBuffer
class OpenGLBufferChecked : public QOpenGLBuffer
{
public:
  OpenGLBufferChecked() : QOpenGLBuffer() {}
  OpenGLBufferChecked(QOpenGLBuffer::Type type) : QOpenGLBuffer(type) {}
  GL_CHECK(QOpenGLBuffer,bind);
  GL_CHECK(QOpenGLBuffer,create);
  GL_CHECK(QOpenGLBuffer,unmap);
  GL_CHECK(QOpenGLBuffer,read,int,void*,int);
};

// Register to profile OpenGLBuffer
class OpenGLBufferProfiled : public OpenGLBufferChecked
{
public:
  OpenGLBufferProfiled() : OpenGLBufferChecked() {}
  OpenGLBufferProfiled(QOpenGLBuffer::Type type) : OpenGLBufferChecked(type) {}
  GL_PROFILE(OpenGLBufferChecked,allocate,const void *,int);
  GL_PROFILE(OpenGLBufferChecked,allocate,int);
  GL_PROFILE(OpenGLBufferChecked,bind);
  GL_PROFILE(OpenGLBufferChecked,create);
  GL_PROFILE(OpenGLBufferChecked,destroy);
  GL_PROFILE(OpenGLBufferChecked,map,QOpenGLBuffer::Access);
  GL_PROFILE(OpenGLBufferChecked,mapRange,int,int,QOpenGLBuffer::RangeAccessFlags);
  GL_PROFILE(OpenGLBufferChecked,read,int,void *,int);
  GL_PROFILE(OpenGLBufferChecked,release);
  GL_PROFILE(OpenGLBufferChecked,setUsagePattern,QOpenGLBuffer::UsagePattern);
  GL_PROFILE(OpenGLBufferChecked,unmap);
  GL_PROFILE(OpenGLBufferChecked,write,int,const void *,int);
};

// Final class
class OpenGLBuffer : public OpenGLBufferProfiled
{
public:
  enum Type
  {
      VertexBuffer        = 0x8892, // GL_ARRAY_BUFFER
      IndexBuffer         = 0x8893, // GL_ELEMENT_ARRAY_BUFFER
      PixelPackBuffer     = 0x88EB, // GL_PIXEL_PACK_BUFFER
      PixelUnpackBuffer   = 0x88EC, // GL_PIXEL_UNPACK_BUFFER
      ArrayBuffer         = 0x8892,
      UniformBuffer       = 0x8A11
  };

  OpenGLBuffer() : OpenGLBufferProfiled() {}
  OpenGLBuffer(Type type) : OpenGLBufferProfiled(static_cast<QOpenGLBuffer::Type>(type)) {}
  void allocate(const void* data, size_t size) { OpenGLBufferProfiled::allocate(data, static_cast<int>(size)); }
  void allocate(size_t size) { OpenGLBufferProfiled::allocate(static_cast<int>(size)); }
  void *mapRange(size_t offset, size_t count, RangeAccessFlags access) { return OpenGLBufferProfiled::mapRange(static_cast<int>(offset), static_cast<int>(count), access); }
  void release() { OpenGLBufferProfiled::release(); }
  static void release(Type type) { OpenGLBufferProfiled::release((QOpenGLBuffer::Type)type); }
};

#endif // OPENGLBUFFER_H
