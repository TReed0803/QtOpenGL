#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>
#include <SampleScene>
#include <QColorDialog>
#include <QColor>
#include <OpenGLRenderer>
#include <RenderPasses>
#include <ScreenSpaceAmbientOcclusion>
#include <MotionBlurPass>
#include <ViewportPresentationPass>
#include <EnvironmentPass>
#include "main.h"
#include <QMessageBox>

template <typename T>
static float asFloat(T const &element)
{
  return float(element->value()) / element->maximum();
}

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_currentColor(255, 30, 30),
  m_currentFloorColor(200, 200, 200),
  m_initialized(false)
{
  ui->setupUi(this);
  setupSignals();
  sg_console = ui->console;
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::setupSignals()
{
  // Actions
  connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(on_about()));

  // Material Properties
  connect(ui->materialMetallic, SIGNAL(valueChanged(int)), this, SLOT(on_materialChanged()));
  connect(ui->materialRoughness, SIGNAL(valueChanged(int)), this, SLOT(on_materialChanged()));
  connect(ui->rotX, SIGNAL(valueChanged(double)), this, SLOT(on_rotationChanged()));
  connect(ui->rotY, SIGNAL(valueChanged(double)), this, SLOT(on_rotationChanged()));
  connect(ui->rotZ, SIGNAL(valueChanged(double)), this, SLOT(on_rotationChanged()));

  // Renderer Properties
  connect(ui->renderFresnel, SIGNAL(currentIndexChanged(int)), this, SLOT(on_rendererChanged()));
  connect(ui->renderGeometric, SIGNAL(currentIndexChanged(int)), this, SLOT(on_rendererChanged()));
  connect(ui->renderNormalDist, SIGNAL(currentIndexChanged(int)), this, SLOT(on_rendererChanged()));
  connect(ui->renderSampling, SIGNAL(currentIndexChanged(int)), this, SLOT(on_rendererChanged()));

  // SSAO Properties
  connect(ui->ssaoContrast, SIGNAL(valueChanged(double)), this, SLOT(on_ssaaChanged()));
  connect(ui->ssaoPower, SIGNAL(valueChanged(double)), this, SLOT(on_ssaaChanged()));
  connect(ui->ssaoRadius, SIGNAL(valueChanged(double)), this, SLOT(on_ssaaChanged()));
  connect(ui->ssaoThreshold, SIGNAL(valueChanged(double)), this, SLOT(on_ssaaChanged()));
  connect(ui->ssaoSamples, SIGNAL(valueChanged(int)), this, SLOT(on_ssaaChanged()));

  // Motion Blur Properties
  connect(ui->motionBlurPower, SIGNAL(valueChanged(double)), this, SLOT(on_motionBlurChanged()));
  connect(ui->motionBlurMaxSamples, SIGNAL(valueChanged(int)), this, SLOT(on_motionBlurChanged()));

  // Tone Mapping Properties
  connect(ui->tmA, SIGNAL(valueChanged(double)), this, SLOT(on_toneMappingChanged()));
  connect(ui->tmB, SIGNAL(valueChanged(double)), this, SLOT(on_toneMappingChanged()));
  connect(ui->tmC, SIGNAL(valueChanged(double)), this, SLOT(on_toneMappingChanged()));
  connect(ui->tmD, SIGNAL(valueChanged(double)), this, SLOT(on_toneMappingChanged()));
  connect(ui->tmE, SIGNAL(valueChanged(double)), this, SLOT(on_toneMappingChanged()));
  connect(ui->tmF, SIGNAL(valueChanged(double)), this, SLOT(on_toneMappingChanged()));
  connect(ui->tmW, SIGNAL(valueChanged(double)), this, SLOT(on_toneMappingChanged()));
  connect(ui->tmExposure, SIGNAL(valueChanged(double)), this, SLOT(on_toneMappingChanged()));
  connect(ui->tmExposureBias, SIGNAL(valueChanged(double)), this, SLOT(on_toneMappingChanged()));

  // Floor Properties
  connect(ui->floorMetallic, SIGNAL(valueChanged(int)), this, SLOT(on_sceneFloorChanged()));
  connect(ui->floorRoughness, SIGNAL(valueChanged(int)), this, SLOT(on_sceneFloorChanged()));

  // Instance Properties
  connect(ui->sceneMetals, SIGNAL(valueChanged(int)), this, SLOT(on_instanceChanged()));
  connect(ui->sceneRoughness, SIGNAL(valueChanged(int)), this, SLOT(on_instanceChanged()));
  connect(ui->sceneMetalSep, SIGNAL(valueChanged(double)), this, SLOT(on_instanceChanged()));
  connect(ui->sceneRoughSep, SIGNAL(valueChanged(double)), this, SLOT(on_instanceChanged()));

  // Light Propertiesv
  connect(ui->lightsCount, SIGNAL(valueChanged(int)), this, SLOT(on_lightsChanged()));
  connect(ui->lightsBaseTemp, SIGNAL(valueChanged(int)), this, SLOT(on_lightsChanged()));
  connect(ui->lightsStepTemp, SIGNAL(valueChanged(int)), this, SLOT(on_lightsChanged()));
  connect(ui->lightsIntensity, SIGNAL(valueChanged(int)), this, SLOT(on_lightsChanged()));
  connect(ui->lightsHeight, SIGNAL(valueChanged(double)), this, SLOT(on_lightsChanged()));
  connect(ui->lightsRadius, SIGNAL(valueChanged(double)), this, SLOT(on_lightsChanged()));

  // Sphere Light Properties
  connect(ui->sphereLightsCount, SIGNAL(valueChanged(int)), this, SLOT(on_sphereLightsChanged()));
  connect(ui->sphereLightsBaseTemp, SIGNAL(valueChanged(int)), this, SLOT(on_sphereLightsChanged()));
  connect(ui->sphereLightsStepTemp, SIGNAL(valueChanged(int)), this, SLOT(on_sphereLightsChanged()));
  connect(ui->sphereLightsIntensity, SIGNAL(valueChanged(int)), this, SLOT(on_sphereLightsChanged()));
  connect(ui->sphereLightsHeight, SIGNAL(valueChanged(double)), this, SLOT(on_sphereLightsChanged()));
  connect(ui->sphereLightsRadius, SIGNAL(valueChanged(double)), this, SLOT(on_sphereLightsChanged()));
  connect(ui->sphereLightsVolumeRadius, SIGNAL(valueChanged(double)), this, SLOT(on_sphereLightsChanged()));

  // Rectangle Light Properties
  connect(ui->rectLightsCount, SIGNAL(valueChanged(int)), this, SLOT(on_rectLightsChanged()));
  connect(ui->rectLightsBaseTemp, SIGNAL(valueChanged(int)), this, SLOT(on_rectLightsChanged()));
  connect(ui->rectLightsStepTemp, SIGNAL(valueChanged(int)), this, SLOT(on_rectLightsChanged()));
  connect(ui->rectLightsIntensity, SIGNAL(valueChanged(int)), this, SLOT(on_rectLightsChanged()));
  connect(ui->rectLightsHeight, SIGNAL(valueChanged(double)), this, SLOT(on_rectLightsChanged()));
  connect(ui->rectLightsRadius, SIGNAL(valueChanged(double)), this, SLOT(on_rectLightsChanged()));
  connect(ui->rectLightsWidth, SIGNAL(valueChanged(double)), this, SLOT(on_rectLightsChanged()));
  connect(ui->rectLightsHeight_2, SIGNAL(valueChanged(double)), this, SLOT(on_rectLightsChanged()));
}

#undef P
#define S(name) name &s = *ui->openGLWidget->sampleScene()
#define R(name) name &r = *ui->openGLWidget->renderer()
#define P(name) name &p = static_cast<name&>(*ui->openGLWidget->renderer()->pass(name::passId()))

void MainWindow::on_actionOpen_Mesh_triggered()
{
  S(SampleScene);
  s.openMesh();
}

void MainWindow::on_materialChanged()
{
  S(SampleScene);
  s.setMaterial(
    m_currentColor.redF(),
    m_currentColor.greenF(),
    m_currentColor.blueF(),
    asFloat(ui->materialMetallic),
    asFloat(ui->materialRoughness)
  );
}

void MainWindow::on_materialColor_clicked()
{
  m_currentColor = QColorDialog::getRgba(m_currentColor.rgb());
  on_materialChanged();
}

void MainWindow::on_rendererChanged()
{
  S(SampleScene);
  s.setRenderer(
    ui->renderFresnel->currentIndex(),
    ui->renderGeometric->currentIndex(),
    ui->renderNormalDist->currentIndex(),
    ui->renderSampling->currentIndex()
  );
}

void MainWindow::on_openGLWidget_frameSwapped()
{
  if (!m_initialized && ui->openGLWidget->isReady())
  {
    on_rendererChanged();
    on_materialChanged();
    on_ssaaChanged();
    on_toneMappingChanged();
    on_sceneFloorChanged();
    on_instanceChanged();
    on_lightsChanged();
    on_sphereLightsChanged();
    on_rectLightsChanged();
    m_initialized = true;
  }
}

void MainWindow::on_ssaoActive_clicked(bool checked)
{
  P(ScreenSpaceAmbientOcclusion);
  p.setActive(checked);
}

void MainWindow::on_ssaaChanged()
{
  P(ScreenSpaceAmbientOcclusion);
  p.setContrast(ui->ssaoContrast->value());
  p.setPower(ui->ssaoPower->value());
  p.setRadius(ui->ssaoRadius->value());
  p.setSamples(ui->ssaoSamples->value());
  p.setThreshold(ui->ssaoThreshold->value());
}

void MainWindow::on_motionBlurChanged()
{
  P(MotionBlurPass);
  p.setMaxSamples(ui->motionBlurMaxSamples->value());
  p.setPower(ui->motionBlurPower->value());
}

void MainWindow::on_ssaoBlur_clicked(bool checked)
{
  P(ScreenSpaceAmbientOcclusion);
  p.setBlur(checked);
}

void MainWindow::on_motionBlurActive_clicked(bool checked)
{
  P(MotionBlurPass);
  p.setActive(checked);
}

void MainWindow::on_toneMappingChanged()
{
  P(ViewportPresentationPass);
  p.setValues(ui->tmA->value(), ui->tmB->value(), ui->tmC->value(), ui->tmD->value(), ui->tmE->value(), ui->tmF->value(), ui->tmW->value());
  p.setExposure(ui->tmExposure->value());
  p.setExposureBias(ui->tmExposureBias->value());
}

void MainWindow::on_tmReset_clicked()
{
  ui->tmA->setValue(0.15f);
  ui->tmB->setValue(0.50f);
  ui->tmC->setValue(0.10f);
  ui->tmD->setValue(0.20f);
  ui->tmE->setValue(0.02f);
  ui->tmF->setValue(0.30f);
  ui->tmW->setValue(11.2f);
  ui->tmExposure->setValue(1.0f);
  ui->tmExposureBias->setValue(2.0f);
  on_toneMappingChanged();
}

void MainWindow::on_sceneAmbient_clicked(bool checked)
{
  P(EnvironmentPass);
  p.setActive(checked);
}

void MainWindow::on_sceneFloor_clicked(bool checked)
{
  S(SampleScene);
  s.setFloorActive(checked);
}

void MainWindow::on_sceneFloorChanged()
{
  S(SampleScene);
  s.setFloorMaterial(
    m_currentFloorColor.redF(),
    m_currentFloorColor.greenF(),
    m_currentFloorColor.blueF(),
    asFloat(ui->floorMetallic),
    asFloat(ui->floorRoughness)
  );
}

void MainWindow::on_floorColor_clicked()
{
  m_currentFloorColor = QColorDialog::getRgba(m_currentFloorColor.rgb());
  on_sceneFloorChanged();
}

void MainWindow::on_instanceChanged()
{
  S(SampleScene);
  s.setInstance(ui->sceneMetals->value(), ui->sceneRoughness->value(), ui->sceneMetalSep->value(), ui->sceneRoughSep->value());
}

void MainWindow::on_lightsAnimated_clicked(bool checked)
{
  S(SampleScene);
  s.setLightsAnimated(checked);
}

void MainWindow::on_lightsChanged()
{
  S(SampleScene);
  s.setLights(ui->lightsCount->value(), ui->lightsBaseTemp->value(), ui->lightsStepTemp->value(), ui->lightsIntensity->value(), ui->lightsHeight->value(), ui->lightsRadius->value());
}

void MainWindow::on_sphereLightsAnimated_clicked(bool checked)
{
  S(SampleScene);
  s.setSphereLightsAnimated(checked);
}

void MainWindow::on_sphereLightsChanged()
{
  S(SampleScene);
  s.setSphereLights(ui->sphereLightsCount->value(), ui->sphereLightsBaseTemp->value(), ui->sphereLightsStepTemp->value(), ui->sphereLightsIntensity->value(), ui->sphereLightsHeight->value(), ui->sphereLightsRadius->value(), ui->sphereLightsVolumeRadius->value());
}

void MainWindow::on_rectLightsAnimated_clicked(bool checked)
{
  S(SampleScene);
  s.setRectLightsAnimated(checked);
}

void MainWindow::on_rectLightsChanged()
{
  S(SampleScene);
  s.setRectLights(ui->rectLightsCount->value(), ui->rectLightsBaseTemp->value(), ui->rectLightsStepTemp->value(), ui->rectLightsIntensity->value(), ui->rectLightsHeight->value(), ui->rectLightsRadius->value(), ui->rectLightsWidth->value(), ui->rectLightsHeight_2->value());
}

void MainWindow::on_about()
{
  QMessageBox::about(this, "KarmaView",
    "KarmaView is an educational framework for learning and developing modern graphics technology. "
    "It is meant solely for learning purposes, and does not promise stability or immediate real-world application. "
    "\n\nUsage:\n"
    "Camera Motion: Hold Right-Mouse button to activate the camera.\n"
    "The camera can be controlled via WASD for forward motion, and QE for vertical motion.\n"
    "Camera direction can be controlled by moving the mouse, for finer movement speeds, hold Ctrl while moving."
    "All other actions can be controlled through menu options.\n"
    "\n\nDeveloped by Trent Reed (www.trentreed.net)\n"
    "Special Thanks: Jen Sward, Pushpak Karnick, and Gary Herron.\n"
    "Copyright DigiPen Institute of Technology 2015"
  );
}

void MainWindow::on_actionFrame_Profiler_triggered(bool checked)
{
  ui->openGLWidget->setProfilerVisible(checked);
}

void MainWindow::on_bvAabb_clicked(bool checked)
{
  S(SampleScene);
  s.setBvAabb(checked);
}

void MainWindow::on_bvObb_clicked(bool checked)
{
  S(SampleScene);
  s.setBvObb(checked);
}

void MainWindow::on_bvSphereRitters_clicked(bool checked)
{
  S(SampleScene);
  s.setBvSphereRitters(checked);
}

void MainWindow::on_bvSpherePca_clicked(bool checked)
{
  S(SampleScene);
  s.setBvSpherePca(checked);
}

void MainWindow::on_bvSphereCentroid_clicked(bool checked)
{
  S(SampleScene);
  s.setBvSphereCentroid(checked);
}

void MainWindow::on_bvEllipse_clicked(bool checked)
{
  S(SampleScene);
  s.setBvEllipse(checked);
}

void MainWindow::on_bvSphereLarssons_clicked(bool checked)
{
  S(SampleScene);
  s.setBvSphereLarssons(checked);
}

void MainWindow::on_rotationChanged()
{
  S(SampleScene);
  s.setObjectRotation(ui->rotX->value(), ui->rotY->value(), ui->rotZ->value());
}
