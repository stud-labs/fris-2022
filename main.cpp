#include <iostream>
#include "core.h"

#ifdef _OPENMP
#include <omp.h>
#endif
#include <thread>  //ïîòîêè
#include <chrono>  //âðåìÿ

// #define FILENAME "C:\\fris\\fristdr\\R\\data4.txt"
//#define FILENAME "./data4.txt"
#define FILENAME "./data5.txt"

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
    f.printFrame(cout);
    cout << "\nCalculating\n";
    f.calculate();
    cout << "Print result\n";
    cout << "\nfrisClus:" <<f.frisClus(1,0,13)<<endl;
    f.stolps(2);
    f.printFrame(cout);
    cout << "\nmixmean = " << f.meanmix()<<endl;
    f.outResult(cout);
    /*#pragma omp parallel
    {
    cout << "\nOpenMP Test" << endl; //îäèí ïîòîê??
    }
    */
    return 0;
}
