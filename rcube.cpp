#include "rcube.h"
#include <QDebug>
#include <QColor>
#include <math.h>

// Use random colors while constructing the faces
//  for testing the cube's movements...
#define RANDOM_COLOR
#ifdef RANDOM_COLOR
#include <stdlib.h>
#endif


RCube::RCube(QObject *parent) : QObject(parent)
{
    for (int i = 0; i < FACE_COUNT; i++)
        faces.append(Face());
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
 *
 * Front:Bottom
 * Top:Front
 * Right:Front
 * Left:Front
 * Bottom:Front
 * Back:Bottom
 *
 * This information is very important.  If you don't understand why yet, you will.
 *
 * Inside of the functions there is a little helper too that goes...
 * Adjacent Face Name : Edge Of That Face That Needs To Rotate
 * Face:edge   Capstart;lowercase
 */

void RCube::frontCCW()
{
    faces[Front].rotateCCW();

    // Adjacents include:
    // Top:bottom; Bottom:bottom; Left:bottom; Right:bottom
    Color adj1[CUBE_DIMENSION] ;
}
void RCube::frontCW()
{
    faces[Front].rotateCW();

    // Adjacents include:
    // Top:bottom; Bottom:bottom; Left:bottom; Right:bottom
}

void RCube::backCCW()
{
    faces[Back].rotateCCW();
    // Adjacents include:
    // Top:top; Bottom:top; Left:top; Right:top
}

void RCube::backCW()
{
    faces[Back].rotateCW();
    // Adjacents include:
    // Top:top; Bottom:top; Left:top; Right:top
}

void RCube::leftCCW()
{
    faces[Left].rotateCCW();
    // Adjacents include:
    // Top:left; Bottom:right; Back:right; Front:left
}

void RCube::leftCW()
{
    faces[Left].rotateCW();
    // Adjacents include:
    // Top:left; Bottom:right; Back:right; Front:left
}
void RCube::rightCCW()
{
    faces[Right].rotateCCW();
    // Adjacents include:
    // Top:right; Bottom:left; Back:left; Front:right
}

void RCube::rightCW()
{
    faces[Right].rotateCW();
    // Adjacents include:
    // Top:right; Bottom:left; Back:left; Front:right
}

void RCube::topCCW()
{
    faces[Top].rotateCCW();
    // Adjacents include:
    // Left:right; Right:left; Back:top; Front:top
}

void RCube::topCW()
{
    faces[Top].rotateCW();
    // Adjacents include:
    // Left:right; Right:left; Back:top; Front:top
}

void RCube::bottomCCW()
{
    faces[Bottom].rotateCCW();
    // Adjacents include:
    // Left:left; Right:right; Back:bottom; Front:bottom
}

void RCube::bottomCW()
{
    faces[Bottom].rotateCW();
    // Adjacents include:
    // Left:left; Right:right; Back:bottom; Front:bottom
}






// ==========
// Face Class
// ==========



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
#else
    for (int x = 0; x < CUBE_DIMENSION; x++)
        for (int y = 0; y < CUBE_DIMENSION; y++)
        {
            int color_itt = rand() % 6;
            Color color = static_cast<Color>(color_itt);
            this->squares[y][x] = color;
        }
#endif
}

void RCube::Face::rotateCCW()
{
    qDebug() << "ccw triggered";
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

void RCube::Face::rotateCW()
{
    qDebug() << "cw triggered";
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


