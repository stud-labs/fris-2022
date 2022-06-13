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
  void printFrame(ostream& out, size_t limit = 100);
    double fun(size_t x,size_t x1,size_t u); //fris
    double rFun(size_t x1, size_t u);        // ред fris
    double frisClus(ssize_t c1, ssize_t c2, size_t u);
    void stolps(size_t maxNumber);
    double meanmix();
    size_t classification(size_t u);

protected:
    bool calcdiss();
    //ssize_t get(size_t a, size_t b, size_t u, bool minimal=true);
      ssize_t get(size_t a, size_t b, size_t u, bool minimal);
    //ssize_t findNearest(size_t c, size_t u, bool inc=true, bool ext = false, bool unc = false);
    ssize_t findNearest(size_t c, size_t u, bool inc, bool ext, bool unc);
    ssize_t stolp(ssize_t c);
    double mean(ssize_t c, size_t u); // среднее fris, дл€ классифицированной выборки
private:
    // vector<double> m_v;
    vector<vector<double>> m_frame;
    vector<ssize_t> m_class;
    vector<ssize_t> m_stolps;
    vector<size_t> vis;
    vector<size_t> viss;
    Matrix * m_diss = nullptr;
public:
    ~FRIS();
private:
    size_t h=0, w=0;
private:
    // By definition it is Euler distance
  virtual double diss(size_t row1, size_t row2);
public:
    void test1();
    //void stolps(size_t maxNumber = 10);
};

#endif // __CORE_H__
