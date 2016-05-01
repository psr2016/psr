
#include "MainWindow.h"
#include <QDebug>
#include <QString>
#include "MainWindow_moc.h"
//#include "oxill_moc.h"

#define TIMER_FREQ  500

MainWindow::MainWindow(QSettings * settings)
    : QOpenGLWindow(), QOpenGLFunctions_2_1(), m_settings(settings)
{
    m_world = new Core::World(m_settings);

    timer = new QTimer(this);
    timer->setInterval(1000 / TIMER_FREQ);
    timer->setSingleShot(false);
    connect(timer, &QTimer::timeout, this, &MainWindow::timerTimeout);
}

MainWindow::~MainWindow()
{
    makeCurrent();
    teardownGL();
}

void MainWindow::openPlot()
{
    omega_plot = new OmegaPlot();
    omega_plot->show();
    connect(this, SIGNAL(new_omega_data(double *)), omega_plot, SLOT(on_new_data(double *)));

    torque_plot = new TorquePlot();
    torque_plot->show();
    connect(this, SIGNAL(new_torque_data(double *)), torque_plot, SLOT(on_new_data(double *)));
}


void MainWindow::timerTimeout()
{
    //m_gear->tick();
    m_world->getBulletWorld()->stepSimulation(1.0/TIMER_FREQ,0);
    double d[2] = {0,0};
    d[0] = m_gear->get_omega_current();
    d[1] = m_gear->get_omega_target();
    emit new_omega_data(d);

    d[0] = m_gear->get_torque();
    emit new_torque_data(d);

    update();
}

/*******************************************************************************
 * OpenGL Events
 ******************************************************************************/

void MainWindow::initializeGL()
{
    // Initialize OpenGL Backend
    initializeOpenGLFunctions();
    printContextInformation();
    // Set global information
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    m_gear = new Gear(this, m_world, 1.0/TIMER_FREQ);
    m_gear->init();
}


void MainWindow::resizeGL(int width, int height)
{
    m_gear->reshape(width, height);
}


void MainWindow::paintGL()
{
    // Clear
    //glClear(GL_COLOR_BUFFER_BIT);
    m_gear->draw();
}


void MainWindow::teardownGL()
{
    // Currently we have no data to teardown
}

void MainWindow::keyPressEvent(QKeyEvent* ev)
{
    if (ev->isAutoRepeat())
        return; // ignore

    switch (ev->key())
        {
        case Qt::Key_G:
            timer->start();
            break;
        case Qt::Key_S:
            timer->stop();
            break;
        default:
            break;
        }
}

/*******************************************************************************
 * Private Helpers
 ******************************************************************************/

void MainWindow::printContextInformation()
{
    QString glType;
    QString glVersion;
    QString glProfile;

    // Get Version Information
    glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

    // Get Profile Information
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
    switch (format().profile())
        {
            CASE(NoProfile);
            CASE(CoreProfile);
            CASE(CompatibilityProfile);
        }
#undef CASE

    // qPrintable() will print our QString w/o quotes around it.
    qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}
