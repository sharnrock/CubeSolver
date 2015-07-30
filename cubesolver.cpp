#include "cubesolver.h"
#include "rcube.h"
#include <QDebug>

CubeSolver::CubeSolver(MainWindow* _window, RCube* _cube) :
    main_window(_window),
    cube(_cube)
{
    // set up connects here
}

CubeSolver::~CubeSolver()
{
    for (int i = 0; i < organisms.length(); i++)
        delete organisms[i];
}

void CubeSolver::run()
{
    genesis();


    for (int i = 0; i < organisms.length(); i++)
    {
        qDebug() << "Moves: " << organisms.at(i)->solveCube();
    }

    qDebug() << "stop";
}

void CubeSolver::genesis()
{
    for (int i = 0; i < STARTING_ORGANISMS; i++)
    {
        organisms.append( new Organism(cube) ); // might need to create a bunch of 'new' to keep them from immediately destructing
        //qDebug() << "appended that organism";
    }
    //qDebug() << "break";
}

void CubeSolver::breed()
{

}

void CubeSolver::scoreFitness()
{

}


CubeSolver::Organism::Organism(RCube* cube) :
    score(0),
    p_cube(new RCube(*cube)) // make a new instance based off the original scrambled cube
{
    // Generate random list of moves
    for (int i = 0; i < STARTING_MOVES; i++)
    {
        int random_move = qrand() % MoveCount;
        moves.append(static_cast<Move>(random_move));
    }
    //qDebug() << "Finished an organism";
}

CubeSolver::Organism::~Organism()
{
    if (p_cube)
        delete p_cube;
}

CubeSolver::Organism::Organism(const CubeSolver::Organism& in)
{
    *this = in;
}

CubeSolver::Organism& CubeSolver::Organism::operator=(const CubeSolver::Organism& in)
{
    this->p_cube = in.p_cube;
    this->moves = in.moves;
    this->score = in.score;
    return (*this);
}

/* Returns the amount of moves taken to solve the cube */
int CubeSolver::Organism::solveCube()
{
    int moved = 0;
    for (; moved < moves.length(); moved++)
    {
        this->move( moves.at(moved) );
        if (this->isSolved())
            break;
    }
    return moved;
}

void CubeSolver::Organism::move(CubeSolver::Move do_move)
{
    switch(do_move)
    {
    case FrontCCW:
        p_cube->frontCCW();
        break;
    case FrontCW:
        p_cube->frontCW();
        break;
    case BackCCW:
        p_cube->backCCW();
        break;
    case BackCW:
        p_cube->backCW();
        break;
    case LeftCCW:
        p_cube->leftCCW();
        break;
    case LeftCW:
        p_cube->leftCW();
        break;
    case RightCCW:
        p_cube->rightCCW();
        break;
    case RightCW:
        p_cube->rightCW();
        break;
    case TopCCW:
        p_cube->topCCW();
        break;
    case TopCW:
        p_cube->topCW();
        break;
    case BottomCCW:
        p_cube->bottomCCW();
        break;
    case BottomCW:
        p_cube->bottomCW();
        break;
    }
}

bool CubeSolver::Organism::isSolved()
{
    for (int i = 0; i < FaceCount; i++)
        if (p_cube->getFace(i).isSolved() ==  false)
            return false;
    return true;
}

