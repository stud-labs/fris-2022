#include <iostream>
#include "core.h"

using namespace std;

int main()
{
    FRIS f;
    cout << "Loading data\n";
    f.loadData("./data3.txt");
    cout << "Frame loaded\n";
    f.printFrame(cout);
    cout << "Calculating\n";
    f.calculate();
    cout << "Print result\n";
    f.outResult(cout);

    return 0;
}
