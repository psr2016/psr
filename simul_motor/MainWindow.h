#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QOpenGLWindow>
//#include <QOpenGLFunctions_2_1>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QKeyEvent>
#include <QSettings>
#include "World.h"
#include "Gear.h"
#include "oxill.h"

class MainWindow : public QOpenGLWindow , protected QOpenGLFunctions_2_1
{
    Q_OBJECT

    // OpenGL Events
 public:
    MainWindow(QSettings * settings);
    ~MainWindow();

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void teardownGL();

    void openPlot();

    void keyPressEvent(QKeyEvent* ke);

  private slots:
      void timerTimeout();

 private:
      QSettings * m_settings;
      // Private Helpers
      void printContextInformation();
      Core::World * m_world;
      Gear * m_gear;
      QTimer * timer;
      OmegaPlot * omega_plot;
      TorquePlot * torque_plot;
signals:
    void new_omega_data(double * p);
    void new_torque_data(double * p);
};

#endif // MAIN_WINDOW_H

