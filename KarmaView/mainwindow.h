#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColor>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_actionOpen_Mesh_triggered();
  void on_materialChanged();
  void on_materialColor_clicked();
  void on_rendererChanged();
  void on_openGLWidget_frameSwapped();
  void on_ssaoActive_clicked(bool checked);
  void on_ssaaChanged();
  void on_ssaoBlur_clicked(bool checked);
  void on_motionBlurChanged();
  void on_motionBlurActive_clicked(bool checked);
  void on_toneMappingChanged();
  void on_tmReset_clicked();
  void on_sceneAmbient_clicked(bool checked);
  void on_sceneFloor_clicked(bool checked);
  void on_sceneFloorChanged();
  void on_floorColor_clicked();
  void on_instanceChanged();
  void on_lightsAnimated_clicked(bool checked);
  void on_lightsChanged();
  void on_sphereLightsAnimated_clicked(bool checked);
  void on_sphereLightsChanged();
  void on_rectLightsAnimated_clicked(bool checked);
  void on_rectLightsChanged();
  void on_about();
  void on_actionFrame_Profiler_triggered(bool checked);
  void on_bvAabb_clicked(bool checked);
  void on_bvObb_clicked(bool checked);
  void on_bvSphereRitters_clicked(bool checked);
  void on_bvSpherePca_clicked(bool checked);
  void on_bvSphereCentroid_clicked(bool checked);
  void on_bvEllipse_clicked(bool checked);
  void on_bvSphereLarssons_clicked(bool checked);
  void on_rotationChanged();

private:
  bool m_initialized;
  Ui::MainWindow *ui;
  QColor m_currentColor;
  QColor m_currentFloorColor;
  void setupSignals();
};

#endif // MAINWINDOW_H
