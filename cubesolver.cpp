#include "cubesolver.h"
#include "rcube.h"
#include <QDebug>

CubeSolver::CubeSolver(MainWindow* _window, RCube* _cube) :
    main_window(_window),
    //solve_cube(new RCube(_cube)),
    live_cube(_cube)

{
    tester_cube = *live_cube;
}

CubeSolver::~CubeSolver()
{
    for (int i = 0; i < organisms.length(); i++)
        delete organisms[i];
}

void CubeSolver::run()
{
    // Create a new instance of a cube, so we can hold on to whatever scramble comes in

    lock.lockForRead();
    solve_cube = new RCube(*live_cube);
    tester_cube = *live_cube;
    lock.unlock();

    // Create the first of the random moves
    genesis();

    for (int generation = 0; generation < GEN_RUNS; generation++)
    {
        for (int i = 0; i < organisms.length(); i++)
        {
#if CUBE_TEST
            if (*organisms.at(i)->p_cube != tester_cube)
            {
                qDebug() << "Cube was not equal to tester before solving";
                throw;
            }
#endif
            organisms.at(i)->solveCube(); // returns score based on largest score they achieved
#if CUBE_TEST
            if (*organisms.at(i)->p_cube == tester_cube)
            {
                qDebug() << "Cube was equal to tester before solving";
                throw;
            }
#endif
        }
        breed();
        //qDebug() << "Generation: " << generation;
    }
    qDebug() << "stop";
}

void CubeSolver::genesis()
{
    for (int i = 0; i < STARTING_ORGANISMS; i++)
    {
        organisms.append( new Organism(solve_cube) );
    }
}

void CubeSolver::breed()
{
    qSort(organisms.begin(), organisms.end(), CubeSolver::Organism::greaterThan );

    qDebug() << "top score: " << organisms[0]->score;
    emit sendCube(*organisms[0]->p_cube); // display the winning cube of the generation

    int left = 0;
    int right = qrand() % STARTING_MOVES;
    int half_way = STARTING_ORGANISMS / 2;

    // Selective reproduction
    for (int i = 0; i < half_way; i++)
    {
        organisms[half_way+i]->moves = copyGenes(organisms[i]->moves, organisms[half_way-i]->moves, left, right);
        // reset scores
        organisms[i]->score = 0;
        *organisms[i]->p_cube = *solve_cube;
        // reset cube
        organisms[half_way+i]->score = 0;
        *organisms[half_way+i]->p_cube = *solve_cube;


#if CUBE_TEST
        // Cube test
        if (*organisms[i]->p_cube != *organisms[half_way+i]->p_cube ||
                *organisms[half_way+i]->p_cube != tester_cube)
        {
            qDebug() << "cubes don't match original cube";
            throw;
        }
#endif

    }



#if 0
    qSort(organisms.begin(), organisms.end(), CubeSolver::Organism::greaterThan );

    qDebug() << "top score: " << organisms[0]->score;
    emit sendCube(*organisms[0]->p_cube);

    int left = qrand() % STARTING_MOVES;
    int right = qrand() % STARTING_MOVES;
    if (right < left)
    {
        int swap = left;
        left = right;
        right = swap;
    }
    if (left > right)
        throw; //you messed up
    if (left == right && right < STARTING_MOVES)
        right++;
    else if (left == right)
        left--;

    int half_way = STARTING_ORGANISMS / 2;
    //int quarter_way = STARTING_MOVES / 4;

    // Selective reproduction
    for (int i = 0; i < half_way; i++)
    {
        organisms[half_way+i]->moves = copyGenes(organisms[i]->moves, organisms[half_way-i]->moves, left, right);
        // reset scores
        organisms[i]->score = 0;
        *organisms[i]->p_cube = *solve_cube;
        // reset cube
        organisms[half_way+i]->score = 0;
        *organisms[half_way+i]->p_cube = *solve_cube;
    }
#endif

#if 1
    // Random mutations
    for (int mutate = 0; mutate < MUTATION_SELECT_AMOUNT; mutate++)
    {
        int selected_one = qrand() % half_way;
        selected_one += half_way; // randomly select one in the bottom half
        int movecount = static_cast<int>(CubeSolver::MoveCount);
        int nl = qrand() % STARTING_MOVES;
        if (nl < STARTING_MOVES-MUTATION_AMOUNT)
        {
            int nr = nl + MUTATION_AMOUNT-1;  // randomly choose the start of the sequence, then go for mutation_amount until done
            for (int i = nl; i < nr; i++)
            {
                organisms[selected_one]->moves[i] = static_cast<Move>( qrand() % movecount );
            }
        }
    }
#endif
}

QList<CubeSolver::Move> CubeSolver::copyGenes(const QList<CubeSolver::Move>& one, const QList<CubeSolver::Move>& two, int left, int right)
{
    //qDebug() << "copy gene started";
    QList<CubeSolver::Move>result = two;
    for (int i = left; i < right; i++)
    {
        result[i] = one.at(i);
    }
    return result;
}

//int CubeSolver::scoreFitness(RCube& rc)
//{
////    for (int i = 0; i < FACE_COUNT; i++)
////    {
////        Face f = rc.getFace(i);
////        QColor center;
////    }
//    return 0;
//}


CubeSolver::Organism::Organism(RCube* solve_cube) :
    score(0),
    p_cube(new RCube(*solve_cube)) // make a new instance based off the original scrambled cube
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


// Used in sorting organisms based on score
bool CubeSolver::Organism::greaterThan(const Organism *l, const Organism *r)
{
    return l->score > r->score;
}



/* Returns the amount of moves taken to solve the cube */
int CubeSolver::Organism::solveCube()
{

    int SOLVED_SCORE = FACE_COUNT*SCORE_FACE_COMPLETED;
    int moved = 0;
    int largest_score = 0;
    for (; moved < moves.length(); moved++)
    {
        int score = this->compute_score();
        this->move( moves.at(moved) );
        if (score > largest_score) largest_score = score;
        if (score >= SOLVED_SCORE)
        {
            qDebug() << "Solved! at move: " << moved;
            break;
        }
    }
    if (moved < STARTING_MOVES)
        largest_score = largest_score * (STARTING_MOVES - moved) * SCORE_SOLVE_MULTIPLIER;
    this->score = largest_score;
    return largest_score;
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

int CubeSolver::Organism::compute_score()
{
    int total_score = 0;
    for (int i = 0; i < FaceCount; i++)
    {
        int matched_colors = p_cube->getFace(i).isAlmostSolved();
        if (matched_colors == CUBE_DIMENSION*CUBE_DIMENSION-1)
        {
            //qDebug() << "One face completed";
            total_score += SCORE_FACE_COMPLETED;
        }
        else
            total_score += matched_colors;
    }
    return total_score;
}

