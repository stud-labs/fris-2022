#include <iostream>
#include "core.h"

#ifdef _OPENMP
#include <omp.h>
#endif

#include <unistd.h>

//#define FILENAME "C:\\fris\\fristdr\\R\\data4.txt"6
//#define FILENAME "./data4.txt"
//#define FILENAME "./data5.txt"
#define FILENAME "./data7.txt"

using namespace std;

int main()
{
    FRIS f;
#ifdef _OPENMP
    cout << "No of cores:" << omp_get_num_procs() << endl;
#else
    cout << "Sequential algorithm used\n";
#endif

    cout << "Loading data\n";
    f.loadData(FILENAME);
    cout << "Frame loaded\n";
    // f.printFrame(cout);
    cout << "\nCalculating\n";
    f.calculate();
    // cout << "Print result\n";
    // cout << "\nfrisClus:" <<f.frisClus(1,0,13)<<endl;
    f.stolps(3);
    f.printFrame(cout, 10);
    cout << "\nmixmean = " << f.meanmix()<<endl;
    //f.outResult(cout);
    // exit(0);
    f.classification(4);

    return 0;
}
