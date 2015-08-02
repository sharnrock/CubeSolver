#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "rcube.h"


const static int FACE_SIZE = 99,
    SQUARE_SIZE = FACE_SIZE / CUBE_DIMENSION,
    SCRAMBLE = 18,
    SOLVERS = 4,  // Each solver will run on its own thread
    MOVE_GRAPH_WIDTH = 400;

class QLabel;
class QPixmap;
class QImage;
class CubeSolver;



namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent * event);

public slots:
    void updateCube(RCube);
    void updateCube();
    void updateMoveGraph(QImage);
    void updateProgressGraph(QImage);

private slots:
    void on_but_scramble_clicked();
    void on_but_autosolve_clicked();
    void on_but_reset_clicked();

private:
    QList<QLabel*> face_labels;
    QList<QPixmap*> face_pixmaps;
    RCube *cube;
    CubeSolver* solvers[SOLVERS];


    void fillFaces();
    void fillFace(int);

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
