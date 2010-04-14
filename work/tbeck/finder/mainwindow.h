#pragma once

#include <QMainWindow>
#include <QProgressDialog>
#include <QColorDialog>
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

public slots:
    void setDetectBoxColor( const QColor& color );
    void redrawBoxes();
    void clearBoxes();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    cv::Mat rgb;
    cv::Mat ycc;
    cv::Mat gry;

    QVector<QGraphicsRectItem*> detectBoxes;
    QColor detectBoxColor;

private slots:
    void on_boxColorBtn_clicked();
    void on_actionGo_triggered();
    void on_actionOpen_Image_triggered();
};

