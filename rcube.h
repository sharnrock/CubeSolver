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

enum Side
{
    top_edge = 0,
    bottom_edge,
    left_edge,
    right_edge
};

enum // Face id
{
    Front = 0,
    Back,
    Left,
    Right,
    Top,
    Bottom,
    FaceCount
};

class RCube : public QObject
{
    Q_OBJECT
public:
    explicit RCube(QObject *parent = 0);
    RCube(const RCube&);
    RCube& operator=(const RCube&);

    class Face
    {
        friend class RCube;
    private:
        Color squares[CUBE_DIMENSION][CUBE_DIMENSION];
    public:
        bool isSolved();
        void rotateCW();
        void rotateCCW();
        QColor getColor(int y, int x);
        void getSide(QList<Color>&, Side);
        void setSide(const QList<Color>&, Side);
        Face();
        Face(const Face&);
        Face& operator=(const Face&);
    };

private:
    QList<Face> faces;
    static int doubleToIntRounder(double d);
    void rotateEdges(QList<Face*>, QList<Side>);

signals:

public slots:
    void reset();

    // Moves
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
    const Face getFace(int) const;

};

#endif // RCUBE_H
