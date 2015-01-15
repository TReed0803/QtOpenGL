#include <QElapsedTimer>
#include <QVector>
#include <QDebug>
#include <QTime>
#include <iostream>
#include "qkeyrandom.h"
#include "tests.inc"

/*******************************************************************************
 * Key Activators
 ******************************************************************************/
template <typename T>
static void ActivateKeys(int keyCount, int seed)
{
  QKeyRandom keyRand(seed);
  for (int i = 0; i < keyCount; ++i)
  {
    Qt::Key key = keyRand.getRandomUniqueKey();
    T::registerKeyPress(key);
  }
}

template <typename T>
static void DeactivateKeys()
{
  T::reset();
}

/*******************************************************************************
 * Tests
 ******************************************************************************/
struct QueryTest
{
  template <typename Instance>
  struct Runner
  {
    inline static void Run(QKeyRandom &keyRand)
    {
      Instance::keyTriggered(keyRand.getRandomKey());
      Instance::keyPressed(keyRand.getRandomKey());
      Instance::keyReleased(keyRand.getRandomKey());
    }
  };
};

struct UpdateTest
{
  template <typename Instance>
  struct Runner
  {
    inline static void Run(QKeyRandom &keyRand)
    {
      (void)keyRand;
      Instance::update();
    }
  };
};

/*******************************************************************************
 * Test Running Logic
 ******************************************************************************/
template <typename Test, typename Instance>
void runTest(int passes, int iterations, int keysActive, int seed, QKeyRandom randKey)
{
  qint64 total = 0;
  int pass, iteration;
  QElapsedTimer timer;
  ActivateKeys<Instance>(keysActive, seed);
  for (pass = 0; pass < passes; ++pass)
  {
    timer.start();
    for (iteration = 0; iteration < iterations; ++iteration)
    {
      Test::Runner<Instance>::Run(randKey);
    }
    total += timer.elapsed();
  }
  DeactivateKeys<Instance>();
  std::cout << total / float(passes) << ",";
}

#define TEST(name) std::cout << #name " " << type << ","
void printHeader(char const *type)
{
  std::cout << "ActiveKeys,";
#include "tests.h"
  std::cout << std::endl;
}
#undef TEST

template <typename Test>
#define TEST(name) runTest<Test, name>(passes, iterations, keysActive, seed, randKey);
void runTests(char const *type, int passes, int iterations, int keyRange)
{
  int seed;
  printHeader(type);
  QKeyRandom randKey(qrand());
  for (int keysActive = 0; keysActive <= keyRange; ++keysActive)
  {
    seed = qrand();
    std::cout << keysActive << ",";
#include "tests.h"
    std::cout << std::endl;
  }
}
#undef TEST

#include <random>

/*******************************************************************************
 * Main
 ******************************************************************************/
int main(int argc, char *argv[])
{
  // Parse command line
  if (argc < 4)
  {
    qFatal("Expected 3 arguments - <Passes> <Iterations> <KeysActive>. %d provided.", argc);
  }
  if (argc > 4)
  {
    qWarning("Expected 3 arguments - <Passes> <Iterations> <KeysActive>. %d provided. Ignoring extra.", argc);
  }
  const int passes = atoi(argv[1]);
  const int iterations = atoi(argv[2]);
  const int keyRange = atoi(argv[3]);

  // Begin printing csv
  qsrand(QTime::currentTime().msec());
  runTests<QueryTest>("Query", passes, iterations, keyRange);
  runTests<UpdateTest>("Update", passes, iterations, keyRange);

  return 0;
}
