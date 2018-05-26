#include "samplescene.h"

// Standard Template Library
#include <vector>
#include <time.h>

// Qt Framework
#include <QMutex>

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
#include <OpenGLSphereLight>
#include <OpenGLSphereLightGroup>
#include <OpenGLRectangleLight>
#include <OpenGLRectangleLightGroup>

struct LightInfo
{
  float m_lightHeight;
  float m_lightRadius;
  float m_lightIntensity;
  bool m_lightsAnimated;
  int m_lightCount;
  int m_lightBaseTemp;
  int m_lightStepTemp;
};

class SampleScenePrivate
{
public:
  float m_rectLightsWidth, m_rectLightsHeight;
  KVector3D m_objectRotation;
  bool m_bvAabb, m_bvObb, m_bvSphereRitters, m_bvSpherePca, m_bvSphereCentroid, m_bvEllipse, m_bvSphereLarssons;
  LightInfo m_spotlights;
  LightInfo m_sphereLights;
  LightInfo m_rectLights;
  float m_sphereLightRadius;
  float m_metalSeparation;
  float m_roughSeparation;
  float m_metallic;
  float m_roughness;
  KVector3D m_baseColor;
  int m_activeMetals;
  int m_activeRoughness;
  OpenGLInstance *m_floorInstance;
  OpenGLInstance *m_mainInstance;
  KCamera3D m_camera;
  OpenGLViewport m_viewport;
  std::vector<std::vector<OpenGLInstance *>> m_instances;
  std::array<KStaticGeometry, 4> m_staticGeometry;
  KAdaptiveOctree m_octree;
  KBspTree m_bspTree;
  bool m_openModel;
  QMutex m_openLock;

  KAabbBoundingVolume *m_aabb;
  KSphereBoundingVolume *m_sphereCentroid;
  KSphereBoundingVolume *m_sphereLarsons;
  KSphereBoundingVolume *m_spherePca;
  KSphereBoundingVolume *m_sphereRitters;
  KOrientedBoundingVolume *m_obb;
  KEllipsoidBoundingVolume *m_ellipse;

  SampleScenePrivate();

  // Object Manipulation
  void loadObj(const char *fileName);
  void loadObj(const KString &fileName);

  template <typename T>
  void buildMethod(T &geom, KHalfEdgeMesh const &mesh, typename T::BuildMethod method, typename T::TerminationPred pred);
};

SampleScenePrivate::SampleScenePrivate() :
  m_rectLightsWidth(0.0f), m_rectLightsHeight(0.0f),
  m_bvAabb(false), m_bvObb(false), m_bvSphereRitters(false),
  m_bvSpherePca(false), m_bvSphereCentroid(false), m_bvEllipse(false), m_bvSphereLarssons(false),
  m_sphereLightRadius(0.0f),
  m_metalSeparation(0.0f),
  m_roughSeparation(0.0f),
  m_metallic(0.0f),
  m_roughness(0.0f),
  m_activeMetals(0),
  m_activeRoughness(0),
  m_floorInstance(nullptr),
  m_mainInstance(nullptr),
  m_openModel(false),
  m_aabb(0),
  m_sphereCentroid(0),
  m_sphereLarsons(0),
  m_spherePca(0),
  m_sphereRitters(0),
  m_obb(0),
  m_ellipse(0)
{
  // Intentionally Empty
}

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
      delete m_aabb;
      delete m_sphereCentroid;
      delete m_sphereLarsons;
      delete m_spherePca;
      delete m_sphereRitters;
      delete m_obb;
      delete m_ellipse;
      m_aabb = new KAabbBoundingVolume(halfEdgeMesh, KAabbBoundingVolume::MinMaxMethod);
      m_sphereCentroid = new KSphereBoundingVolume(halfEdgeMesh, KSphereBoundingVolume::CentroidMethod);
      m_sphereLarsons = new KSphereBoundingVolume(halfEdgeMesh, KSphereBoundingVolume::LarssonsMethod);
      m_spherePca = new KSphereBoundingVolume(halfEdgeMesh, KSphereBoundingVolume::PcaMethod);
      m_sphereRitters = new KSphereBoundingVolume(halfEdgeMesh, KSphereBoundingVolume::RittersMethod);
      m_obb = new KOrientedBoundingVolume(halfEdgeMesh, KOrientedBoundingVolume::PcaMethod);
      m_ellipse = new KEllipsoidBoundingVolume(halfEdgeMesh, KEllipsoidBoundingVolume::PcaMethod);
      ms = timer.elapsed();
      kDebug() << "Bounding Volume Gen. (sec)   :" << float(ms) / 1e3f;
    }
    kDebug() << "--------------------------------------";
    kDebug() << "Mesh Vertexes  :" << halfEdgeMesh.numVertices();
    kDebug() << "Mesh Faces     :" << halfEdgeMesh.numFaces();
    kDebug() << "Mesh HalfEdges :" << halfEdgeMesh.numHalfEdges();
    kDebug() << "Boundary Edges :" << boundaries;
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
  p.m_camera.setTranslation(0.0f, 0.0f, 25.0f);
  p.m_viewport.create();
  p.m_viewport.setCamera(p.m_camera);
  p.m_viewport.activate();

  // Initialize the Spot Light Group
  static const int LightsCount = 10;
  for (int i = 0; i < LightsCount; ++i)
  {
    createSphereLight();
    createRectangleLight();
    OpenGLSpotLight *light = createSpotLight();
    light->setAttenuation(1.0f, 0.5f, 0.1f);
    light->setShadowCasting(true);
    light->setInnerAngle(0.0f);
    light->setOuterAngle(45.0f);
    light->setDepth(45.0f);
  }

  // Create Floor mesh
  KHalfEdgeMesh floorMesh;
  OpenGLMesh floorMeshGL;
  floorMesh.create(":/resources/objects/floor.obj");
  floorMesh.calculateVertexNormals();
  floorMeshGL.create(floorMesh);
  OpenGLMeshManager::setMesh("Floor", floorMeshGL);

  // Note: Currently there is no Material System.
  //       All material properties are per-instance.
  OpenGLMaterial floorMaterial;
  floorMaterial.create();
  p.m_floorInstance = createInstance();
  p.m_floorInstance->setMesh(floorMeshGL);
  p.m_floorInstance->setMaterial(floorMaterial);
  p.m_floorInstance->transform().setScale(1000.0f);
  p.m_floorInstance->transform().setTranslation(0.0f, -5.0f, 0.0f);
  p.m_floorInstance->setVisible(false);

  // Create instance data
  static const int MetallicCount = 20;
  static const int RoughnessCount = 20;
  p.m_instances.resize(MetallicCount + 1);
  for (int i = 0; i <= MetallicCount; ++i)
  {
    for (int j = 0; j <= RoughnessCount; ++j)
    {
      OpenGLMaterial material;
      material.create();
      OpenGLInstance *instance = createInstance();
      instance->setMaterial(material);
      instance->currentTransform().setScale(5.0f);
      p.m_instances[i].push_back(instance);
    }
  }

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
  env->setDirect(":/resources/images/AlexsApt.hdr");
  env->setIndirect(":/resources/images/AlexsApt_Env.hdr");
}

void SampleScene::update(OpenGLUpdateEvent *event)
{
  P(SampleScenePrivate);
  (void)event;

  // IMPORTANT:
  // In original implementation, QFileDialog blocked and didn't allow update
  // to be called again until the OpenGLWidget::openFileName method returned.
  // However, this doesn't seem to be the case any more, and update is still
  // being called (Qt bug?)
  if (p.m_openLock.tryLock())
  {
    if (p.m_openModel)
    {
      QString fileName = OpenGLWidget::openFileName("Open Model", ".", "Wavefront Object File (*)");
      OpenGLWidget::sMakeCurrent();
      if (!fileName.isNull())
      {
        OpenGLContext::currentContext();
        p.loadObj(fileName);
      }
      p.m_openModel = false;
    }
    p.m_openLock.unlock();
  }

  // Update Lights (Scene update)
  float angle;
  static float f_spotlight = 0.0f;
  static float f_spherelight = 0.0f;
  static float f_rectlight = 0.0f;
  if (p.m_spotlights.m_lightsAnimated) f_spotlight += 0.016f;
  if (p.m_sphereLights.m_lightsAnimated) f_spherelight += 0.016f;
  if (p.m_rectLights.m_lightsAnimated) f_rectlight += 0.016f;

  // Update Spotlights
  angle = f_spotlight;
  for (OpenGLSpotLightGroup::SizeType light = 0; light < spotLights().size(); ++light)
  {
    OpenGLSpotLight *instance = spotLights()[light];
    if (light >= static_cast<unsigned>(p.m_spotlights.m_lightCount))
    {
      instance->setActive(false);
      continue;
    }
    instance->setActive(true);
    instance->setTranslation(cos(angle) * p.m_spotlights.m_lightRadius, p.m_spotlights.m_lightHeight, sin(angle) * p.m_spotlights.m_lightRadius);
    instance->setDirection(-instance->translation().normalized());
    instance->setDiffuse(p.m_spotlights.m_lightIntensity * Karma::k2rgb(p.m_spotlights.m_lightBaseTemp + p.m_spotlights.m_lightStepTemp * light));
    angle += 2 * 3.1415926 / p.m_spotlights.m_lightCount;
  }

  // Update Spherelights
  angle = f_spherelight;
  for (OpenGLSphereLightGroup::SizeType light = 0; light < sphereLights().size(); ++light)
  {
    OpenGLSphereLight *instance = sphereLights()[light];
    if (light >= static_cast<unsigned>(p.m_sphereLights.m_lightCount))
    {
      instance->setActive(false);
      continue;
    }
    instance->setActive(true);
    instance->setTranslation(cos(angle) * p.m_sphereLights.m_lightRadius, p.m_sphereLights.m_lightHeight, sin(angle) * p.m_sphereLights.m_lightRadius);
    instance->setDirection(-instance->translation().normalized());
    instance->setIntensity(p.m_sphereLights.m_lightIntensity);
    instance->setRadius(p.m_sphereLightRadius);
    instance->setTemperature(p.m_sphereLights.m_lightBaseTemp + p.m_sphereLights.m_lightStepTemp * light);
    angle += 2 * 3.1415926 / p.m_sphereLights.m_lightCount;
  }

  // Update Rectlights
  angle = f_rectlight;
  for (OpenGLRectangleLightGroup::SizeType light = 0; light < rectangleLights().size(); ++light)
  {
    OpenGLRectangleLight *instance = rectangleLights()[light];
    if (light >= static_cast<unsigned>(p.m_rectLights.m_lightCount))
    {
      instance->setActive(false);
      continue;
    }
    instance->setActive(true);
    instance->setTranslation(cos(angle) * p.m_rectLights.m_lightRadius, p.m_rectLights.m_lightHeight, sin(angle) * p.m_rectLights.m_lightRadius);
    instance->setDirection(-instance->translation().normalized());
    instance->setIntensity(p.m_rectLights.m_lightIntensity);
    instance->setDimensions(p.m_rectLightsWidth, p.m_rectLightsHeight);
    instance->setTemperature(p.m_rectLights.m_lightBaseTemp + p.m_rectLights.m_lightStepTemp * light);
    angle += 2 * 3.1415926 / p.m_rectLights.m_lightCount;
  }

  OpenGLInstance *instance;
  size_t layerCount = p.m_instances.size();
  for (size_t metal = 0; metal < layerCount; ++metal)
  {
    size_t objectCount = p.m_instances[metal].size();
    for (size_t rough = 0; rough < objectCount; ++rough)
    {
      instance = p.m_instances[metal][rough];

      // Everyone should rotate to keep in sync
      instance->currentTransform().rotate(p.m_objectRotation.x(), 1.0f, 0.0f, 0.0f);
      instance->currentTransform().rotate(p.m_objectRotation.y(), 0.0f, 1.0f, 0.0f);
      instance->currentTransform().rotate(p.m_objectRotation.z(), 0.0f, 0.0f, 1.0f);

      if (rough >= static_cast<unsigned>(p.m_activeRoughness) || metal >= static_cast<unsigned>(p.m_activeMetals))
      {
        instance->setVisible(false);
        continue;
      }

      instance->setVisible(true);
      const float RoughSep = p.m_roughSeparation * p.m_activeRoughness;
      const float MetalSep = p.m_metalSeparation * p.m_activeMetals;

      instance->material().setBaseColor(p.m_baseColor);
      if (p.m_activeMetals == 1)
        instance->material().setMetallic(p.m_metallic);
      else
        instance->material().setMetallic(float(metal + 1) / p.m_activeMetals);
      if (p.m_activeRoughness == 1)
        instance->material().setRoughness(p.m_roughness);
      else
        instance->material().setRoughness(float(rough + 1) / p.m_activeRoughness);

      float xSep = 0.0f;
      float ySep = 0.0f;
      if (p.m_activeRoughness > 1) xSep = (-0.5 + float(rough) / (p.m_activeRoughness - 1)) * RoughSep;
      if (p.m_activeMetals > 1)  ySep = (-0.5 + float(metal) / (p.m_activeMetals - 1)) * MetalSep;
      instance->currentTransform().setTranslation(xSep, 0.0f, ySep);

      if (p.m_bvAabb) p.m_aabb->draw(instance->currentTransform(), Qt::red);
      if (p.m_bvObb) p.m_obb->draw(instance->currentTransform(), Qt::red);
      if (p.m_bvEllipse) p.m_ellipse->draw(instance->currentTransform(), Qt::yellow);
      if (p.m_bvSphereCentroid) p.m_sphereCentroid->draw(instance->currentTransform(), Qt::green);
      if (p.m_bvSpherePca) p.m_spherePca->draw(instance->currentTransform(), Qt::green);
      if (p.m_bvSphereRitters) p.m_sphereRitters->draw(instance->currentTransform(), Qt::green);
      if (p.m_bvSphereLarssons) p.m_sphereLarsons->draw(instance->currentTransform(), Qt::green);
    }
  }

  // Camera Selection
  KCamera3D *camera = 0;
  if (KInputManager::buttonPressed(Qt::RightButton))
  {
    camera = &p.m_camera;
  }

  // Camera Transformation
  if (camera)
  {
    float transSpeed = 10.0f;
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
}

void SampleScene::end()
{
  OpenGLScene::end();
}

void SampleScene::openMesh()
{
  P(SampleScenePrivate);
  p.m_openModel = true;
}

void SampleScene::setMaterial(float r, float g, float b, float metal, float rough)
{
  P(SampleScenePrivate);
  p.m_baseColor = KVector3D(r, g, b);
  p.m_metallic = metal;
  p.m_roughness = rough;
}

void SampleScene::setRenderer(int fresnel, int geometric, int dist, int sample)
{
  OpenGLAbstractLightGroup::FFactor() = fresnel;
  OpenGLAbstractLightGroup::GFactor() = geometric;
  OpenGLAbstractLightGroup::DFactor() = dist;
  OpenGLAbstractLightGroup::SFactor() = sample;
}

void SampleScene::setFloorActive(bool f)
{
  P(SampleScenePrivate);
  p.m_floorInstance->setVisible(f);
}

void SampleScene::setFloorMaterial(float r, float g, float b, float metal, float rough)
{
  P(SampleScenePrivate);
  OpenGLMaterial &material = p.m_floorInstance->material();
  material.setBaseColor(r, g, b);
  material.setMetallic(metal);
  material.setRoughness(rough);
}

void SampleScene::setInstance(int metal, int rough, float mSep, float rSep)
{
  P(SampleScenePrivate);
  p.m_metalSeparation = mSep;
  p.m_roughSeparation = rSep;
  p.m_activeMetals = metal;
  p.m_activeRoughness = rough;
}

void SampleScene::setLightsAnimated(bool a)
{
  P(SampleScenePrivate);
  p.m_spotlights.m_lightsAnimated = a;
}

void SampleScene::setLights(int count, int baseTemp, int stepTemp, float intensity, float height, float radius)
{
  P(SampleScenePrivate);
  p.m_spotlights.m_lightCount = count;
  p.m_spotlights.m_lightBaseTemp = baseTemp;
  p.m_spotlights.m_lightStepTemp = stepTemp;
  p.m_spotlights.m_lightIntensity = intensity;
  p.m_spotlights.m_lightHeight = height;
  p.m_spotlights.m_lightRadius = radius;
}

void SampleScene::setSphereLightsAnimated(bool a)
{
  P(SampleScenePrivate);
  p.m_sphereLights.m_lightsAnimated = a;
}

void SampleScene::setSphereLights(int count, int baseTemp, int stepTemp, float intensity, float height, float radius, float vRadius)
{
  P(SampleScenePrivate);
  p.m_sphereLights.m_lightCount = count;
  p.m_sphereLights.m_lightBaseTemp = baseTemp;
  p.m_sphereLights.m_lightStepTemp = stepTemp;
  p.m_sphereLights.m_lightIntensity = intensity;
  p.m_sphereLights.m_lightHeight = height;
  p.m_sphereLights.m_lightRadius = radius;
  p.m_sphereLightRadius = vRadius;
}

void SampleScene::setRectLightsAnimated(bool a)
{
  P(SampleScenePrivate);
  p.m_rectLights.m_lightsAnimated = a;
}

void SampleScene::setRectLights(int count, int baseTemp, int stepTemp, float intensity, float height, float radius, float vWidth, float vHeight)
{
  P(SampleScenePrivate);
  p.m_rectLights.m_lightCount = count;
  p.m_rectLights.m_lightBaseTemp = baseTemp;
  p.m_rectLights.m_lightStepTemp = stepTemp;
  p.m_rectLights.m_lightIntensity = intensity;
  p.m_rectLights.m_lightHeight = height;
  p.m_rectLights.m_lightRadius = radius;
  p.m_rectLightsWidth = vWidth;
  p.m_rectLightsHeight = vHeight;
}

void SampleScene::setBvSphereRitters(bool bv)
{
  P(SampleScenePrivate);
  p.m_bvSphereRitters = bv;
}

void SampleScene::setBvSpherePca(bool bv)
{
  P(SampleScenePrivate);
  p.m_bvSpherePca = bv;
}

void SampleScene::setBvSphereCentroid(bool bv)
{
  P(SampleScenePrivate);
  p.m_bvSphereCentroid = bv;
}

void SampleScene::setBvEllipse(bool bv)
{
  P(SampleScenePrivate);
  p.m_bvEllipse = bv;
}

void SampleScene::setBvSphereLarssons(bool bv)
{
  P(SampleScenePrivate);
  p.m_bvSphereLarssons = bv;
}

void SampleScene::setObjectRotation(float x, float y, float z)
{
  P(SampleScenePrivate);
  p.m_objectRotation = KVector3D(x, y, z);
}

void SampleScene::setBvObb(bool bv)
{
  P(SampleScenePrivate);
  p.m_bvObb = bv;
}

void SampleScene::setBvAabb(bool bv)
{
  P(SampleScenePrivate);
  p.m_bvAabb = bv;
}
