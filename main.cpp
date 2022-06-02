#include <iostream>
#include "core.h"

#include <omp.h>
#include <thread>  //потоки
#include <chrono>  //время

// #define FILENAME "C:\\fris\\fristdr\\R\\data4.txt"
//#define FILENAME "./data4.txt"
#define FILENAME "./data5.txt"

using namespace std;

int main()
{
    FRIS f;
    cout << "Loading data\n";
    f.loadData(FILENAME);
    cout << "Frame loaded\n";
    f.printFrame(cout);
    cout << "\nCalculating\n";
    f.calculate();
    cout << "Print result\n";
    cout << "\nfrisClus:" <<f.frisClus(1,0,13)<<endl;
    .test1();
    f.stolps(2);
    f.printFrame(cout);
    cout << "\nmixmean = " << f.meanmix()<<endl;
    f.outResult(cout);
    /*#pragma omp parallel
    {
    cout << "\nOpenMP Test" << endl; //один поток??
    }
    */
    return 0;
}
