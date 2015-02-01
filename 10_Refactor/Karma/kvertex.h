#ifndef KVERTEX_H
#define KVERTEX_H

#include <KVector3D>

class KVertex
{
public:
  // Constructors
  Q_DECL_CONSTEXPR KVertex();
  Q_DECL_CONSTEXPR explicit KVertex(const QVector3D &position);
  Q_DECL_CONSTEXPR KVertex(const QVector3D &position, const QVector3D &color);
  Q_DECL_CONSTEXPR KVertex(const QVector3D &position, const QVector3D &color, const QVector3D &normal);

  // Accessors / Mutators
  Q_DECL_CONSTEXPR const QVector3D& position() const;
  Q_DECL_CONSTEXPR const QVector3D& color() const;
  Q_DECL_CONSTEXPR const QVector3D& normal() const;
  void setPosition(const QVector3D& position);
  void setColor(const QVector3D& color);
  void setNormal(const QVector3D& normal);

  // OpenGL Helpers
  static const int PositionTupleSize = 3;
  static const int ColorTupleSize = 3;
  static const int NormalTupleSize = 3;
  static Q_DECL_CONSTEXPR int positionOffset();
  static Q_DECL_CONSTEXPR int colorOffset();
  static Q_DECL_CONSTEXPR int normalOffset();
  static Q_DECL_CONSTEXPR int stride();

private:
  KVector3D m_position;
  KVector3D m_color;
  KVector3D m_normal;
};

/*******************************************************************************
 * Inline Implementation
 ******************************************************************************/

// Note: Q_MOVABLE_TYPE means it can be memcpy'd.
Q_DECLARE_TYPEINFO(KVertex, Q_MOVABLE_TYPE);

// Constructors
Q_DECL_CONSTEXPR inline KVertex::KVertex() {}
Q_DECL_CONSTEXPR inline KVertex::KVertex(const QVector3D &position) : m_position(position), m_color(1.0f,1.0f,1.0f) {}
Q_DECL_CONSTEXPR inline KVertex::KVertex(const QVector3D &position, const QVector3D &color) : m_position(position), m_color(color) {}
Q_DECL_CONSTEXPR inline KVertex::KVertex(const QVector3D &position, const QVector3D &color, const QVector3D &normal) : m_position(position), m_color(color), m_normal(normal) {}

// Accessors / Mutators
Q_DECL_CONSTEXPR inline const QVector3D& KVertex::position() const { return m_position; }
Q_DECL_CONSTEXPR inline const QVector3D& KVertex::color() const { return m_color; }
Q_DECL_CONSTEXPR inline const QVector3D& KVertex::normal() const { return m_normal; }
void inline KVertex::setPosition(const QVector3D& position) { m_position = position; }
void inline KVertex::setColor(const QVector3D& color) { m_color = color; }
void inline KVertex::setNormal(const QVector3D& normal) { m_normal = normal; }

// OpenGL Helpers
Q_DECL_CONSTEXPR inline int KVertex::positionOffset() { return offsetof(KVertex, m_position); }
Q_DECL_CONSTEXPR inline int KVertex::colorOffset() { return offsetof(KVertex, m_color); }
Q_DECL_CONSTEXPR inline int KVertex::normalOffset() { return offsetof(KVertex, m_normal); }
Q_DECL_CONSTEXPR inline int KVertex::stride() { return sizeof(KVertex); }

#endif // KVERTEX_H
