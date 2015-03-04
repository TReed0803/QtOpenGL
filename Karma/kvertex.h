#ifndef KVERTEX_H
#define KVERTEX_H KVertex

#include <KVector3D>

class KVertex
{
public:
  // Constructors
  Q_DECL_CONSTEXPR KVertex();
  Q_DECL_CONSTEXPR explicit KVertex(const KVector3D &position);
  Q_DECL_CONSTEXPR KVertex(const KVector3D &position, const KVector3D &normal);

  // Accessors / Mutators
  Q_DECL_CONSTEXPR const KVector3D& position() const;
  Q_DECL_CONSTEXPR const KVector3D& normal() const;
  void setPosition(const KVector3D& position);
  void setNormal(const KVector3D& normal);

  // OpenGL Helpers
  static const int PositionTupleSize = 3;
  static const int NormalTupleSize = 3;
  static Q_DECL_CONSTEXPR int positionOffset();
  static Q_DECL_CONSTEXPR int normalOffset();
  static Q_DECL_CONSTEXPR int stride();

private:
  KVector3D m_position;
  KVector3D m_normal;
};

/*******************************************************************************
 * Inline Implementation
 ******************************************************************************/

// Note: Q_MOVABLE_TYPE means it can be memcpy'd.
Q_DECLARE_TYPEINFO(KVertex, Q_MOVABLE_TYPE);

// Constructors
Q_DECL_CONSTEXPR inline KVertex::KVertex() {}
Q_DECL_CONSTEXPR inline KVertex::KVertex(const KVector3D &position) : m_position(position), m_normal(1.0f,1.0f,1.0f) {}
Q_DECL_CONSTEXPR inline KVertex::KVertex(const KVector3D &position, const KVector3D &normal) : m_position(position), m_normal(normal) {}

// Accessors / Mutators
Q_DECL_CONSTEXPR inline const KVector3D& KVertex::position() const { return m_position; }
Q_DECL_CONSTEXPR inline const KVector3D& KVertex::normal() const { return m_normal; }
void inline KVertex::setPosition(const KVector3D& position) { m_position = position; }
void inline KVertex::setNormal(const KVector3D& normal) { m_normal = normal; }

// OpenGL Helpers
Q_DECL_CONSTEXPR inline int KVertex::positionOffset() { return offsetof(KVertex, m_position); }
Q_DECL_CONSTEXPR inline int KVertex::normalOffset() { return offsetof(KVertex, m_normal); }
Q_DECL_CONSTEXPR inline int KVertex::stride() { return sizeof(KVertex); }

#endif // KVERTEX_H
