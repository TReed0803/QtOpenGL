#ifndef KEPOSSPHERE_H
#define KEPOSSPHERE_H KEposSphere

#include <KMath>
typedef std::vector<Karma::MinMaxKVector3D> KMinMaxVectorCloud;

class KEposSphere
{
public:

  typedef KMinMaxVectorCloud::const_iterator const_iterator;

  template <typename It1, typename It2, typename VecAccessor = Karma::DefaultAccessor<KVector3D>, typename AxisAccessor = Karma::DefaultAccessor<KVector3D>>
  KEposSphere(It1 bVec, It1 eVec, It2 bAxis, It2 eAxis, VecAccessor vAccessor = Karma::DefaultAccessor<KVector3D>(), AxisAccessor aAccessor = Karma::DefaultAccessor<KVector3D>());
  void calculateMinimumSphere(const_iterator begin, const_iterator end);

  float radius;
  KVector3D centroid;
};

template <typename It1, typename It2, typename VecAccessor, typename AxisAccessor>
KEposSphere::KEposSphere(It1 bVec, It1 eVec, It2 bAxis, It2 eAxis, VecAccessor vAccessor, AxisAccessor aAccessor)
{
  KMinMaxVectorCloud extremalVerts = Karma::findExtremalPointsAlongAxes(bVec, eVec, bAxis, eAxis, vAccessor, aAccessor);
  calculateMinimumSphere(extremalVerts.begin(), extremalVerts.end());
}

#endif // KEPOSSPHERE_H
