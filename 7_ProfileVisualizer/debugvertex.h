#ifndef DEBUGVERTEX_H
#define DEBUGVERTEX_H

#include <QColor>
#include <QVector3D>

class DebugVertex
{
public:
  // Constructors
  Q_DECL_CONSTEXPR DebugVertex();
  Q_DECL_CONSTEXPR explicit DebugVertex(const QVector3D &position);
  Q_DECL_CONSTEXPR DebugVertex(const QVector3D &position, const QColor &color);
  Q_DECL_CONSTEXPR DebugVertex(const QVector3D &position, const QVector3D &color);

  // Accessors / Mutators
  Q_DECL_CONSTEXPR const QVector3D& position() const;
  Q_DECL_CONSTEXPR const QVector3D& color() const;
  void setPosition(const QVector3D& position);
  void setColor(const QVector3D& color);

  // OpenGL Helpers
  static const int PositionTupleSize = 3;
  static const int ColorTupleSize = 3;
  static Q_DECL_CONSTEXPR int positionOffset();
  static Q_DECL_CONSTEXPR int colorOffset();
  static Q_DECL_CONSTEXPR int stride();

private:
  QVector3D m_position;
  QVector3D m_color;
};

/*******************************************************************************
 * Inline Implementation
 ******************************************************************************/

// Note: Q_MOVABLE_TYPE means it can be memcpy'd.
Q_DECLARE_TYPEINFO(DebugVertex, Q_MOVABLE_TYPE);

// Constructors
Q_DECL_CONSTEXPR inline DebugVertex::DebugVertex() {}
Q_DECL_CONSTEXPR inline DebugVertex::DebugVertex(const QVector3D &position) : m_position(position) {}
Q_DECL_CONSTEXPR inline DebugVertex::DebugVertex(const QVector3D &position, const QColor &color) : m_position(position), m_color(color.redF(), color.greenF(), color.blueF()) {}
Q_DECL_CONSTEXPR inline DebugVertex::DebugVertex(const QVector3D &position, const QVector3D &color) : m_position(position), m_color(color) {}

// Accessors / Mutators
Q_DECL_CONSTEXPR inline const QVector3D& DebugVertex::position() const { return m_position; }
Q_DECL_CONSTEXPR inline const QVector3D& DebugVertex::color() const { return m_color; }
void inline DebugVertex::setPosition(const QVector3D& position) { m_position = position; }
void inline DebugVertex::setColor(const QVector3D& color) { m_color = color; }

// OpenGL Helpers
Q_DECL_CONSTEXPR inline int DebugVertex::positionOffset() { return offsetof(DebugVertex, m_position); }
Q_DECL_CONSTEXPR inline int DebugVertex::colorOffset() { return offsetof(DebugVertex, m_color); }
Q_DECL_CONSTEXPR inline int DebugVertex::stride() { return sizeof(DebugVertex); }

#endif // DEBUGVERTEX_H
