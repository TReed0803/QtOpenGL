#ifndef KDEBUGDRAW_H
#define KDEBUGDRAW_H

class KCamera3D;
class KColor;
class KMatrix4x4;
class KRectF;
class KVector3D;

struct KDebugDraw
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
  static void draw(KCamera3D &camera, KMatrix4x4 const &view);
  static void teardown();
};

#endif // KDEBUGDRAW_H
