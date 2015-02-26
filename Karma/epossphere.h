#ifndef EPOSSPHERE_H
#define EPOSSPHERE_H


class EposSphere
{
public:

  template <typename It1, typename It2, typename VecAccessor = Karma::DefaultAccessor<KVector3D, KVector3D>, typename AxisAccessor = Karma::DefaultAccessor<KVector3D, KVector3D>>
  EposSphere(It1 bVec, It1 eVec, It2 bAxis, It2 eAxis, VecAccessor vAccessor = Karma::DefaultAccessor<KVector3D, KVector3D>(), AxisAccessor aAccessor = Karma::DefaultAccessor<KVector3D, KVector3D>());
  ~EposSphere();

  float radius() const;
  KVector3D centroid() const;
};

template <typename It1, typename It2, typename VecAccessor = Karma::DefaultAccessor<KVector3D, KVector3D>, typename AxisAccessor = Karma::DefaultAccessor<KVector3D, KVector3D>>
EposSphere::EposSphere(It1 bVec, It1 eVec, It2 bAxis, It2 eAxis, VecAccessor vAccessor, AxisAccessor aAccessor)
{
  // Do nothing (yet)
  /*
  KHalfEdgeMesh::VertexContainer const &vertices = mesh.vertices();
  size_t vertCount = vertices.size();
  size_t normCount = std::distance(beginNorms, endNorms);
  if (vertCount > 2 * normCount)
  {
    std::vector<KVector3D> extremalVerts = Karma::findExtremalPoints(vertices.begin(), vertices.end(), beginNorms, endNorms, KHalfEdgeMesh::VertexPositionPred());
    calculateMinimumSphere(extremalVerts);
    for (KHalfEdgeMesh::Vertex const & v : mesh.vertices())
    {
      expandToContainPoint(v.position);
    }
  }
  else
  {
    calculateMinimumSphere(mesh.vertices());
  }
  */
}

#endif // EPOSSPHERE_H
