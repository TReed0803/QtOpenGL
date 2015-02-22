#ifndef KVERTEX_H
#define KVERTEX_H KVertex

#include <KColor>
#include <KVector3D>

class KVertex
{
public:
  // Constructors
  Q_DECL_CONSTEXPR KVertex();
  Q_DECL_CONSTEXPR explicit KVertex(const KVector3D &position);
  Q_DECL_CONSTEXPR KVertex(const KVector3D &position, const KVector3D &color);
  KVertex(const KVector3D &position, const KColor &color);

  // Accessors / Mutators
  Q_DECL_CONSTEXPR const KVector3D& position() const;
  Q_DECL_CONSTEXPR const KVector3D& color() const;
  void setPosition(const KVector3D& position);
  void setColor(const KVector3D& color);

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
Q_DECLARE_TYPEINFO(KVertex, Q_MOVABLE_TYPE);

// Constructors
Q_DECL_CONSTEXPR inline KVertex::KVertex() {}
Q_DECL_CONSTEXPR inline KVertex::KVertex(const KVector3D &position) : m_position(position), m_color(1.0f,1.0f,1.0f) {}
Q_DECL_CONSTEXPR inline KVertex::KVertex(const KVector3D &position, const KVector3D &color) : m_position(position), m_color(color) {}
inline KVertex::KVertex(const KVector3D &position, const KColor &color) : m_position(position), m_color(color.redF(), color.greenF(), color.blueF()) {}

// Accessors / Mutators
Q_DECL_CONSTEXPR inline const KVector3D& KVertex::position() const { return m_position; }
Q_DECL_CONSTEXPR inline const KVector3D& KVertex::color() const { return m_color; }
void inline KVertex::setPosition(const KVector3D& position) { m_position = position; }
void inline KVertex::setColor(const KVector3D& color) { m_color = color; }

// OpenGL Helpers
Q_DECL_CONSTEXPR inline int KVertex::positionOffset() { return offsetof(KVertex, m_position); }
Q_DECL_CONSTEXPR inline int KVertex::colorOffset() { return offsetof(KVertex, m_color); }
Q_DECL_CONSTEXPR inline int KVertex::stride() { return sizeof(KVertex); }

#endif // KVERTEX_H
