#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMap>


const static int FACE_SIZE = 99,
    SQUARE_SIZE = FACE_SIZE / 3;


// forward declares
class RCube;

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
    void updateCube();

private:
    QList<QLabel*> face_labels;
    QList<QPixmap*> face_pixmaps;
    RCube *cube;


    void fillFaces();
    void fillFace(int);

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
