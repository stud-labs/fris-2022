#include <iostream>
#include "core.h"

using namespace std;

int main()
{
    FRIS f;
    cout << "Loading data\n";
    f.loadData("./data4.txt");
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
