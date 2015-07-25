#ifndef RCUBE_H
#define RCUBE_H

#include <QObject>

const static int CUBE_DIMENSION = 3;
const static int FACE_COUNT = 6;
const static double PI = 3.14159;

enum Color
{
    White = 0,
    Blue,
    Orange,
    Red,
    Green,
    Yellow
};

enum
{
    Front = 0,
    Back,
    Left,
    Right,
    Top,
    Bottom
};

class RCube : public QObject
{
    Q_OBJECT
public:
    explicit RCube(QObject *parent = 0);


    class Face
    {
        friend class RCube;
    private:
        Color squares[CUBE_DIMENSION][CUBE_DIMENSION];
    public:
        void rotateCW();
        void rotateCCW();
        QColor getColor(int y, int x);
        Face();
        Face(const Face&);
        Face& operator=(const Face&);
    };


private:
    QList<Face> faces;
    static int doubleToIntRounder(double d);

signals:

public slots:
    void frontCCW();
    void frontCW();
    void backCCW();
    void backCW();
    void leftCCW();
    void leftCW();
    void rightCCW();
    void rightCW();
    void topCCW();
    void topCW();
    void bottomCCW();
    void bottomCW();

public:
    Face getFace(int);

//    Face getFrontFace() { return faces[Front]; }
//    Face getBackFace() { return faces[Back]; }
//    Face getLeftFace() { return faces[Left]; }
//    Face getRightFace() { return faces[Right]; }
//    Face getTopFace() { return faces[Top]; }
//    Face getBottomFace() { return faces[Bottom]; }
};

#endif // RCUBE_H
