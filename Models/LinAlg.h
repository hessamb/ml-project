#ifndef __LINALG__
#define __LINALG__

#include <iostream>
#include <stdio.h>
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
    if(n != input.n)
	{
		n = input.n ;
		delete []a ;
		a = new double [n] ;
    }
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
  
  inline Vector operator -(const Vector& v2)const
  {
    if(v2.n != n)
    {
      cerr << "dimention mismatch" << endl ;
      return *this ;
    }
    Vector result(n) ;
    for(int i=0 ; i<n ; i++)
      result[i] = a[i] - v2.a[i] ;
    return result ;

  }
  
  inline Vector operator *(const double c)const
  {
    Vector result(n) ;
    for(int i=0 ; i<n ; i++)
      result[i] = c * a[i] ;
    return result ;
  }

  inline int operator * (const Vector& v2)const
  {
    if(v2.n != n)
    {
      cerr << "dimention mismatch" << endl ;
      return -1 ;
    }
    double sum = 0 ;
    for (int i=0 ; i<n ; i++)
    {
      sum += a[i] * v2.a[i] ;
    }
    return sum ;
  }

  inline void operator -= (const Vector& v2)const
  {
    if(v2.n != n)
    {
      cerr << "dimention mismatch" << endl ;
      return  ;
    }

    for (int i=0 ; i<n ; i++)
    {
      a[i] -=  v2.a[i] ;
    }
  }
	inline void operator += (const Vector& v2)const
	{	
		if(v2.n != n)
		{
			cerr << "dimention mismatch" << endl ;
			return  ;
		}
		
		for (int i=0 ; i<n ; i++)
		{
			a[i] +=  v2.a[i] ;
		}
    }
 	
	void save (string filename)
	{
		FILE * pFile ;
		pFile = fopen (filename.c_str(),"w+");
		fprintf (pFile, "%d \n " , n);
		for (int i=0 ; i<n ; i++)
			fprintf (pFile, "%lf " , a[i]);
		fclose (pFile);
	}
	
	void load (string filename)
	{
		FILE * pFile ;
		pFile = fopen (filename.c_str(),"r");
		for (int i=0 ; i<n ; i++)
			fscanf (pFile, "%lf " , &a[i]);
		fclose (pFile);
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
  void save (string filename)
	{
		FILE * pFile ;
		pFile = fopen (filename.c_str(),"w+");
		fprintf (pFile, "%d %d \n " , m , n);
		for (int i=0 ; i<m ; i++)
		{
			for(int j=0 ; j<n ; j++ )
				fprintf (pFile, "%lf " , a[i][j]);
			fprintf (pFile,"\n") ;
		}
		
		fclose (pFile);
	}
	void load (string filename)
	{
		FILE * pFile ;
		pFile = fopen (filename.c_str(),"r");
		for (int i=0 ; i<m ; i++)
		{
			for(int j=0 ; j<n ; j++ )
				fscanf (pFile, "%lf " , &a[i][j]);
			
		}
		
		fclose (pFile);
	
	}
	
	~Matrix () 
	{
		delete []a ;
	}

};

#endif
