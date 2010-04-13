#pragma once

#include <QMainWindow>
#include <QProgressDialog>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include <MegaSlider/megaslider.h>

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

#include "tools.h"
#include "qtools.h"

#include "detector.h"
#include "fasthoughdetector.h"
#include "tophatdetector.h"
#include "cascadedetector.h"
#include "stardetector.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    cv::Mat rgb;
    cv::Mat ycc;
    cv::Mat gry;

    QVector<QGraphicsRectItem*> detectBoxes;


private slots:
    void on_actionGo_triggered();
    void on_actionOpen_Image_triggered();
};

