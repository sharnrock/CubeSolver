#ifndef CUBESOLVER_H
#define CUBESOLVER_H

#include <QObject>
#include <QThread>
#include <QList>

#include "rcube.h"
//class RCube;

class MainWindow;

class CubeSolver : public QThread
{
    Q_OBJECT

public:
    explicit CubeSolver(MainWindow*, RCube*);
    ~CubeSolver();


    // Constants
    const static int STARTING_MOVES = 1000;
    const static int STARTING_ORGANISMS = 100;
    const static int SCORE_FACE_COMPLETED = 50;
    const static int SCORE_SOLVE_MULTIPLIER = 8000;
    const static int GEN_RUNS = 10000;
    const static int MUTATION_AMOUNT = 100;

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
