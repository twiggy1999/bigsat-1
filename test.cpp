#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
using namespace std;

// Only parameters to make changes to
#define maxVars 100
#define maxClauses 10000
#define AvgLitsPerClause 6

// Assisting functions
inline int rand_lits(int avg) { return avg / 2 + (rand() % avg); }
inline int rand_lit(int max) { int i = 0; while (!i) i = rand() % (2 * max + 1) - max; return i; }

int main()
{
        ofstream out("test.txt", ios::out);

        out << "c ++++ Auto Generated Input File ++++ " << endl;
        out << "c Parameters: " << endl;
        out << "c Maximum variables: " << maxVars << endl;
        out << "c Maximum clauses: " << maxClauses << endl;
        out << "c Average literals per clause: " << AvgLitsPerClause << endl;

        out << "p cnf " << maxVars << " " << maxClauses << endl;

        for (int i = 0; i < maxClauses; i++) {
                int lits = rand_lits(AvgLitsPerClause);
                for (int j = 0; j < lits; j++) {
                        out << rand_lit(maxVars) << ' ';
                }
                out << 0 << endl;
        }
        return 0;
}

