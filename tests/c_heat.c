// -*-c++-*-
//
//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
//
// MASA - Manufactured Analytical Solutions Abstraction Library
//
// Copyright (C) 2010 The PECOS Development Team
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the Version 2.1 GNU Lesser General
// Public License as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc. 51 Franklin Street, Fifth Floor,
// Boston, MA  02110-1301  USA
//
//-----------------------------------------------------------------------el-
// $Author: nick $
// $Id: c_euler1d.c 13404 2010-09-15 02:56:33Z nick $
//
// c_misc.c :program that tests masa helper functions
//
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include <config.h>
#include <masa.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const double threshold = 1.0e-15; // should be small enough to catch any obvious problems

double SourceQ_t_1d(double x, double A_x, double k_0)
{
  double Q_T = A_x * A_x * k_0 * cos(A_x * x);
  return Q_T;
}

double Source_t_1d_an(double A_x,double x)
{
  double T_an;
  T_an = cos(A_x * x);
  return T_an;
}

double SourceQ_t_2d (
  double x,
  double y,
  double A_x,
  double B_y,
  double k_0)
{
  double Q_T = k_0 * cos(A_x * x) * cos(B_y * y) * (A_x * A_x + B_y * B_y);
  return Q_T;
}

double SourceQ_t_3d (
  double x,
  double y,
  double z,
  double A_x,
  double B_y,
  double C_z,
  double k_0)
{
  double Q_T = k_0 * cos(A_x * x) * cos(B_y * y) * cos(C_z * z) * (A_x * A_x + B_y * B_y + C_z * C_z);
  return Q_T;
}




int main()
{
  int i;

  double tfield,tfield2,tfield3;
  double t_an,t_an2,t_an3;
  double x;
  double y;
  double z;

  double A_x;
  double k_0;

  double B_y;
  double C_z;  

  //problem size
  int nx = 200;  // number of points
  int lx=10;     // length
  double dx = (double)lx/(double)nx;

  // initalize everyone
  cmasa_init("temp-test-1d","heateq_1d_steady_const");
  cmasa_init_param();
  cmasa_sanity_check();

  A_x = cmasa_get_param("A_x");
  k_0 = cmasa_get_param("k_0"); 

  // evaluate source terms (1D)
  for(i=0;i<nx;i++)
    {
      x=i*dx;
      
      //evalulate source terms
      tfield = cmasa_eval_1d_t_source(x);
      
      //evaluate analytical terms
      t_an   = cmasa_eval_1d_t_an(x);
	
      // get fundamental source term solution
      tfield2   = SourceQ_t_1d  (x,A_x,k_0);
      t_an2     = Source_t_1d_an(x,A_x);

      // test the result is roughly zero
      // choose between abs and rel error
#ifdef MASA_STRICT_REGRESSION

      tfield3 = fabs(tfield-tfield2);
      t_an3   = fabs(t_an-t_an2);

#else

      tfield3 = fabs(tfield-tfield2)/fabs(tfield2);
      t_an3   = fabs(t_an-t_an2)/fabs(tfield2);

#endif

	if(tfield3 > threshold)
	  {
	    printf("\nMASA REGRESSION TEST FAILED: C-binding Euler-1d\n");
	    printf("U Field Source Term\n");
	    printf("Threshold Exceeded: %g\n",tfield3);
	    printf("CMASA:              %5.16f\n",tfield);
	    printf("Maple:              %5.16f\n",tfield2);
	    exit(1);
	  }

	if(t_an3 > threshold)
	  {
	    printf("\nMASA REGRESSION TEST FAILED: C-binding Euler-1d\n");
	    printf("U Field Analytical Term\n");
	    printf("Threshold Exceeded: %g\n",t_an3);
	    printf("CMASA:              %5.16f\n",t_an);
	    printf("Maple:              %5.16f\n",t_an2);
	    exit(1);
	  }
    } // done iterating


  cmasa_init("temp-test-2d","heateq_2d_steady_const");
  cmasa_init_param();
  cmasa_sanity_check();  

  cmasa_init("temp-test-3d","heateq_3d_steady_const");
  cmasa_init_param();
  cmasa_sanity_check();

}