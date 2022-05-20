#include "core.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <assert.h>
#include <math.h>

using namespace std;


double FRIS::diss(vector<double> o1, vector<double> o2) {
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

    for (size_t row=0;row<h; row++) {         //по ряду
        for (size_t col=0;col<h; col++) {     //по колонке
            vector<double> row1;
            vector<double> row2;
            for (size_t c=0;c<w;c++) {
               row1.push_back(m_frame[c][row]);
               row2.push_back(m_frame[c][col]);
            }
            double d = diss(row1,row2);
            //m_diss->mData[row*m_diss->mCols+col] = d;
            (*m_diss)(row,col)=diss(row1,row2);
        }
    }
    m_diss->print(cout);
    return true;
}

FRIS::~FRIS() {
    if (m_diss != nullptr) delete m_diss;
}

bool FRIS::calculate() {
    if (!calcdiss()) return false;
    return true;
};


bool FRIS::loadData(string filename) {
    // Read tabular data from file.
    // Each row in the file consists of numbers separated by
    // spaces or tabulators
    // istream inp(filename);

    std::string line;
    std::ifstream in("C:\\fris\\fristdr\\R\\new.csv");
    if (in.is_open()){
        size_t i=0;
        while (getline(in, line)) {
            // std::cout << line << std::endl;
            istringstream sti(line);
            size_t col = 0;
            while (true) {
                if (sti.eof()) break;
                double num;
                sti>>num;
                // cout<<col<<"-"<<num<<"\n";
                if(sti.fail()){
                    break;
                }
                if (col<m_frame.size()) {
                    vector<double>& v = m_frame[col];
                    v.push_back(num);
                    i=v.size();
                }  else {
                    vector<double> v;
                    m_frame.push_back(v);
                    v.push_back(num);
                    //i=v.size();
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
    return mData[i * mCols + j];
}

double Matrix::operator()(size_t i, size_t j) const {
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
