#include <iostream>
#include "core.h"

using namespace std;

int main()
{
    FRIS f;
    f.loadData("./data3.txt");
    f.printFrame(cout);
    f.calculate();
    f.outResult(cout);

    return 0;
}
