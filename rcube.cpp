#include "rcube.h"
#include <QDebug>
#include <QColor>
#include <math.h>

// Use random colors while constructing the faces
//  for testing the cube's movements...

//#define RANDOM_COLOR
#ifdef RANDOM_COLOR
#include <stdlib.h>
#endif


RCube::RCube(QObject *parent) : QObject(parent)
{
    for (int i = 0; i < FACE_COUNT; i++)
        faces.append(Face());
}

RCube::RCube(const RCube& in)
{
    (*this) = in;
}
RCube& RCube::operator=(const RCube& in)
{
//    // manual resize to avoid a crash
//    for (int i = 0; i < FaceCount; i++)
//        this->faces.append(NULL);

    this->faces = in.faces;

//    this->faces[Front] = in.getFace(Front);
//    this->faces[Back] = in.getFace(Back);
//    this->faces[Left] = in.getFace(Left);
//    this->faces[Right] = in.getFace(Right);
//    this->faces[Top] = in.getFace(Top);
//    this->faces[Bottom] = in.getFace(Bottom);
    return *this;
}


// ==============
// Cube Movements
// ==============

/* FACE ORIENTATION GUIDE
 *
 * The origin faces for the cube are oriented as follows...
 * (bottom edge of this face meets ->) : (Adjacent face)
 * SO Top:Front MEANS means if you're looking at the Top face, the bottom edge will be the Front face
 * the top edge would be the Back face, the left edge would be the Left face and right edge would be Right face
 * Square[0][0] would be at the bottom left square if you were to orient the cube facing you with the bottom edge
 * at the bottom.  Square[2][2] would be at the top right for a 3x3 cube.  Keep in mind the square represents a matrix
 * so cartesian coords will be square[y][x].
 *
 * Front:Bottom
 * Top:Front
 * Right:Front
 * Left:Front
 * Bottom:Front
 * Back:Bottom
 *
 * Inside of the functions there is a little helper too that goes...
 * Adjacent Face Name : Edge Of That Face That Needs To Rotate
 * Face:edge   Capstart;lowercase
 * So, when you're doing your rotations the <edge> squares of <Face> should be the ones changing
 */

void RCube::frontCCW()
{
    faces[Front].rotateCCW();

    // Adjacents include:
    // Top:bottom; Bottom:bottom; Left:bottom; Right:bottom
    QList<Face*> rotate_faces;
    QList<Side> sides;
    rotate_faces.append(&faces[Right]);  sides.append(bottom_edge);
    rotate_faces.append(&faces[Bottom]); sides.append(bottom_edge);
    rotate_faces.append(&faces[Left]);   sides.append(bottom_edge);
    rotate_faces.append(&faces[Top]);    sides.append(bottom_edge);
    rotateEdges(rotate_faces, sides);
}
void RCube::frontCW()
{
    faces[Front].rotateCW();

    // Adjacents include:
    // Top:bottom; Bottom:bottom; Left:bottom; Right:bottom
    QList<Face*> rotate_faces;
    QList<Side> sides;
    rotate_faces.append(&faces[Top]);    sides.append(bottom_edge);
    rotate_faces.append(&faces[Left]);   sides.append(bottom_edge);
    rotate_faces.append(&faces[Bottom]); sides.append(bottom_edge);
    rotate_faces.append(&faces[Right]);  sides.append(bottom_edge);
    rotateEdges(rotate_faces, sides);
}

void RCube::backCCW()
{
    faces[Back].rotateCCW();
    // Adjacents include:
    // Top:top; Bottom:top; Left:top; Right:top
    QList<Face*> rotate_faces;
    QList<Side> sides;
    rotate_faces.append(&faces[Top]);    sides.append(top_edge);
    rotate_faces.append(&faces[Left]);   sides.append(top_edge);
    rotate_faces.append(&faces[Bottom]); sides.append(top_edge);
    rotate_faces.append(&faces[Right]);  sides.append(top_edge);
    rotateEdges(rotate_faces, sides);
}

void RCube::backCW()
{
    faces[Back].rotateCW();
    // Adjacents include:
    // Top:top; Bottom:top; Left:top; Right:top
    QList<Face*> rotate_faces;
    QList<Side> sides;
    rotate_faces.append(&faces[Right]);  sides.append(top_edge);
    rotate_faces.append(&faces[Bottom]); sides.append(top_edge);
    rotate_faces.append(&faces[Left]);   sides.append(top_edge);
    rotate_faces.append(&faces[Top]);    sides.append(top_edge);
    rotateEdges(rotate_faces, sides);
}

void RCube::leftCCW()
{
    faces[Left].rotateCCW();
    // Adjacents include:
    // Top:left; Bottom:right; Back:right; Front:left
    QList<Face*> rotate_faces;
    QList<Side> sides;
    rotate_faces.append(&faces[Front]);   sides.append(left_edge);
    rotate_faces.append(&faces[Bottom]);  sides.append(right_edge);
    rotate_faces.append(&faces[Back]);    sides.append(right_edge);
    rotate_faces.append(&faces[Top]);     sides.append(left_edge);
    rotateEdges(rotate_faces, sides);
}

void RCube::leftCW()
{
    faces[Left].rotateCW();
    // Adjacents include:
    // Top:left; Bottom:right; Back:right; Front:left
    QList<Face*> rotate_faces;
    QList<Side> sides;
    rotate_faces.append(&faces[Top]);     sides.append(left_edge);
    rotate_faces.append(&faces[Back]);    sides.append(right_edge);
    rotate_faces.append(&faces[Bottom]);  sides.append(right_edge);
    rotate_faces.append(&faces[Front]);   sides.append(left_edge);
    rotateEdges(rotate_faces, sides);
}
void RCube::rightCCW()
{
    faces[Right].rotateCCW();
    // Adjacents include:
    // Top:right; Bottom:left; Back:left; Front:right
    QList<Face*> rotate_faces;
    QList<Side> sides;
    rotate_faces.append(&faces[Top]);     sides.append(right_edge);
    rotate_faces.append(&faces[Back]);    sides.append(left_edge);
    rotate_faces.append(&faces[Bottom]);  sides.append(left_edge);
    rotate_faces.append(&faces[Front]);   sides.append(right_edge);
    rotateEdges(rotate_faces, sides);
}

void RCube::rightCW()
{
    faces[Right].rotateCW();
    // Adjacents include:
    // Top:right; Bottom:left; Back:left; Front:right
    QList<Face*> rotate_faces;
    QList<Side> sides;
    rotate_faces.append(&faces[Front]);   sides.append(right_edge);
    rotate_faces.append(&faces[Bottom]);  sides.append(left_edge);
    rotate_faces.append(&faces[Back]);    sides.append(left_edge);
    rotate_faces.append(&faces[Top]);     sides.append(right_edge);
    rotateEdges(rotate_faces, sides);
}

void RCube::topCCW()
{
    faces[Top].rotateCCW();
    // Adjacents include:
    // Left:right; Right:left; Back:top; Front:top
    QList<Face*> rotate_faces;
    QList<Side> sides;
    rotate_faces.append(&faces[Right]); sides.append(left_edge);
    rotate_faces.append(&faces[Front]); sides.append(top_edge);
    rotate_faces.append(&faces[Left]);  sides.append(right_edge);
    rotate_faces.append(&faces[Back]);  sides.append(top_edge);
    rotateEdges(rotate_faces, sides);
}

void RCube::topCW()
{
    faces[Top].rotateCW();
    // Adjacents include:
    // Left:right; Right:left; Back:top; Front:top
    QList<Face*> rotate_faces;
    QList<Side> sides;
    rotate_faces.append(&faces[Back]);  sides.append(top_edge);
    rotate_faces.append(&faces[Left]);  sides.append(right_edge);
    rotate_faces.append(&faces[Front]); sides.append(top_edge);
    rotate_faces.append(&faces[Right]); sides.append(left_edge);
    rotateEdges(rotate_faces, sides);
}

void RCube::bottomCCW()
{
    faces[Bottom].rotateCCW();
    // Adjacents include:
    // Left:left; Right:right; Back:bottom; Front:bottom
    QList<Face*> rotate_faces;
    QList<Side> sides;
    rotate_faces.append(&faces[Back]);  sides.append(bottom_edge);
    rotate_faces.append(&faces[Left]);  sides.append(left_edge);
    rotate_faces.append(&faces[Front]); sides.append(bottom_edge);
    rotate_faces.append(&faces[Right]); sides.append(right_edge);
    rotateEdges(rotate_faces, sides);
}

void RCube::bottomCW()
{
    faces[Bottom].rotateCW();
    // Adjacents include:
    // Left:left; Right:right; Back:bottom; Front:bottom
    QList<Face*> rotate_faces;
    QList<Side> sides;
    rotate_faces.append(&faces[Right]); sides.append(right_edge);
    rotate_faces.append(&faces[Front]); sides.append(bottom_edge);
    rotate_faces.append(&faces[Left]);  sides.append(left_edge);
    rotate_faces.append(&faces[Back]);  sides.append(bottom_edge);
    rotateEdges(rotate_faces, sides);
}


void RCube::rotateEdges(QList<Face*> faces, QList<Side> sides)
{
    // Grab the first edge and stick it in the hold
    QList<Color> hold, pass;
    faces[0]->getSide(hold, sides.at(0));

    // Shift three sides
    faces[1]->getSide(pass, sides.at(1));
    faces[0]->setSide(pass, sides.at(0));
    pass.clear();

    faces[2]->getSide(pass, sides.at(2));
    faces[1]->setSide(pass, sides.at(1));
    pass.clear();

    faces[3]->getSide(pass, sides.at(3));
    faces[2]->setSide(pass, sides.at(2));

    // Stick the hold in the last open spot
    faces[3]->setSide(hold, sides.at(3));
}



void RCube::reset()
{
    for (int i = 0; i < faces.count(); i++)
    {
        faces[i] = Face();
    }
}



// ==========
// Face Class
// ==========

void RCube::Face::getSide(QList<Color>& list, Side side)
{
    switch(side)
    {
    case top_edge:
        for (int i = 0; i < CUBE_DIMENSION; i++)
            list.append( squares[CUBE_DIMENSION-1][i] );
        return;
    case bottom_edge:
        for (int i = 0; i < CUBE_DIMENSION; i++)
            list.append( squares[0][i] );
        return;
    case left_edge:
        for (int i = 0; i < CUBE_DIMENSION; i++)
            list.append( squares[i][0] );
        return;
    case right_edge:
        for (int i = 0; i < CUBE_DIMENSION; i++)
            list.append( squares[i][CUBE_DIMENSION-1] );
        return;
    }
}

void RCube::Face::setSide(const QList<Color>& list, Side side)
{
    switch(side)
    {
    case top_edge:
        for (int i = 0; i < CUBE_DIMENSION; i++)
            squares[CUBE_DIMENSION-1][i] = list.at(i);
        return;
    case bottom_edge:
        for (int i = 0; i < CUBE_DIMENSION; i++)
            squares[0][i] = list.at(i);
        return;
    case left_edge:
        for (int i = 0; i < CUBE_DIMENSION; i++)
            squares[i][0] = list.at(i);
        return;
    case right_edge:
        for (int i = 0; i < CUBE_DIMENSION; i++)
            squares[i][CUBE_DIMENSION-1] = list.at(i);
        return;
    }
}

// QColor getter for squares
QColor RCube::Face::getColor(int y, int x)
{
    /* Returns the color of a given square
     * The coordinates come in y then x due it being a matrix */

    // There might be a more automated way to do this... I'll look into it later
    switch(squares[y][x])
    {
    case White:
        return Qt::white;
    case Blue:
        return Qt::blue;
    case Orange:
        return QColor(255,140,0);
    case Red:
        return Qt::red;
    case Green:
        return Qt::green;
    case Yellow:
        return Qt::yellow;
    default:
        qDebug() << "color not handled" << __FILE__ << __LINE__;
        return QColor(0,0,0);
    }
}

// Face getter
RCube::Face RCube::getFace(int face_i)
{
    return faces[face_i];
}

const RCube::Face RCube::getFace(int face_i) const
{
    return faces.at(face_i);
}


// Face constructor
RCube::Face::Face()
{
#ifndef RANDOM_COLOR
    static int color_itt = 0;
    for (int x = 0; x < CUBE_DIMENSION; x++)
        for (int y = 0; y < CUBE_DIMENSION; y++)
        {
            Color color = static_cast<Color>(color_itt);
            this->squares[y][x] = color;
        }
    color_itt++;
    if (color_itt > 5)
        color_itt = 0;
#else
    for (int x = 0; x < CUBE_DIMENSION; x++)
        for (int y = 0; y < CUBE_DIMENSION; y++)
        {
            int color_itt = qrand() % 6;
            Color color = static_cast<Color>(color_itt);
            this->squares[y][x] = color;
        }
#endif
}

void RCube::Face::rotateCCW()
{
    //qDebug() << "ccw triggered";
    // To rotate this thing we're going to use some trig
    // First we're going to set the middle sqaure at the origin of the coordinate system
    // Calculate offset


    // this will make it easier to do the reverse direction
    double rotate = -1*PI/2; // radians

    int origin_offset = CUBE_DIMENSION / 2;

    Face face_result = *this;
    for (int x = 0; x < CUBE_DIMENSION; x++)
    {
        for (int y = 0; y < CUBE_DIMENSION; y++)
        {
            // Calculate the theta (or angle) of the coordinate from 0 degrees
            // Find the vector length (hypotenuse)
            // Rotate a unit vector 90 degrees in the direction we want to go
            //  then scale it using the hypotenuse

            // Get new virtual positions
            double vir_x = x-origin_offset;
            double vir_y = y-origin_offset;
            // Hypotenuse & angle
            double length = sqrt(vir_x*vir_x + vir_y*vir_y);
            double theta = atan2(vir_y, vir_x);

            // Rotate and maintain scale
            // the 0.5 forces the int to round; otherwise it just truncates
            double d_new_x = ( length*cos(theta+rotate));
            double d_new_y = ( length*sin(theta+rotate));

            int new_x = doubleToIntRounder(d_new_x);// Give C a little help rounding..
            int new_y = doubleToIntRounder(d_new_y);

            // Stick the color into the resultant square
            face_result.squares[new_y+origin_offset][new_x+origin_offset] = this->squares[y][x];
        }
    }
    *this = face_result;
}

bool RCube::Face::isSolved()
{
    Color test = squares[0][0];
    for (int y = 0; y < CUBE_DIMENSION; y++)
        for (int x = 0; x < CUBE_DIMENSION; x++)
            if (test != squares[y][x])
                return false;

    return true;
}

void RCube::Face::rotateCW()
{
    //qDebug() << "cw triggered";
    // To rotate this thing we're going to use some trig
    // First we're going to set the middle sqaure at the origin of the coordinate system
    // Calculate offset


    // this will make it easier to do the reverse direction
    double rotate = PI/2; // radians

    int origin_offset = CUBE_DIMENSION / 2;

    Face face_result = *this;
    for (int x = 0; x < CUBE_DIMENSION; x++)
    {
        for (int y = 0; y < CUBE_DIMENSION; y++)
        {
            // Calculate the theta (or angle) of the coordinate from 0 degrees
            // Find the vector length (hypotenuse)
            // Rotate a unit vector 90 degrees in the direction we want to go
            //  then scale it using the hypotenuse

            // Get new virtual positions
            double vir_x = x-origin_offset;
            double vir_y = y-origin_offset;
            // Hypotenuse & angle
            double length = sqrt(vir_x*vir_x + vir_y*vir_y);
            double theta = atan2(vir_y, vir_x);

            // Rotate and maintain scale
            // the 0.5 forces the int to round; otherwise it just truncates
            double d_new_x = ( length*cos(theta+rotate));
            double d_new_y = ( length*sin(theta+rotate));

            int new_x = doubleToIntRounder(d_new_x);// Give C a little help rounding..
            int new_y = doubleToIntRounder(d_new_y);

            // Stick the color into the resultant square
            face_result.squares[new_y+origin_offset][new_x+origin_offset] = this->squares[y][x];
        }
    }
    *this = face_result;
}

// helper
int RCube::doubleToIntRounder(double d)
{
    if (d > 0)
        d += 0.5;
    else
        d -= 0.5;
    return (int)d;
}

// Face copy constructor
RCube::Face::Face(const Face& face)
{
    *this = face;
}

// Face assignment operator overload
RCube::Face& RCube::Face::operator=(const RCube::Face& face)
{
    for (int x = 0; x < CUBE_DIMENSION; x++)
    {
        for (int y = 0; y < CUBE_DIMENSION; y++)
        {
            this->squares[y][x] = face.squares[y][x];
        }
    }

    return *this;
}


