#ifndef OPENGLDEBUGDRAW_H
#define OPENGLDEBUGDRAW_H OpenGLDebugDraw

class KQuaternion;
class KColor;
class KRectF;
class KMatrix3x3;
class KVector3D;
class OpenGLUniformBufferObject;
class OpenGLTexture;

struct OpenGLDebugDraw
{

  // Drawing in screen space [0,1] from top-let to bottom-right
  struct Screen
  {
    static void drawRect(const KRectF &rect, const KColor &color);
    static void drawTexture(const KRectF &rect, OpenGLTexture &texture);
  };

  // Drawing points within the world. (Unmanipulated)
  struct World
  {
    static void drawPoint(const KVector3D &point, const KColor &color);
    static void drawOval(const KVector3D &center, const KVector3D &normal, const KVector3D &up, float upRadius, float rightRadius, const KColor &color);
    static void drawOval(const KVector3D &center, const KVector3D &normal, const KVector3D &up, float upRadius, float rightRadius, int segments, const KColor &color);
    static void drawObb(const KVector3D &center, const KMatrix3x3 &axes, const KVector3D &halfLengths, const KColor &color);
    static void drawCircle(const KVector3D &center, const KVector3D &normal, float radius, const KColor &color);
    static void drawCircle(const KVector3D &center, const KVector3D &normal, float radius, int segments, const KColor &color);
    static void drawSphere(const KVector3D &center, float radius, const KColor &color);
    static void drawSphere(const KVector3D &center, float radius, int segments, int rings, const KColor &color);
    static void drawSphere(const KVector3D &center, float radius, int segments, int rings, int subdivisions, const KColor &color);
    static void drawAabb(const KVector3D &min, const KVector3D &max, const KColor &color);
    static void drawLine(const KVector3D &from, const KVector3D &to, const KColor &color);
  };

  // Global states
  static void initialize();
  static void draw();
  static void teardown();
};

#endif // OPENGLDEBUGDRAW_H
