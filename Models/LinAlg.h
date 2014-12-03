#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <cstring>

using namespace std ;


class Vector
{
public:
  int n ;
  double *a ;

  Vector(int nn=0)
  {
    n = nn ;
    a = new double[n];
    memset(a,0,n*sizeof(double));
  }

  ~Vector(){ delete []a ;}

  inline void resize(int nn)
  {
    n = nn ;
    delete []a;
    a = new double[n];
    memset(a,0,n*sizeof(double));
  }

  inline double & operator [](int i)
  {
    if (i>= n){
      cerr << "Limit Exceeded" << endl;

      return *new double(-1);
    }
    return a[i] ;
  }

  inline Vector operator = (const Vector& input)
  {
    n = input.n ;
    delete []a ;
    a = new double [n] ;
    copy (input.a,input.a+n,a);
    return *this ;
  }

  inline Vector (const Vector& input)
  {
    n = input.n ;
    a = new double [n] ;
    copy ( input.a , input.a+n , a);
  }

  inline Vector operator +(const Vector& v2)const
  {
    if(v2.n != n)
    {
      cerr << "dimention mismatch" << endl ;
      return *this ;
    }
    Vector result(n) ;
    for(int i=0 ; i<n ; i++)
      result[i] = a[i] + v2.a[i] ;
    return result ;

  }
  inline Vector operator *(const int c)const
  {
    Vector result(n) ;
    for(int i=0 ; i<n ; i++)
      result[i] = c*a[i] ;
    return result ;
  }

  inline int operator * (const Vector& v2)const
  {
    if(v2.n != n)
    {
      cerr << "dimention mismatch" << endl ;
      return -1 ;
    }
    int sum = 0 ;
    for (int i=0 ; i<n ; i++)
    {
      sum += a[i]*v2.a[i] ;
    }
    return sum ;
  }

};

class Matrix
{
public:

  int m,n ;
  Vector* a;

  Matrix (int mm=0 , int nn=0 )
  {
    m = mm ;
    n = nn ;
    a =  new Vector [m];

    for (int i=0 ; i<m ; i++)
    {
      a[i].resize(n);
    }

  }

  Vector& operator [](int i)
  {
    if ( i >= m )
    {
      cerr << "Index Exceeded Boundaries" << endl;
      return *new Vector(0);
    }
    else
      return a[i] ;
  }
};
