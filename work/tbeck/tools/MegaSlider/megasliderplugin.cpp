#include "megaslider.h"
#include "megasliderplugin.h"

#include <QtCore/QtPlugin>

MegaSliderPlugin::MegaSliderPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void MegaSliderPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool MegaSliderPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *MegaSliderPlugin::createWidget(QWidget *parent)
{
    return new MegaSlider(parent);
}

QString MegaSliderPlugin::name() const
{
    return QLatin1String("MegaSlider");
}

QString MegaSliderPlugin::group() const
{
    return QLatin1String("");
}

QIcon MegaSliderPlugin::icon() const
{
    return QIcon();
}

QString MegaSliderPlugin::toolTip() const
{
    return QLatin1String("");
}

QString MegaSliderPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool MegaSliderPlugin::isContainer() const
{
    return false;
}

QString MegaSliderPlugin::domXml() const
{
    return QLatin1String("<widget class=\"MegaSlider\" name=\"megaSlider\">\n</widget>\n");
}

QString MegaSliderPlugin::includeFile() const
{
    return QLatin1String("megaslider.h");
}

Q_EXPORT_PLUGIN2(megasliderplugin, MegaSliderPlugin)
