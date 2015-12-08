#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <assert.h>
#include "databinding.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    settings.loadSettingsOrDefaults();

    QGLFormat qglFormat;
    qglFormat.setVersion(4,0);
    qglFormat.setProfile(QGLFormat::CoreProfile);
    m_ui->setupUi(this);
    QGridLayout *gridLayout = new QGridLayout(m_ui->centralWidget);
    m_glWidget = new GLWidget(qglFormat, this);
    gridLayout->addWidget(m_glWidget, 0, 1);

    dataBind();
}

MainWindow::~MainWindow()
{
    foreach (DataBinding *b, m_bindings)
    {
        delete b;
    }
    delete m_ui;
    delete m_glWidget;
}

void MainWindow::dataBind()
{
#define BIND(b) { DataBinding *_b = (b); m_bindings.push_back(_b); assert(connect(_b, SIGNAL(dataChanged()), this, SLOT(settingsChanged()))); }

    BIND(ChoiceBinding::bindRadioButtons(NUM_SHADER_PROGRAMS, settings.shaderProgram,
                                    m_ui->solidShader,
                                    m_ui->gradientShader,
                                    m_ui->textureShader));

#undef BIND
}

void MainWindow::settingsChanged()
{
    m_glWidget->update();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Save the settings before we quit
    settings.saveSettings();
    QSettings qtSettings("Lab03");
    qtSettings.setValue("geometry", saveGeometry());
    qtSettings.setValue("windowState", saveState());

    QMainWindow::closeEvent(event);
}
