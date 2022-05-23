#include <iostream>
#include "core.h"

using namespace std;

int main()
{
    FRIS f;
    f.loadData("./data3.txt");
    f.printFrame(cout);
    cout << "\nfrisClus:" <<f.frisClus(1,0,14)<<endl;
    f.calculate();
    f.test1();
    f.outResult(cout);

    return 0;
}
