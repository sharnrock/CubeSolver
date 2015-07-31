#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include <QPixmap>
#include <QDebug>
#include <QPainter>
#include <QTime>
#include "cubesolver.h"
#include "rcube.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    cube( new RCube() ),
    solver(new CubeSolver(this, cube) ),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTime now = QTime::currentTime();
    qsrand(now.msec());

    qRegisterMetaType<RCube>("RCube");
    QObject::connect(solver, SIGNAL(sendCube(RCube)), this, SLOT(updateCube(RCube)));

    // ================================
    // Set up buttons with cube's slots
    // ================================
    // front
    QObject::connect(ui->but_front_ccw, SIGNAL( clicked() ), cube, SLOT( frontCCW() ));
    QObject::connect(ui->but_front_cw, SIGNAL( clicked() ), cube, SLOT( frontCW() ));
    QObject::connect(ui->but_front_ccw, SIGNAL( clicked() ), this, SLOT( updateCube() ));
    QObject::connect(ui->but_front_cw, SIGNAL( clicked() ), this, SLOT( updateCube() ));

    // back
    QObject::connect(ui->but_back_ccw, SIGNAL( clicked() ), cube, SLOT( backCCW() ));
    QObject::connect(ui->but_back_cw, SIGNAL( clicked() ), cube, SLOT( backCW() ));
    QObject::connect(ui->but_back_ccw, SIGNAL( clicked() ), this, SLOT( updateCube() ));
    QObject::connect(ui->but_back_cw, SIGNAL( clicked() ), this, SLOT( updateCube() ));

    // left
    QObject::connect(ui->but_left_ccw, SIGNAL( clicked() ), cube, SLOT( leftCCW() ));
    QObject::connect(ui->but_left_cw, SIGNAL( clicked() ), cube, SLOT( leftCW() ));
    QObject::connect(ui->but_left_ccw, SIGNAL( clicked() ), this, SLOT( updateCube() ));
    QObject::connect(ui->but_left_cw, SIGNAL( clicked() ), this, SLOT( updateCube() ));

    // right
    QObject::connect(ui->but_right_ccw, SIGNAL( clicked() ), cube, SLOT( rightCCW() ));
    QObject::connect(ui->but_right_cw, SIGNAL( clicked() ), cube, SLOT( rightCW() ));
    QObject::connect(ui->but_right_ccw, SIGNAL( clicked() ), this, SLOT( updateCube() ));
    QObject::connect(ui->but_right_cw, SIGNAL( clicked() ), this, SLOT( updateCube() ));

    // top
    QObject::connect(ui->but_top_ccw, SIGNAL( clicked() ), cube, SLOT( topCCW() ));
    QObject::connect(ui->but_top_cw, SIGNAL( clicked() ), cube, SLOT( topCW() ));
    QObject::connect(ui->but_top_ccw, SIGNAL( clicked() ), this, SLOT( updateCube() ));
    QObject::connect(ui->but_top_cw, SIGNAL( clicked() ), this, SLOT( updateCube() ));

    // bottom
    QObject::connect(ui->but_bottom_ccw, SIGNAL( clicked() ), cube, SLOT( bottomCCW() ));
    QObject::connect(ui->but_bottom_cw, SIGNAL( clicked() ), cube, SLOT( bottomCW() ));
    QObject::connect(ui->but_bottom_ccw, SIGNAL( clicked() ), this, SLOT( updateCube() ));
    QObject::connect(ui->but_bottom_cw, SIGNAL( clicked() ), this, SLOT( updateCube() ));


    // ===================================
    // Create pixmaps and labels for faces
    // ===================================
    for (int i = 0; i < FACE_COUNT; i++)
    {
        face_pixmaps.append( new QPixmap(FACE_SIZE,FACE_SIZE) );
    }
    face_labels << ui->front << ui->back << ui->left << ui->right << ui->top << ui->bottom;


    // ===================================
    // Color the faces on the screen
    // ===================================
    fillFaces();
}

void MainWindow::mousePressEvent(QMouseEvent *)
{
    //qDebug() << "updating";
    updateCube();
}


// SLOT: The cube has moved, draw it again
void MainWindow::updateCube(RCube incoming)
{
    *cube = incoming;
    //qDebug() << "updating";
    fillFaces();
}

void MainWindow::updateCube()
{
    //qDebug() << "updating";
    fillFaces();
}

// Color a single face on screen
void MainWindow::fillFace(int face)
{
    QLabel *face_l = face_labels[face];
    QPixmap *face_p = face_pixmaps[face];
    QPainter paintr(face_p);

    RCube::Face current_face = cube->getFace(face);

    for (int y = 0; y < CUBE_DIMENSION; y++)
    {
        for (int x = 0; x < CUBE_DIMENSION; x++)
        {
            QPixmap square(SQUARE_SIZE,SQUARE_SIZE);
            QColor current_color = current_face.getColor(y,x);

            // stick the square's color for this face into the fill
            square.fill(/*somecoolor*/Qt::yellow);
            square.fill(current_color);

            int pos_x = x*SQUARE_SIZE;
            int pos_y = y*SQUARE_SIZE;
            paintr.drawPixmap(pos_x, pos_y, SQUARE_SIZE, SQUARE_SIZE, square);
        }
    }
    face_l->setPixmap(*face_p);
}

// Color all the faces on screen
void MainWindow::fillFaces()
{
    for (int face = 0; face < face_labels.count(); face++)
    {
        fillFace(face);
    }
}

// Destructor
MainWindow::~MainWindow()
{
    delete ui;
    delete cube;
    /* this can come back when it exists
    solver->exit();

    solver->wait();
    delete solver;
    */
}

void MainWindow::on_but_scramble_clicked()
{

    for (int i = 0; i < SCRAMBLE; i++)
    {
        int move = qrand() % 12;
        switch(move)
        {
        case 0:
            ui->but_front_ccw->clicked();
            break;
        case 1:
            ui->but_front_cw->clicked();
            break;
        case 2:
            ui->but_back_ccw->clicked();
            break;
        case 3:
            ui->but_back_cw->clicked();
            break;
        case 4:
            ui->but_left_ccw->clicked();
            break;
        case 5:
            ui->but_left_cw->clicked();
            break;
        case 6:
            ui->but_right_ccw->clicked();
            break;
        case 7:
            ui->but_right_cw->clicked();
            break;
        case 8:
            ui->but_top_ccw->clicked();
            break;
        case 9:
            ui->but_top_cw->clicked();
            break;
        case 10:
            ui->but_bottom_ccw->clicked();
            break;
        case 11:
            ui->but_bottom_cw->clicked();
            break;
        }

    }
}

void MainWindow::on_but_autosolve_clicked()
{
    solver->start();
}

void MainWindow::on_but_reset_clicked()
{
    cube->reset();
    updateCube();
}
