#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "qimagehsom.h"
#include "qlocal.h"
#include "local.h"


namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QList<QImageHSOM*> muses;
    Ui::MainWindow *ui;
    TrainDialog* tdlg;
    QProgressDialog pgrs;
    QDirModel classifyTreeModel;
    QGraphicsScene classifyThumbScene;
    QGraphicsPixmapItem classifyThumbPixmap;
    void loadClassifyDirTree( QString dirPath );
    bool addMuse( QImageHSOM* muse );
    bool removeMuse( int index );
    void updateMuseTable();

private slots:
    void on_actionRect_tester_triggered();
    void newMuse();
    void openMuses();
    void killMuse();
    void on_classifyDirTreeView_activated(QModelIndex index);
    void on_classifyDirTextBox_editingFinished();
    void on_classifyDirBrowseBtn_clicked();
    void on_testBtn_clicked();
    void on_testDirBrowseBtn_clicked();
};

#endif                                                                                                                  // MAINWINDOW_H
