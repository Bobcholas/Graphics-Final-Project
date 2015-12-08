#include "settings.h"
#include <QFile>
#include <QSettings>

Settings settings;


/**
  Loads the application settings, or, if no saved settings are available,
  loads default values for the settings. You can change the defaults here.
**/
void Settings::loadSettingsOrDefaults()
{
    // Set the default values below
    QSettings s;
    shaderProgram = s.value("fillMode", SOLID_SHADER_PROGRAM).toInt();
}

void Settings::saveSettings()
{
    QSettings s;
    s.setValue("shape", shaderProgram);
}
