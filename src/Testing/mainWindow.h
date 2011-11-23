#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QVTKWidget.h>

#include <QtCore>

class MainWindow : public QMainWindow {
    Q_OBJECT;
public:
    MainWindow();
    ~MainWindow();

    void BuildPipeLine();

    void setFilename( const QString & fn );

    void restartRefreshTimer();

protected:
    void createMenus();

protected slots:
    void open();
    void onRefreshTimer();

private:
    class MainWindowPrivate;
    QScopedPointer<MainWindowPrivate> d;
};

#endif // MAINWINDOW_H
