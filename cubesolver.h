#ifndef CUBESOLVER_H
#define CUBESOLVER_H

#include <QObject>
#include <QThread>
#include <QList>

//#include "rcube.h"
class RCube;

class MainWindow;

class CubeSolver : public QThread
{
    Q_OBJECT
public:
    explicit CubeSolver(MainWindow*, RCube*);
    ~CubeSolver();

    // Constants
    const static int STARTING_MOVES = 1000;
    const static int STARTING_ORGANISMS = 50;

    // Some defined enums
    enum Move
    {
        FrontCCW,
        FrontCW,
        BackCCW,
        BackCW,
        LeftCCW,
        LeftCW,
        RightCCW,
        RightCW,
        TopCCW,
        TopCW,
        BottomCCW,
        BottomCW,
        MoveCount
    };

    // organism class forward declaration
    class Organism;

signals:
    // probably something to make the screen update
public slots:
    // incoming from window?
protected:
    void run();
private:
    RCube* cube;
    MainWindow* main_window;

    QList<Organism*> organisms;

    void genesis();
    void breed();
    void scoreFitness();

};

class CubeSolver::Organism
{
friend class CubeSolver;
public:
    Organism() = delete;
    Organism(RCube*);
    Organism(const Organism&);
    Organism& operator=(const Organism&);
    ~Organism();

    int solveCube();
private:
    void move(CubeSolver::Move do_move);
    bool isSolved();
    RCube* p_cube;
    QList<Move> moves;
    double score;
};

#endif // CUBESOLVER_H
