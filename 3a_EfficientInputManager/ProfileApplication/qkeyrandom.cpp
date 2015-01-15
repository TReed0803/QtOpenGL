#include "qkeyrandom.h"
#include <QMetaObject>
#include <QMetaProperty>
#include <QMetaEnum>
#include "keyobject.h"

QKeyRandom::QKeyRandom(int seed) : m_generator(seed)
{
  // Find our Qt::Key QMetaEnum
  const QMetaObject &meta = KeyObject::staticMetaObject;
  int propertyIndex = meta.indexOfProperty("KeyEnum");
  QMetaProperty metaProperty = meta.property(propertyIndex);
  QMetaEnum metaEnum = metaProperty.enumerator();

  // Aggregate possible key values
  int keyCount = metaEnum.keyCount();
  for (int i = 0; i < keyCount; ++i)
  {
    m_keys.push_back((Qt::Key)metaEnum.value(i));
  }

  // Initialize the random number distribution
  m_distribution = std::uniform_int_distribution<int>(0, m_keys.size() - 1);
}

Qt::Key QKeyRandom::getRandomKey()
{
  return m_keys[m_distribution(m_generator)];
}

Qt::Key QKeyRandom::getRandomUniqueKey()
{
  int idx = m_distribution(m_generator);
  Qt::Key key = m_keys[idx];
  m_keys.erase(m_keys.begin() + idx);
  m_distribution = std::uniform_int_distribution<int>(0, m_keys.size() - 1);
  return key;
}

