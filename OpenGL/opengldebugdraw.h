#ifndef OPENGLDEBUGDRAW_H
#define OPENGLDEBUGDRAW_H OpenGLDebugDraw

class KCamera3D;
class KColor;
class KMatrix4x4;
class KRectF;
class KVector3D;

struct OpenGLDebugDraw
{

  // Drawing in screen space [0,1] from top-let to bottom-right
  struct Screen
  {
    static void drawRect(const KRectF &rect, const KColor &color);
  };

  // Drawing points within the world. (Unmanipulated)
  struct World
  {
    static void drawLine(const KVector3D &from, const KVector3D &to, const KColor &color);
  };

  // Global states
  static void initialize();
  static void draw();
  static void teardown();
};

#endif // OPENGLDEBUGDRAW_H
