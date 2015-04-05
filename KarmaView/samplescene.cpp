#include "samplescene.h"

// Standard Template Library
#include <vector>
#include <time.h>

// Karma Framework
#include <KCamera3D>
#include <KDebug>
#include <KElapsedTimer>
#include <KHalfEdgeMesh>
#include <KLinq>
#include <KMacros>
#include <KMath>
#include <KString>
#include <KVector3D>
#include <KInputManager>
#include <KMath>

// Bounding Volumes / BVH
#include <KAabbBoundingVolume>
#include <KSphereBoundingVolume>
#include <KEllipsoidBoundingVolume>
#include <KOrientedBoundingVolume>
#include <KStaticGeometry>
#include <KAdaptiveOctree>
#include <KBspTree>

// OpenGL Framework
#include <OpenGLInstance>
#include <OpenGLMaterial>
#include <OpenGLMeshManager>
#include <OpenGLViewport>
#include <OpenGLDirectionLight>
#include <OpenGLSpotLight>
#include <OpenGLPointLight>
#include <OpenGLDirectionLightGroup>
#include <OpenGLSpotLightGroup>
#include <OpenGLPointLightGroup>
#include <OpenGLContext>
#include <OpenGLWidget>
#include <OpenGLEnvironment>

#define MULTI_MESH
#define SQUARE_GRID

class SampleScenePrivate
{
public:
  KCamera3D m_camera;
  OpenGLViewport m_viewport;
  std::vector<std::vector<OpenGLInstance *>> m_instances;
  std::array<KStaticGeometry, 4> m_staticGeometry;
  KAdaptiveOctree m_octree;
  KBspTree m_bspTree;

  // Object Manipulation
  void loadObj(const char *fileName);
  void loadObj(const KString &fileName);

  template <typename T>
  void buildMethod(T &geom, KHalfEdgeMesh const &mesh, typename T::BuildMethod method, typename T::TerminationPred pred);
};

void SampleScenePrivate::loadObj(const char *fileName)
{
  loadObj(KString(fileName));
}

void SampleScenePrivate::loadObj(const KString &fileName)
{
  OpenGLMesh openGLMesh;
  KHalfEdgeMesh halfEdgeMesh;
  KCountResult boundaries;

  // Boundary Query
  auto query =
    COUNT
      FROM  ( edge : halfEdgeMesh.halfEdges() )
      WHERE ( edge.face == 0 )
      INCREMENT (1);

  // Initialize an object
  quint64 ms;
  KElapsedTimer timer;
  {
    // Load Half Edge Mesh
    {
      timer.start();
      halfEdgeMesh.create(qPrintable(fileName));
      ms = timer.elapsed();
      kDebug() << "Create HalfEdgeMesh (sec)    :" << float(ms) / 1e3f;
    }
    // Calculate Normals
    {
      timer.start();
      halfEdgeMesh.calculateVertexNormals();
      ms = timer.elapsed();
      kDebug() << "Calculate Normals (sec)      :" << float(ms) / 1e3f;
    }
    // Center the mesh
    {
      timer.start();
      halfEdgeMesh.fixToCenter();
      ms = timer.elapsed();
      kDebug() << "Center Volume (sec)          :" << float(ms) / 1e3f;
    }
    // Normalize the mesh
    {
      timer.start();
      halfEdgeMesh.normalizeVertices();
      ms = timer.elapsed();
      kDebug() << "Normalization (sec)          :" << float(ms) / 1e3f;
    }
    // Calculate OpenGLMesh
    {
      timer.start();
      openGLMesh.create(halfEdgeMesh);
      ms = timer.elapsed();
      kDebug() << "Create OpenGLMesh (sec)      :" << float(ms) / 1e3f;
    }
    // Query Boundaries
    {
      timer.start();
      boundaries = query();
      ms = timer.elapsed();
      kDebug() << "Mesh Query Time (sec)        :" << float(ms) / 1e3f;
    }
    // Generate the Bounding Volumes
    {
      timer.start();
      KAabbBoundingVolume aabbMinMax(halfEdgeMesh, KAabbBoundingVolume::MinMaxMethod);
      KSphereBoundingVolume sphereCentroid(halfEdgeMesh, KSphereBoundingVolume::CentroidMethod);
      KSphereBoundingVolume sphereLarssons(halfEdgeMesh, KSphereBoundingVolume::LarssonsMethod);
      KSphereBoundingVolume spherePca(halfEdgeMesh, KSphereBoundingVolume::PcaMethod);
      KSphereBoundingVolume sphereRitter(halfEdgeMesh, KSphereBoundingVolume::RittersMethod);
      KOrientedBoundingVolume obbPca(halfEdgeMesh, KOrientedBoundingVolume::PcaMethod);
      KEllipsoidBoundingVolume ellipsoidPca(halfEdgeMesh, KEllipsoidBoundingVolume::PcaMethod);
      ms = timer.elapsed();
      kDebug() << "Bounding Volume Gen. (sec)   :" << float(ms) / 1e3f;
    }
    // Generate the Bounding Volume Hierarchy
    {
      timer.start();
      auto depthPred =
        [](size_t numTriangles, size_t depth)->bool
        {
          (void)numTriangles;
          return depth >= 7;
        };
      auto trianglePred =
        [](size_t numTriangles, size_t depth)->bool
        {
          (void)depth;
          return numTriangles < 300;
        };
      //buildMethod(m_staticGeometry[0], halfEdgeMesh, KStaticGeometry::BottomUpMethod, depthPred);
      //buildMethod(m_staticGeometry[1], halfEdgeMesh, KStaticGeometry::BottomUpMethod, trianglePred);
      //buildMethod(m_staticGeometry[2], halfEdgeMesh, KStaticGeometry::TopDownMethod, depthPred);
      //buildMethod(m_staticGeometry[3], halfEdgeMesh, KStaticGeometry::TopDownMethod, trianglePred);
      //buildMethod(m_octree           , halfEdgeMesh, KAdaptiveOctree::TopDownMethod, trianglePred);
      //buildMethod(m_bspTree          , halfEdgeMesh, KBspTree::TopDownMethod, trianglePred);
      ms = timer.elapsed();
      kDebug() << "BV Hierarchy Gen. (sec)      :" << float(ms) / 1e3f;
    }
    kDebug() << "--------------------------------------";
    kDebug() << "Mesh Vertexes  :" << halfEdgeMesh.numVertices();
    kDebug() << "Mesh Faces     :" << halfEdgeMesh.numFaces();
    kDebug() << "Mesh HalfEdges :" << halfEdgeMesh.numHalfEdges();
    kDebug() << "Boundary Edges :" << boundaries;
    kDebug() << "Polygons/Frame :" << halfEdgeMesh.numFaces() * m_instances.size();
  }

  for (std::vector<OpenGLInstance *> layer : m_instances)
  {
    for (OpenGLInstance *instance : layer)
    {
      instance->setMesh(openGLMesh);
    }
  }
}

template <typename T>
void SampleScenePrivate::buildMethod(T &geom, KHalfEdgeMesh const &mesh, typename T::BuildMethod method, typename T::TerminationPred pred)
{
  KTransform3D transform;
  geom.clear();
  for (int i = 0; i < 4; ++i)
  {
    static const float radius = 10.0f;
    float rads = float(i * Karma::Pi) / 2;
    transform.setTranslation(cos(rads) * radius, 0.0f, sin(rads) * radius);
    geom.addGeometry(mesh, transform);
  }
  geom.build(method, pred);
}

SampleScene::SampleScene() :
  m_private(new SampleScenePrivate)
{
  // Intentionally Empty
}

SampleScene::~SampleScene()
{
  // Intentionally Empty
}

void SampleScene::start()
{
  OpenGLScene::start();
  P(SampleScenePrivate);

  // Initialize the camera
  p.m_camera.setTranslation(0.0f, -20.0f, 0.0f);
  p.m_camera.setRotation(90.0f, 1.0f, 0.0f, 0.0f);
  p.m_viewport.create();
  p.m_viewport.setCamera(p.m_camera);
  p.m_viewport.activate();

  // Initialize the Direction Light Group
  for (int i = 0; i < 1; ++i)
  {
    static const float Intensity = 1.0f;
    OpenGLDirectionLight *light = createDirectionLight();
    light->setDiffuse(Intensity, Intensity, Intensity);
    light->setSpecular(0.1f, 0.1f, 0.1f);
  }

  // Initialize the Point Light Group
  srand(time(NULL));
  for (int i = 0; i < 0; ++i)
  {
    OpenGLPointLight *light = createPointLight();
    light->setRadius(200.0f);
    light->setAttenuation(1.0f, 0.01f, 0.0f);
    light->setDiffuse(float(rand())/RAND_MAX,float(rand())/RAND_MAX,float(rand())/RAND_MAX);
  }

  // Initialize the Spot Light Group
  for (int i = 0; i < 0; ++i)
  {
    OpenGLSpotLight *light = createSpotLight();
    light->setAttenuation(1.0f, 0.01f, 0.0f);
    light->setShadowCasting(true);
    light->setInnerAngle(40.0f);
    light->setOuterAngle(45.0f);
    light->setDiffuse(1.0f, 1.0f, 1.0f);
    light->setDepth(25.0f);
  }

  // Create Floor mesh
  KHalfEdgeMesh floorMesh;
  OpenGLMesh floorMeshGL;
  floorMesh.create(":/resources/objects/floor.obj");
  floorMesh.calculateVertexNormals();
  floorMeshGL.create(floorMesh);
  OpenGLMeshManager::setMesh("Floor", floorMeshGL);

  // Create the floor material
  OpenGLMaterial floorMaterial;
  floorMaterial.create();
  floorMaterial.setBaseColor(132.0f / 255.0f, 21.0f / 255.0f, 176.0f / 255.0f);
  floorMaterial.setMetallic(0.2f);
  floorMaterial.setRoughness(0.4f);

  // Note: Currently there is no Material System.
  //       All material properties are per-instance.
  /*
  OpenGLInstance *floor = createInstance();
  floor->setMesh(floorMeshGL);
  floor->setMaterial(floorMaterial);
  floor->transform().setScale(1000.0f);
  floor->transform().setTranslation(0.0f, -1.0f, 0.0f);
  //*/

  // Create instance data
#ifdef MULTI_MESH
  static const int MetallicCount = 4;
  static const int RoughnessCount = 8;
  p.m_instances.resize(MetallicCount + 1);
  for (int i = 0; i <= MetallicCount; ++i)
  {
    for (int j = 0; j <= RoughnessCount; ++j)
    {
      OpenGLMaterial material;
      material.create();
      material.setBaseColor(255.0f / 255.0f, 191.0f / 255.0f, 0.0f / 255.0f);
      material.setBaseColor(1.0f, 0.3f, 0.3f);
      material.setMetallic(float(i) / MetallicCount);
      material.setRoughness(float(j) / RoughnessCount);
      OpenGLInstance *instance = createInstance();
      instance->setMaterial(material);
      p.m_instances[i].push_back(instance);
    }
  }
#else

  // Create the instance material
  OpenGLMaterial material;
  material.create();
  material.setBaseColor(0.01f);
  material.setMetallic(0.02f);
  material.setRoughness(0.1f);

  // Create the instance
  OpenGLInstance *instance = createInstance();
  instance->setMaterial(material);
  p.m_instances.resize(1);
  p.m_instances[0].push_back(instance);

#endif

  // Load the SharedMesh
  p.loadObj(":/resources/objects/sphere.obj");

  // Create the environment (for now, assume one global environment)
  // Note: Implementation could theoretically involve multiple environment maps.
  //       The current "active envrionment" for an object would be whichever environment
  //       the object is mostly in. (Blending could be done, but this might be enough.)
  // Warning: I could not create a resource file for the environment maps. They are too big. :(
  //          This is obviously not ideal. I would rather distribute an application with the
  //          environment maps. At the time, this must be hardcoded. (Will have to find a fix later.)
  //          This means the code will only run on my machine unless you change the path.
  OpenGLEnvironment *env = environment();
  env->setToneMappingFunction(new OpenGLStandardToneMapping(1.0f, 2.2f));
  env->setDirect(":/resources/images/AlexsApt.hdr");
  env->setIndirect(":/resources/images/AlexsApt_Env.hdr");
}

void SampleScene::update(KUpdateEvent *event)
{
  P(SampleScenePrivate);
  (void)event;

  // Update Lights (Scene update)
  static float f = 0.0f;
  f += 0.0016f;
  f = 0.0f;
  float angle = f;
  for (OpenGLDirectionLight *light : directionLights())
  {
    light->setDirection(std::cos(angle), -1, std::sin(angle));
  }
  for (OpenGLPointLight *instance : pointLights())
  {
    static const float radius = 7.0f;
    instance->setTranslation(cos(angle) * radius, 2.0f, sin(angle) * radius);
    angle += 2 * 3.1415926 / pointLights().size();
  }
  angle = f;

  for (OpenGLSpotLight *instance : spotLights())
  {
    static const float radius = 5.0f;
    instance->setTranslation(cos(angle) * radius, /*5.0f + std::sin(angle * 15.0f) * 5.0f*/ 2.5f, sin(angle) * radius);
    instance->setDirection(-instance->translation().normalized());
    angle += 2 * 3.1415926 / spotLights().size();
  }

#ifdef SQUARE_GRID
  size_t layerCount = p.m_instances.size();
  for (int i = 0; i < layerCount; ++i)
  {
    size_t objectCount = p.m_instances[i].size();
    for (int j = 0; j < objectCount; ++j)
    {
      static const float SeparationX = 1.7f * objectCount;
      static const float SeparationY = 2.0f * layerCount;
      p.m_instances[i][j]->currentTransform().setTranslation((-0.5 + float(j) / (objectCount - 1)) * SeparationX, 0.0f, (-0.5 + float(i) / (layerCount - 1)) * SeparationY);
    }
  }
#else
angle = 0.0f;
  for (int i = 0; i < p.m_instances.size(); ++i)
  {
    size_t objectCount = p.m_instances[i].size();
    for (OpenGLInstance *instance : p.m_instances[i])
    {
       const float Radius = 4.0f;
      instance->currentTransform().setTranslation(cos(angle) * Radius, 2.5f * i, sin(angle) * Radius);
      angle += Karma::TwoPi / objectCount;
    }
  }
#endif

  // Camera Selection
  KCamera3D *camera = 0;
  if (KInputManager::buttonPressed(Qt::RightButton))
  {
    camera = &p.m_camera;
  }

  // Camera Transformation
  if (camera)
  {
    float transSpeed = 3.0f;
    float rotSpeed   = 0.5f;

    if (KInputManager::keyPressed(Qt::Key_Control))
    {
      transSpeed = 1.0f;
    }
    transSpeed *= 0.16f;

    // Handle rotations
    camera->rotate(-rotSpeed * KInputManager::mouseDelta().x(), KCamera3D::LocalUp);
    camera->rotate(-rotSpeed * KInputManager::mouseDelta().y(), camera->right());

    // Handle translations
    KVector3D translation;
    if (KInputManager::keyPressed(Qt::Key_W))
    {
      translation += camera->forward();
    }
    if (KInputManager::keyPressed(Qt::Key_S))
    {
      translation -= camera->forward();
    }
    if (KInputManager::keyPressed(Qt::Key_A))
    {
      translation -= camera->right();
    }
    if (KInputManager::keyPressed(Qt::Key_D))
    {
      translation += camera->right();
    }
    if (KInputManager::keyPressed(Qt::Key_E))
    {
      translation -= camera->up();
    }
    if (KInputManager::keyPressed(Qt::Key_Q))
    {
      translation += camera->up();
    }
    camera->translate(transSpeed * translation);
  }

  if (KInputManager::keyPressed(Qt::Key_Control))
  {
    if (KInputManager::keyTriggered(Qt::Key_O))
    {
      OpenGLWidget::sMakeCurrent();
      QString fileName = OpenGLWidget::openFileName("Open Model", ".", "Wavefront Object File (*.obj))");
      if (!fileName.isNull())
      {
        OpenGLContext::currentContext();
        p.loadObj(fileName);
      }
    }
  }

  if (!KInputManager::buttonPressed(Qt::RightButton))
  {
    int amount = 1;
    if (KInputManager::keyPressed(Qt::Key_Shift))
    {
      amount = -1;
    }

    if (KInputManager::keyTriggered(Qt::Key_F))
    {
      OpenGLAbstractLightGroup::FFactor() = (OpenGLAbstractLightGroup::FFactor() + amount) % FresnelCount;
    }

    if (KInputManager::keyTriggered(Qt::Key_G))
    {
      OpenGLAbstractLightGroup::GFactor() = (OpenGLAbstractLightGroup::GFactor() + amount) % GeometryCount;
    }

    if (KInputManager::keyTriggered(Qt::Key_D))
    {
      OpenGLAbstractLightGroup::DFactor() = (OpenGLAbstractLightGroup::DFactor() + amount) % DistributionCount;
    }

    if (KInputManager::keyTriggered(Qt::Key_S))
    {
      OpenGLAbstractLightGroup::SFactor() = (OpenGLAbstractLightGroup::SFactor() + amount) % DistributionCount;
    }

    if (OpenGLAbstractLightGroup::FFactor() < 0) OpenGLAbstractLightGroup::FFactor() = FresnelCount - 1;
    if (OpenGLAbstractLightGroup::GFactor() < 0) OpenGLAbstractLightGroup::GFactor() = GeometryCount - 1;
    if (OpenGLAbstractLightGroup::DFactor() < 0) OpenGLAbstractLightGroup::DFactor() = DistributionCount - 1;
    if (OpenGLAbstractLightGroup::SFactor() < 0) OpenGLAbstractLightGroup::SFactor() = DistributionCount - 1;
  }

  Karma::setTitle(
    KString((
    FToCStr(OpenGLAbstractLightGroup::FFactor()) + "|" +
    GToCStr(OpenGLAbstractLightGroup::GFactor()) + "|" +
    DToCStr(OpenGLAbstractLightGroup::DFactor()) + " => Sample: " + DToCStr(OpenGLAbstractLightGroup::SFactor())).c_str())
  );

  //OpenGLDebugDraw::Screen::drawTexture(KRectF(0.0, 0.0, 1.0f, 1.0f), environment()->direct());
}

void SampleScene::end()
{
  OpenGLScene::end();
}
