#ifndef OPENGLFRAMEBUFFEROBJECT_H
#define OPENGLFRAMEBUFFEROBJECT_H OpenGLFramebufferObject

#include <QOpenGLFramebufferObject>

class OpenGLTexture;
class OpenGLRenderbufferObject;

class OpenGLFramebufferObjectPrivate;
class OpenGLFramebufferObject
{
public:

  enum Target
  {
    TargetRead             = 0x8CA8,
    TargetDraw             = 0x8CA9
  };

  enum Attachment
  {
    ColorAttachment0            = 0x8CE0,
    ColorAttachment1            = 0x8CE1,
    ColorAttachment2            = 0x8CE2,
    ColorAttachment3            = 0x8CE3,
    ColorAttachment4            = 0x8CE4,
    ColorAttachment5            = 0x8CE5,
    ColorAttachment6            = 0x8CE6,
    ColorAttachment7            = 0x8CE7,
    ColorAttachment8            = 0x8CE8,
    ColorAttachment9            = 0x8CE9,
    ColorAttachment10           = 0x8CEA,
    ColorAttachment11           = 0x8CEB,
    ColorAttachment12           = 0x8CEC,
    ColorAttachment13           = 0x8CED,
    ColorAttachment14           = 0x8CEE,
    ColorAttachment15           = 0x8CEF,
    DepthAttachment             = 0x8D00,
    StencilAttachment           = 0x8D20,
    DepthStencilAttachment      = 0x821A
  };

  enum Status
  {
    Complete                    = 0x8CD5,
    IncompleteAttachment        = 0x8CD6,
    IncompleteMissingAttachment = 0x8CD7,
    IncompleteDrawBuffer        = 0x8CDB,
    IncompleteReadBuffer        = 0x8CDC,
    Unsupported                 = 0x8CDD
  };

  // Note: Add value to attachment to get attachment value
  OpenGLFramebufferObject();
  ~OpenGLFramebufferObject();

  void create();
  void bind();
  void release();
  void attachTexture(Target target, Attachment attachment, OpenGLTexture &texture, int level = 0);
  void attachTexture1D(Target target, Attachment attachment, OpenGLTexture &texture,  int level = 0);
  void attachTexture2D(Target target, Attachment attachment, OpenGLTexture &texture, int level = 0);
  void attachTexture3D(Target target, Attachment attachment, OpenGLTexture &texture, int level = 0, int layer = 0);
  void attachRenderbuffer(Target target, Attachment attachment, OpenGLRenderbufferObject &rBuffer);
  Status status() const;
  int objectId();
  void drawBuffers(Attachment a1);
  void drawBuffers(Attachment a1, Attachment a2);
  void drawBuffers(Attachment a1, Attachment a2, Attachment a3);
  void drawBuffers(Attachment a1, Attachment a2, Attachment a3, Attachment a4);
  void drawBuffers(Attachment a1, Attachment a2, Attachment a3, Attachment a4, Attachment a5);

private:
  OpenGLFramebufferObjectPrivate *m_private;
  void removeReference();
};

#endif // OPENGLFRAMEBUFFEROBJECT_H
