#include <iostream>
#include "core.h"

// #define FILENAME "C:\\fris\\fristdr\\R\\data4.txt"
#define FILENAME "./data4.txt"

using namespace std;


int main()
{
    FRIS f;
    cout << "Loading data\n";
    f.loadData(FILENAME);
    cout << "Frame loaded\n";
    f.printFrame(cout);
    cout << "Calculating\n";
    f.calculate();
    cout << "Print result\n";
    cout << "\nfrisClus:" <<f.frisClus(1,0,13)<<endl;
    f.test1();
    f.outResult(cout);

    return 0;
}
