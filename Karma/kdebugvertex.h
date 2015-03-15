#ifndef KDEBUGVERTEX_H
#define KDEBUGVERTEX_H KDebugVertex

#include <KColor>
#include <KVector2D>
#include <KVector3D>

class KDebugVertex
{
public:
  // Constructors
  Q_DECL_CONSTEXPR KDebugVertex();
  Q_DECL_CONSTEXPR explicit KDebugVertex(const KVector3D &position);
  Q_DECL_CONSTEXPR KDebugVertex(const KVector3D &position, const KColor &normal);
  Q_DECL_CONSTEXPR KDebugVertex(const KVector3D &position, const KVector2D &uv);

  // Accessors / Mutators
  Q_DECL_CONSTEXPR const KVector3D& position() const;
  Q_DECL_CONSTEXPR KColor color() const;
  void setPosition(const KVector3D& position);
  void setColor(const KColor& color);

  // OpenGL Helpers
  static const int PositionTupleSize = 3;
  static const int ColorTupleSize = 3;
  static Q_DECL_CONSTEXPR int positionOffset();
  static Q_DECL_CONSTEXPR int colorOffset();
  static Q_DECL_CONSTEXPR int stride();

private:
  KVector3D m_position;
  KVector3D m_color;
};

/*******************************************************************************
 * Inline Implementation
 ******************************************************************************/

// Note: Q_MOVABLE_TYPE means it can be memcpy'd.
Q_DECLARE_TYPEINFO(KDebugVertex, Q_MOVABLE_TYPE);

// Constructors
Q_DECL_CONSTEXPR inline KDebugVertex::KDebugVertex() {}
Q_DECL_CONSTEXPR inline KDebugVertex::KDebugVertex(const KVector3D &position) : m_position(position), m_color(1.0f,1.0f,1.0f) {}
Q_DECL_CONSTEXPR inline KDebugVertex::KDebugVertex(const KVector3D &position, const KColor &color) : m_position(position), m_color(color.redF(), color.greenF(), color.blueF()) {}
Q_DECL_CONSTEXPR inline KDebugVertex::KDebugVertex(const KVector3D &position, const KVector2D &uv) : m_position(position), m_color(uv.x(), uv.y(), 0.0f) {}

// Accessors / Mutators
Q_DECL_CONSTEXPR inline const KVector3D& KDebugVertex::position() const { return m_position; }
Q_DECL_CONSTEXPR inline KColor KDebugVertex::color() const { return KColor(m_color.x(), m_color.y(), m_color.z()); }
void inline KDebugVertex::setPosition(const KVector3D& position) { m_position = position; }
void inline KDebugVertex::setColor(const KColor& color) { m_color = KVector3D(color.redF(), color.greenF(), color.blueF()); }

// OpenGL Helpers
Q_DECL_CONSTEXPR inline int KDebugVertex::positionOffset() { return offsetof(KDebugVertex, m_position); }
Q_DECL_CONSTEXPR inline int KDebugVertex::colorOffset() { return offsetof(KDebugVertex, m_color); }
Q_DECL_CONSTEXPR inline int KDebugVertex::stride() { return sizeof(KDebugVertex); }

#endif // KDEBUGVERTEX_H
