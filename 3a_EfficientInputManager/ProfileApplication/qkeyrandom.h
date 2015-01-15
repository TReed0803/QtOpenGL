#ifndef QKEYRANDOM_H
#define QKEYRANDOM_H

#include <Qt>
#include <vector>
#include <random>

class QKeyRandom
{
public:
  QKeyRandom(int seed);
  Qt::Key getRandomKey();
  Qt::Key getRandomUniqueKey();
protected:
  std::vector<Qt::Key> m_keys;
  std::mt19937 m_generator;
  std::uniform_int_distribution<int> m_distribution;
};

#endif // QKEYRANDOM_H
