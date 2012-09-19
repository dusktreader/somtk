#pragma once

#include <QImage>
#include <QSize>
#include <QPoint>
#include <QRect>
#include <QVector>

#include "suspects/suspect.h"
#include "histograms/histogram.h"
#include "features/feature.h"

namespace somtk {

class ColorSuspect : public Suspect
{

private:

    QImage _image;

protected:

    virtual void generateFeatures();

public:

    ColorSuspect( QImage image, HistogramGrid gridTemplate );
};

} // namespace somtk
