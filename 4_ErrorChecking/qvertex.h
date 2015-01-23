#ifndef QVERTEX_H
#define QVERTEX_H

#include <QVector3D>

class QVertex
{
public:
  // Constructors
  Q_DECL_CONSTEXPR QVertex();
  Q_DECL_CONSTEXPR explicit QVertex(const QVector3D &position);
  Q_DECL_CONSTEXPR QVertex(const QVector3D &position, const QVector3D &color);

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
Q_DECLARE_TYPEINFO(QVertex, Q_MOVABLE_TYPE);

// Constructors
Q_DECL_CONSTEXPR inline QVertex::QVertex() {}
Q_DECL_CONSTEXPR inline QVertex::QVertex(const QVector3D &position) : m_position(position) {}
Q_DECL_CONSTEXPR inline QVertex::QVertex(const QVector3D &position, const QVector3D &color) : m_position(position), m_color(color) {}

// Accessors / Mutators
Q_DECL_CONSTEXPR inline const QVector3D& QVertex::position() const { return m_position; }
Q_DECL_CONSTEXPR inline const QVector3D& QVertex::color() const { return m_color; }
void inline QVertex::setPosition(const QVector3D& position) { m_position = position; }
void inline QVertex::setColor(const QVector3D& color) { m_color = color; }

// OpenGL Helpers
Q_DECL_CONSTEXPR inline int QVertex::positionOffset() { return offsetof(QVertex, m_position); }
Q_DECL_CONSTEXPR inline int QVertex::colorOffset() { return offsetof(QVertex, m_color); }
Q_DECL_CONSTEXPR inline int QVertex::stride() { return sizeof(QVertex); }

#endif // QVERTEX_H
