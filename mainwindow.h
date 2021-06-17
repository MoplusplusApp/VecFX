#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <SVGObject.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString currentFile="";
    NatronSVGObject *mainSVGObject;
    bool svgOpened=false;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_SVG_triggered();
    void cellChangedTable(int i, int j);
    void render();
    void on_actionOpen_Keyframe_File_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
