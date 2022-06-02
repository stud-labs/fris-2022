#include "core.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>
#include <thread>  //потоки
#include <chrono>  //время



using namespace std;

double FRIS::fun(size_t x1, size_t x2, size_t u){ //функция конкурентного сходства

      if (x1 == x2) return 0.0;
      double f = ((*m_diss)(u,x1) - (*m_diss)(u,x2))/((*m_diss)(u,x1) + (*m_diss)(u,x2));
      // double f = ((r1 - (*m_diss)(u,x1))/(r1 + (*m_diss)(u,x1)));
      return f;
}

double FRIS::rFun(size_t x1, size_t u){ // функция конкурентного сходства по неклассифицированной выборке
      double r = 1000000;               // расстояние до виртуального образа
      double f = ((r - (*m_diss)(u,x1))/(r + (*m_diss)(u,x1)));
      return f;
}

double FRIS::diss(vector<double> o1, vector<double> o2) { //вычисление дистанции между точками
    double s=0;
    //int n = omp_get_num_threads();
    #pragma omp parallel for
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
            double d = diss(row1,row2);       // заполнение матрицы
            //m_diss->mData[row*m_diss->mCols+col] = d;
            (*m_diss)(row,col)=diss(row1,row2);
        }
    }
     m_diss->print(cout);
    // comment
    return true;
}

FRIS::~FRIS() {
    if (m_diss != nullptr) delete m_diss;
}

struct search_t{
    ssize_t minIndex = -2;
    double minDist;
    void update(double d, size_t row);

private:
    bool first = true;
};

void search_t::update(double d, size_t row) {
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

ssize_t FRIS::findNearest(size_t c, size_t u, bool inc, bool ext, bool unc) {
    // c>=0 ищет элемент с минимальной дистанцией до u в классе с (not=false, unc=false) ИЛИ
    // not = true ищет элемент везде, но не в с
    // unc = true ищет в неклассифицированной выборке
    // inc = true ищет в классе с
    // int t = 0; // считает кол-во элементов в классе
    search_t s;
    for (size_t row=0; row<h; row++) {
        double d = (*m_diss)(u, row);
        if (inc) {
            if (m_class[row]==c and d!=0) { //добавила условие d!=0
                s.update(d, row);
            };
        }
        if (ext) {
            if (m_class[row]!=c and m_class[row]>=0 and d!=0) {
                s.update(d, row);
            }
        }
        if (unc) {
            if (m_class[row]<0 and d!=0) {
                s.update(d, row);
            }
        }
    };
    //cout<<"\nt for "<<c<<" = "<<t<<endl;
    return s.minIndex; // возвращает индекс элемента !!!
}

double FRIS::frisClus(ssize_t c1, ssize_t c2, size_t u) {
  assert(u>=0);
  assert(u<=h);
  //                              inc    ext   unknown
  ssize_t x1 = findNearest(c1, u, true, false, false);
  ssize_t x2 = findNearest(c2, u, true, false, false);
  cout << "1c["<<x1<<"]="<<m_class[x1]<<"="<<c1<<endl;
  cout << "2c["<<x2<<"]="<<m_class[x2]<<"="<<c2<<endl;
  // double r1 = min((*m_diss)(x1,u),(*m_diss)(x2,u)); // r1 для алгоритма
  // cout << "min diss r1 ="<<r1<<endl;
  // cout << "f(x1,x2,u)=";
  return fun(x1,x2,u);
}

ssize_t FRIS::get(size_t a, size_t b, size_t u, bool minimal) {
    double da,db;
    da = (*m_diss)(a, u);
    db = (*m_diss)(b, u);
    if (minimal) {
        return da<db ? a:b;
    } else {
        return da<db ? b:a;
    }
}

double FRIS::meanmix(){
    ssize_t c1;
    ssize_t c2;
    ssize_t u1;
    ssize_t u2;
    size_t x2 = -1;
    int t=0;
    double sum;
    for (size_t row=0; row<h; row++) {
        c1=m_class[row];
        u1=stolp(c1);
        //x2 = findNearest(c,u,true,true,true); // не правильно, берет сам столп!!! --исправлено --не правильно, нужен ближайший столп из другого класса
        x2 = findNearest(c1,u1,false,true,true);
        c2 = m_class[x2];
        u2 = stolp(c2);
        //sum = sum+fun(row,x2,u);
        sum = sum+fun(u2,u1,row); // сначала конкурирующий потом свой
        t++;
        cout<< "\n c1["<<c1<<"]="<<u1<<endl;
        cout<< "x2:"<<x2<<endl;
        cout<< "c2["<<c2<<"]="<<u2<<endl;
        cout<< "sum:"<<sum<<endl;
    }
    cout<< "\nt:"<<t<<endl;
    return sum/t;
}

double FRIS::mean(ssize_t c, size_t u){ // среднее значение FRIS
    double s;
    double f;
    int t = 0;
    bool first = true;
    size_t x1 =-1, x2 = -1;
    ssize_t c2=-2;  // Other Class not equal to "c" of u
//  ssize_t c1=-2;  // intermediate class
    if (c<0) return c;

    if (m_class[u] == c) {
        //                     inc,   ext, unknown
        x1 = findNearest(c, u, true, false, true);
        x2 = findNearest(c, u, false, true, false);
    } else {
        c2 = m_class[u];
        if (c2 >=0 ) {
            x1 = findNearest(c2, u, true, false, true);
            x2 = findNearest(c, u, true, false, false);
        } else { // u is non classified
            x1 = findNearest(c, u, true, true, true);
            size_t x21 = findNearest(c, u, true, false, false);
            size_t x22 = findNearest(c, u, false, true, false);
            x21 = get(x21,x22, u, false);  // index!
            x22 = findNearest(c, u, false, true, true);
            x2 = get(x21,x22,u, true);
        }
    }
    // found x1, x2
    //cout << "\nx1:" << x1<< endl;
    //cout << "x2:" << x2<< endl;
    for (size_t row=0; row<h; row++) {
        if (m_class[row]==c) {
            double f = fun(x2,row,u);       // rFun(row,u); !!! сначала конкурирующий, потом "свой"
           // cout<<"c["<<row<<"]="<<f<<endl; // выводить для проверки
            s=s+f;
            t++;
        }
    }
    //cout << "s:" << s << endl;
    s=s/t;
    //cout <<"t:" << t << endl;
    return s;
}

ssize_t FRIS::stolp(ssize_t c){ //найти средее функции конку*96рентного сходства
    ssize_t s = -1;
    double m = -2;

    if (c<0) return c;

    for (size_t row=0; row<h; row++) {
        if (m_class[row]==c) {
            double a = mean(c, row); // calc Favg(c, row,...)
            if (a>m) {
                m = a;
                s = row;
            }
        }
    }
    return s;
}

bool FRIS::calculate() {
    if (!calcdiss()) return false;
    return true;
};

void FRIS::stolps(size_t maxNumber) {
    vector<size_t> vis; //..ited
    vector<size_t> viss; // .. numbers of stolps
    for (size_t row=0; row<h; row++ ) {
        ssize_t s = -1;
        ssize_t c = m_class[row];
        if (c<0) goto again;
        for (size_t ind=0; ind<vis.size(); ind++) {
            if (vis[ind]==c) {
                s = viss[ind];
                goto again;
            }
        }

        s = stolp(c);

        again:
        m_stolps.push_back(s);
    }
}

void FRIS::test1() {
    setlocale(LC_ALL,"Russian");
    wcout <<"Result"<<endl;
  //  cout<< "f=";
  //  cout<<fun(3,4,2)<<endl;*/
  //  cout<<"rF= "<<rFun(3,13)<<endl;
    cout<< "f среднее:" <<endl;
    cout<< mean(0,2)<<endl;

    //cout<< mean(1,3)<<endl;
    //cout<< mean(0,3)<<endl;
}

bool FRIS::loadData(string filename) {
    // Read tabular data from file.
    // Each row in the file consists of numbers separated by
    // spaces or tabulators
    // istream inp(filename);

    std::string line;
    ifstream in(filename);
    if (in.is_open()){
        size_t i=0;
        while (getline(in, line)) {
            //std::cout << "DBG:" << line << std::endl;
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
                    }
                }
                col++;
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
    out << "Frame:" << endl;
    size_t i = 0;
    out<<"#\tst\tcls\tdata\n";
    while(true) {
        out<<i<<"\t";
        if (i<m_stolps.size()) {
            out<<m_stolps[i]<<"\t";
        } else {
            out<<"-\t";
        }
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
    return mData[i * mCols + j];
}

double Matrix::operator()(size_t i, size_t j) const {
    assert(i>=0);
    assert(i<mRows);
    assert(j>=0);
    assert(j<mCols);
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
