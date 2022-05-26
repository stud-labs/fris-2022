#include "core.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

using namespace std;

//double FRIS::fun(int x, int x1, int u){ //функци€ конкурентного сходства
double FRIS::fun(size_t x1, size_t x2, size_t u){
      // assert(x<h||x1<h||u<h);
      // double r1=1000000;
      //double r2=1;
/*      for (size_t row=0;row<h; row++){              //по р€ду
       for (size_t col=0;col<h; col++){              //по колонке
         if(((*m_diss)(u,col)<r1)&(col!=u)){
            r1 = (*m_diss)(u,col);
         }
       }
      }
      cout <<"r1:"<<r1<<endl;
  */
      if (x1 == x2) return 0.0;

      double f = ((*m_diss)(u,x1) - (*m_diss)(u,x2))/((*m_diss)(u,x1) + (*m_diss)(u,x2));
   //   double f = ((r1 - (*m_diss)(u,x1))/(r1 + (*m_diss)(u,x1)));
      return f;
}

double FRIS::diss(vector<double> o1, vector<double> o2) { //вычисление дистанции между точками
    double s=0;
    for (size_t col=0; col<o1.size(); col++) {
        double d = (o1[col]-o2[col]);
        s+=d*d;
    }
    return sqrt(s);
}

bool FRIS::calcdiss() {
    assert(w>0);
    cout << "h:";
    cout<<h<<endl;
    cout << "w:";
    cout<<w<<endl;
    // cout<<v.size();
    m_diss = new Matrix(h,h);

    for (size_t row=0;row<h; row++) {         //по р€ду
        for (size_t col=0;col<h; col++) {     //по колонке
            vector<double> row1;
            vector<double> row2;
            for (size_t c=0;c<w;c++) {
               row1.push_back(m_frame[c][row]);
               row2.push_back(m_frame[c][col]);
            }
            double d = diss(row1,row2);       // заполнение матрицы
            //m_diss->mData[row*m_diss->mCols+col] = d;
            (*m_diss)(row,col)=diss(row1,row2);
        }
    }
    // m_diss->print(cout);
    // comment
    return true;
}

FRIS::~FRIS() {
    if (m_diss != nullptr) delete m_diss;
}

ssize_t FRIS::findNearest(ssize_t c, size_t u) {
    if (c<0) return c;
    ssize_t minIndex = -2;
    double minDist;
    bool first = true;
    for (size_t row=0; row<h; row++) {
        if (m_class[row]==c) {
            double d = (*m_diss)(u, row);
            if (first) {
                minDist = d;
                minIndex = row;
                first = false;
            } else {
                if (minDist > d) {
                    minDist = d;
                    minIndex = row;
                }
            }
        }
    };
    return minIndex;
}


double FRIS::frisClus(ssize_t c1, ssize_t c2, size_t u) {
  //  ssize_t x1 = findNearest(c1, u);
  //  ssize_t x2 = findNearest(c2, u);
  //  return fun(x1,x2,u);
  assert(u>=0);
  assert(u<=h);
  ssize_t x1 = findNearest(c1,u);
  ssize_t x2 = findNearest(c2,u);
  cout <<"1c["<<x1<<"]="<<m_class[x1]<<"="<<c1<<endl;
  cout <<"2c["<<x2<<"]="<<m_class[x2]<<"="<<c2<<endl;
  return fun(x1,x2,u);
}

bool FRIS::calculate() {
    if (!calcdiss()) return false;
    return true;
};


void FRIS::test1() {
    setlocale(LC_ALL,"Russian");
    wcout <<"—ообщение"<<endl;
    wcout <<"Result"<<endl;
    cout << "f=";
    cout<<fun(3,4,2)<<endl;
}

bool FRIS::loadData(string filename) {
    // Read tabular data from file.
    // Each row in the file consists of numbers separated by
    // spaces or tabulators
    // istream inp(filename);

    std::string line;
    // std::ifstream in("C:\\fris\\fristdr\\R\\new.csv");
    //std::ifstream in("C:\\fris\\fristdr\\R\\data4.txt");
    ifstream in(filename);
    if (in.is_open()){
        size_t i=0;
        while (getline(in, line)) {
            std::cout << "DBG:" << line << std::endl;
            istringstream sti(line);
            size_t col = 0;
            while (true){
                if (sti.eof()) break;
                double num;
                ssize_t cls;
                if (col == 0) {
                    sti>>cls;
                } else {
                    sti>>num;
                }
                // cout<<col<<"-"<<num<<"\n";
                if(sti.fail()){
                    break;
                }
                if (col == 0) {
                    m_class.push_back(cls);
                } else {
                    assert(col>0);
                    size_t col1=col-1;
                    if (col1<m_frame.size()) {
                        vector<double>& v = m_frame[col1];
                        v.push_back(num);
                        i=v.size();
                    }  else {
                        vector<double> v;
                        v.push_back(num);
                        m_frame.push_back(v);
                        //i=v.size();
                    }
                }
                col++;
                //i++;
            }
        }
        h=i;
        assert(h>0);
    }
    in.close();
    w=m_frame.size();

    return true;
};

void FRIS::printFrame(ostream& out) {
    out << "file open:" << endl;
    size_t i = 0;
    while(true) {
        out<<m_class[i]<<"\t";
        for (vector<double> v: m_frame) {
                if (v.size()<=i) goto end;
                double d = v[i];
                out<<d<<"\t";
            }
            out<<endl;
        i++;
    }
    h=i;
    //out<<i<<endl;
    end:;
}

bool FRIS::outResult(ostream& stream) {
    stream<<"Hello world"<<endl;
    return true;
};


Matrix::Matrix(size_t rows, size_t cols)
    : mRows(rows),
    mCols(cols),
    mData(rows * cols) {
}

double& Matrix::operator()(size_t i, size_t j) {
    assert(i>=0);
    // cout << "\nMATR:" << mCols << " " << mRows << endl;
    // cout << "    :" << i << " " << j << endl;
    assert(i<mRows);
    assert(j>=0);
    assert(j<mCols);
    //cout << "MATR:" << mCols << " " << mRows << endl;
    return mData[i * mCols + j];
}

double Matrix::operator()(size_t i, size_t j) const {
    //assert(i>=0);
    //assert(i<mRows);
    //assert(j>=0);
    //assert(j<mCols);
    return mData[i * mCols + j];
}

void Matrix::print (ostream& o){
    o << "Matrix:" <<endl;
    for (size_t row=0;row<mRows; row++) {
        for (size_t col=0;col<mCols; col++) {
            double d = mData[row*mCols+col];
            o<<d<<"\t";
        }
        o<<endl;
    }
}
