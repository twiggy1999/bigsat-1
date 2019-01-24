#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
using namespace std;

// Only parameters to make changes to
#define maxVars 30
#define maxClauses 1000
#define AvgLitsPerClause 5
#define range 0.1          // double[0~0.5]: About (2*100*range)% variables will be set to extreme (both low and high occurrence)
#define extreme_cmp 10     // int[1~INF] extreme_cmp = average_occurrence_of_high_ones / average_occurrence_of_low_ones

// Assisting functions
inline int rand_lits(int avg) { return avg / 2 + (rand() % avg); }
inline int rand_lit(int max) {
	int border1 = maxVars * range;
	int border2 = 2*border1;

	int i = (rand() % maxVars) + 1;
	int j = rand() % (extreme_cmp + 1);
	int k = rand() % 2;

	if (1 <= i && i <= border1) {
		if (j <= 1);
		else	i+= border1;
	}
	
	i = (k == 1) ? -i : i;
	return i;
}

int main()
{
        ofstream out("test.txt", ios::out);

        out << "c ++++ Auto Generated Input File ++++ " << endl;
        out << "c Parameters: " << endl;
        out << "c Maximum variables: " << maxVars << endl;
        out << "c Maximum clauses: " << maxClauses << endl;
        out << "c Average literals per clause: " << AvgLitsPerClause << endl;
	out << "c Variables from 1 to " << (int)(maxVars * range) << " will significantly occur less." << endl;
	out << "c Variables from " << (int)(maxVars * range) + 1 << " to " << 2*(int)(maxVars * range) << " will frequently appear." << endl; 	

        out << "p cnf " << maxVars << " " << maxClauses << endl;

	srand(time(0));

        for (int i = 0; i < maxClauses; i++) {
                int lits = rand_lits(AvgLitsPerClause);
                for (int j = 0; j < lits; j++) {
                        out << rand_lit(maxVars) << ' ';
                }
                out << 0 << endl;
        }
        return 0;
}

