#ifndef __CORE_H__
#define __CORE_H__

#include <string>
#include <iostream>
#include <vector>

using namespace std;
class Matrix {
public:
    Matrix() {};
    Matrix(size_t rows, size_t cols);
    double& operator()(size_t i, size_t j);
    double operator()(size_t i, size_t j) const;
    void print(ostream& o);

public:
    size_t mRows;
    size_t mCols;
    vector<double> mData;
};

class FRIS {
public:
    FRIS() {};
    bool loadData(string filename);
    bool outResult(ostream& stream);
    bool calculate();
    void printFrame(ostream& out);
    double fun(size_t x,size_t x1,size_t u); //fris
    double rFun(size_t x1, size_t u);        // ред fris
    double frisClus(ssize_t c1, ssize_t c2, size_t u);
    double mean(ssize_t c, size_t u);        // среднее fris, дл€ классифицированной выборки

protected:
    bool calcdiss();
    ssize_t get(size_t a, size_t b, size_t u, bool minimal=true) {

    ssize_t findNearest(size_t c, size_t u, bool inc=true, bool ext = false, bool unc = false);
    ssize_t stolp(ssize_t c);
private:
    // vector<double> m_v;
    vector<vector<double>> m_frame;
    vector<ssize_t> m_class;
    vector<size_t> m_stolps;
    Matrix * m_diss = nullptr;
public:
    ~FRIS();
private:
    size_t h=0, w=0;
private:
    // By definition it is Euler distance
    virtual double diss(vector<double> o1, vector<double> o2);
public:
    void test1();
    void stolps(size_t maxNumber = 10);
};

#endif // __CORE_H__
