#ifndef OPENGLUNIFORMBUFFEROBJECT_H
#define OPENGLUNIFORMBUFFEROBJECT_H OpenGLUniformBufferObject

#include <OpenGLBuffer>

class OpenGLUniformBufferObjectPrivate;
class OpenGLUniformBufferObject : private OpenGLBuffer
{
public:
  OpenGLUniformBufferObject();
  ~OpenGLUniformBufferObject();

  OpenGLUniformBufferObject &operator=(const OpenGLUniformBufferObject &other);

  Q_DECLARE_FLAGS(RangeAccessFlags, RangeAccessFlag)

  QOpenGLBuffer::UsagePattern usagePattern() const;
  void setUsagePattern(QOpenGLBuffer::UsagePattern value);

  bool create();
  bool isCreated() const;

  void destroy();

  bool bind(int uniformIndex);
  void release();

  GLuint bufferId() const;
  GLuint locationId() const;

  int size() const;

  bool read(int offset, void *data, int count);
  void write(int offset, const void *data, int count);

  void allocate(const void *data, int count);
  void allocate(int count);

  void *map(QOpenGLBuffer::Access access);
  void *mapRange(int offset, int count, QOpenGLBuffer::RangeAccessFlags access);
  bool unmap();

private:
  OpenGLUniformBufferObjectPrivate *m_private;
};

inline OpenGLUniformBufferObject &OpenGLUniformBufferObject::operator=(const OpenGLUniformBufferObject &other)
{
  this->OpenGLBuffer::operator=(other);
  return *this;
}

inline QOpenGLBuffer::UsagePattern OpenGLUniformBufferObject::usagePattern() const
{
  return this->OpenGLBuffer::usagePattern();
}

inline void OpenGLUniformBufferObject::setUsagePattern(QOpenGLBuffer::UsagePattern value)
{
  this->OpenGLBuffer::setUsagePattern(value);
}

inline bool OpenGLUniformBufferObject::create()
{
  return this->OpenGLBuffer::create();
}

inline bool OpenGLUniformBufferObject::isCreated() const
{
  return this->OpenGLBuffer::isCreated();
}

inline void OpenGLUniformBufferObject::destroy()
{
  this->OpenGLBuffer::destroy();
}

inline GLuint OpenGLUniformBufferObject::bufferId() const
{
  return this->OpenGLBuffer::bufferId();
}

inline int OpenGLUniformBufferObject::size() const
{
  return this->OpenGLBuffer::size();
}

inline bool OpenGLUniformBufferObject::read(int offset, void *data, int count)
{
  return this->OpenGLBuffer::read(offset, data, count);
}

inline void OpenGLUniformBufferObject::write(int offset, const void *data, int count)
{
  this->OpenGLBuffer::write(offset, data, count);
}

inline void OpenGLUniformBufferObject::allocate(const void *data, int count)
{
  this->OpenGLBuffer::allocate(data, count);
}

inline void OpenGLUniformBufferObject::allocate(int count)
{
  this->OpenGLBuffer::allocate(0, count);
}

inline void *OpenGLUniformBufferObject::map(QOpenGLBuffer::Access access)
{
  return this->OpenGLBuffer::map(access);
}

inline void *OpenGLUniformBufferObject::mapRange(int offset, int count, QOpenGLBuffer::RangeAccessFlags access)
{
  return this->OpenGLBuffer::mapRange(offset, count, access);
}

inline bool OpenGLUniformBufferObject::unmap()
{
  return this->OpenGLBuffer::unmap();
}

#endif // OPENGLUNIFORMBUFFEROBJECT_H
