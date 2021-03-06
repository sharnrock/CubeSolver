#ifndef CUBESOLVER_H
#define CUBESOLVER_H

#include <QObject>
#include <QThread>
#include <QList>
#include <QImage>
#include <QReadWriteLock>

#include "rcube.h"



class MainWindow;
class CubeSolver : public QThread
{
    Q_OBJECT

public:
    explicit CubeSolver(MainWindow*, RCube*);
    ~CubeSolver();

// ==============================
// SETTINGS!
// ==============================

// Quickly choose breeding algorithm
//#define BREED1
#define BREED2
#define CUBE_TEST 0

    // Constants
    const static int STARTING_MOVES = 20; // this many random moves are created and used before giving up
    const static int STARTING_ORGANISMS = 200;
    const static int GEN_RUNS = 80000; // generation runs; how many generations before the program quits

    // Replace this amount of moves per mutation;
    const static int MUTATION_AMOUNT = 2; // How many moves in a sequence will mutate
    const static int MUTATION_SELECT_AMOUNT = STARTING_ORGANISMS / 2; // How many organisms will mutate out of the total amount
    const static int BOTTOM_CUTOFF = 10; // Take the last N of the list and just start over with brand new random



    // SCORE CONSTANTS
    // If any of the squares match the color of the center square that is worth 1 point per square (total of 8 points [because -1 for center square])
    const static int SCORE_FACE_COMPLETED = 50; // score multiplier for completing a face (8*this)

    // This number is multiplied against the moves that haven't been used yet;  It gets big VERY quickly
    const static int SCORE_SOLVE_MULTIPLIER = 100; // moves_left*this# * face_completed_score  total maximum score of around 3Million or something...



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
    void sendCube(RCube);
    void sendMoveGraph(QImage);
    void sendProgressGraph(QImage);
public slots:
    // incoming from window?
protected:
    void run();
private:
    RCube *live_cube, *solve_cube;
    RCube tester_cube;
    MainWindow* main_window;

    QReadWriteLock lock;

    QList<Organism*> organisms;
    QImage *moveGraph;
    QImage *progressGraph;

    void drawMoveGraph();
    void drawProgressgraph();
    void genesis();
    void breed();
    QList<Move> copyGenes(const QList<Move>& one, const QList<Move>& two, int left, int right);
    //int scoreFitness(RCube);

};

class CubeSolver::Organism
{
friend class CubeSolver;
public:
    Organism() = delete;
    Organism(RCube*);
    Organism(const Organism&);
    Organism& operator=(const Organism&);
    static bool greaterThan(const Organism* l, const Organism* r);
    ~Organism();

    int solveCube();
private:
    void move(CubeSolver::Move do_move);
    int compute_score();
    RCube* p_cube;
    QList<Move> moves;
    QList<int> top_scores;
    double score;
};


#endif // CUBESOLVER_H
