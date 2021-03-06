// -*-c++-*-
//
//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
//
// MASA - Manufactured Analytical Solutions Abstraction Library
//
// Copyright (C) 2010,2011,2012 The PECOS Development Team
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
// $Author$
// $Id$
//
// euler2d.cpp :program that tests euler2d from masa against known source term
//
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include <tests.h>
#include <cmath>

using namespace MASA;
using namespace std;

template<typename Scalar>
Scalar anQ_p (Scalar x,Scalar y,Scalar p_0,Scalar p_x,Scalar p_y,Scalar a_px,Scalar a_py,Scalar L)
{
  Scalar pi = std::acos(Scalar(-1));
  Scalar exact_p = p_0 + p_x * std::cos(a_px * pi * x / L) + p_y * std::sin(a_py * pi * y / L);
  return exact_p;
}
  
template<typename Scalar>
Scalar anQ_u (Scalar x,Scalar y,Scalar u_0,Scalar u_x,Scalar u_y,Scalar a_ux,Scalar a_uy,Scalar L)
{
  Scalar pi = std::acos(Scalar(-1));
  Scalar exact_u = u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L);
  return exact_u;
} 

template<typename Scalar> 
Scalar anQ_v (Scalar x,Scalar y,Scalar v_0,Scalar v_x,Scalar v_y,Scalar a_vx,Scalar a_vy,Scalar L)
{
  Scalar pi = std::acos(Scalar(-1));
  Scalar exact_v = v_0 + v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L);
  return exact_v;
}

template<typename Scalar>
Scalar anQ_rho (Scalar x,Scalar y,Scalar rho_0,Scalar rho_x,Scalar rho_y,Scalar a_rhox,Scalar a_rhoy,Scalar L)
{ 
  Scalar pi = std::acos(Scalar(-1));
  Scalar exact_rho = rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L);
  return exact_rho;
}

template<typename Scalar>
Scalar SourceQ_e (
  Scalar x,
  Scalar y,
  Scalar u_0,
  Scalar u_x,
  Scalar u_y,
  Scalar v_0,
  Scalar v_x,
  Scalar v_y,
  Scalar rho_0,
  Scalar rho_x,
  Scalar rho_y,
  Scalar p_0,
  Scalar p_x,
  Scalar p_y,
  Scalar a_px,
  Scalar a_py,
  Scalar a_rhox,
  Scalar a_rhoy,
  Scalar a_ux,
  Scalar a_uy,
  Scalar a_vx,
  Scalar a_vy,
  Scalar Gamma,
  Scalar L)
{
  Scalar pi = std::acos(Scalar(-1));
  Scalar Q_e;
  Scalar RHO;
  Scalar U;
  Scalar V;
  Scalar P;

  RHO = rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L);
  U = u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L);
  V = v_0 + v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L);
  P = p_0 + p_x * std::cos(a_px * pi * x / L) + p_y * std::sin(a_py * pi * y / L);

  Q_e = -a_px * pi * p_x * Gamma * U * std::sin(a_px * pi * x / L) / (Gamma - Scalar(0.1e1)) / L + a_py * pi * p_y * Gamma * V * std::cos(a_py * pi * y / L) / (Gamma - Scalar(0.1e1)) / L + (U * U + V * V) * a_rhox * pi * rho_x * U * std::cos(a_rhox * pi * x / L) / L / Scalar(0.2e1) - (U * U + V * V) * a_rhoy * pi * rho_y * V * std::sin(a_rhoy * pi * y / L) / L / Scalar(0.2e1) + (Scalar(0.3e1) * a_ux * u_x * std::cos(a_ux * pi * x / L) + a_vy * v_y * std::cos(a_vy * pi * y / L)) * pi * RHO * U * U / L / Scalar(0.2e1) - (a_uy * u_y * std::sin(a_uy * pi * y / L) + a_vx * v_x * std::sin(a_vx * pi * x / L)) * pi * RHO * U * V / L + (a_ux * u_x * std::cos(a_ux * pi * x / L) + Scalar(0.3e1) * a_vy * v_y * std::cos(a_vy * pi * y / L)) * pi * RHO * V * V / L / Scalar(0.2e1) + (a_ux * u_x * std::cos(a_ux * pi * x / L) + a_vy * v_y * std::cos(a_vy * pi * y / L)) * pi * Gamma * P / (Gamma - Scalar(0.1e1)) / L;

  return(Q_e);
}

template<typename Scalar>
Scalar SourceQ_u ( // 23 variables
  Scalar x,
  Scalar y,
  Scalar u_0,
  Scalar u_x,
  Scalar u_y,
  Scalar v_0,
  Scalar v_x,
  Scalar v_y,
  Scalar rho_0,
  Scalar rho_x,
  Scalar rho_y,
  Scalar p_x,
  Scalar a_px,
  Scalar a_rhox,
  Scalar a_rhoy,
  Scalar a_ux,
  Scalar a_uy,
  Scalar a_vx,
  Scalar a_vy,
  Scalar L)
{
  Scalar pi = std::acos(Scalar(-1));
  Scalar Q_u;
  Scalar RHO;
  Scalar U;
  Scalar V;

  RHO = rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L);
  U = u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L);
  V = v_0 + v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L);

  Q_u = a_rhox * pi * rho_x * U * U * std::cos(a_rhox * pi * x / L) / L - a_rhoy * pi * rho_y * U * V * std::sin(a_rhoy * pi * y / L) / L - a_uy * pi * u_y * RHO * V * std::sin(a_uy * pi * y / L) / L - a_px * pi * p_x * std::sin(a_px * pi * x / L) / L + (Scalar(0.2e1) * a_ux * u_x * std::cos(a_ux * pi * x / L) + a_vy * v_y * std::cos(a_vy * pi * y / L)) * pi * RHO * U / L;

  return(Q_u);
}

template<typename Scalar>
Scalar SourceQ_v (
  Scalar x,
  Scalar y,
  Scalar u_0,
  Scalar u_x,
  Scalar u_y,
  Scalar v_0,
  Scalar v_x,
  Scalar v_y,
  Scalar rho_0,
  Scalar rho_x,
  Scalar rho_y,
  Scalar p_y,
  Scalar a_py,
  Scalar a_rhox,
  Scalar a_rhoy,
  Scalar a_ux,
  Scalar a_uy,
  Scalar a_vx,
  Scalar a_vy,
  Scalar L)
{

  Scalar pi = std::acos(Scalar(-1));
  Scalar Q_v;
  Scalar RHO;
  Scalar U;
  Scalar V;
  RHO = rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L);
  U = u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L);
  V = v_0 + v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L);

  Q_v = a_rhox * pi * rho_x * U * V * std::cos(a_rhox * pi * x / L) / L - a_rhoy * pi * rho_y * V * V * std::sin(a_rhoy * pi * y / L) / L - a_vx * pi * v_x * RHO * U * std::sin(a_vx * pi * x / L) / L + a_py * pi * p_y * std::cos(a_py * pi * y / L) / L + (a_ux * u_x * std::cos(a_ux * pi * x / L) + Scalar(0.2e1) * a_vy * v_y * std::cos(a_vy * pi * y / L)) * pi * RHO * V / L;

  return(Q_v);
}

template<typename Scalar>
Scalar SourceQ_rho(
  Scalar x,
  Scalar y,
  Scalar u_0,
  Scalar u_x,
  Scalar u_y,
  Scalar v_0,
  Scalar v_x,
  Scalar v_y,
  Scalar rho_0,
  Scalar rho_x,
  Scalar rho_y,
  Scalar a_rhox,
  Scalar a_rhoy,
  Scalar a_ux,
  Scalar a_uy,
  Scalar a_vx,
  Scalar a_vy,
  Scalar L)
{
  Scalar pi = std::acos(Scalar(-1));
  Scalar Q_rho;
  Scalar RHO;
  Scalar U;
  Scalar V;

  RHO = rho_0 + rho_x * std::sin(a_rhox * pi * x / L) + rho_y * std::cos(a_rhoy * pi * y / L);
  U = u_0 + u_x * std::sin(a_ux * pi * x / L) + u_y * std::cos(a_uy * pi * y / L);
  V = v_0 + v_x * std::cos(a_vx * pi * x / L) + v_y * std::sin(a_vy * pi * y / L);

  Q_rho = a_rhox * pi * rho_x * U * std::cos(a_rhox * pi * x / L) / L - a_rhoy * pi * rho_y * V * std::sin(a_rhoy * pi * y / L) / L + (a_ux * u_x * std::cos(a_ux * pi * x / L) + a_vy * v_y * std::cos(a_vy * pi * y / L)) * pi * RHO / L;

  return(Q_rho);
}

template<typename Scalar>
int run_regression()
{  
  
  // need to add variable based on precision
  Scalar threshold = 5 * numeric_limits<Scalar>::epsilon();
  
  //variables
  Scalar u_0;
  Scalar u_x;
  Scalar u_y;
  Scalar v_0;
  Scalar v_x;
  Scalar v_y;
  Scalar rho_0;
  Scalar rho_x;
  Scalar rho_y;
  Scalar p_0;
  Scalar p_x;
  Scalar p_y;
  Scalar a_px;
  Scalar a_py;
  Scalar a_rhox;
  Scalar a_rhoy;
  Scalar a_ux;
  Scalar a_uy;
  Scalar a_vx;
  Scalar a_vy;
  Scalar Gamma;
  Scalar L;

  // parameters
  Scalar x;
  Scalar y;

  // solutions -- efield is MASA term, efield2 is maple, efield3 is abs error between them
  Scalar ufield,ufield2,ufield3;
  Scalar vfield,vfield2,vfield3;
  Scalar efield,efield2,efield3;
  Scalar rho,rho2,rho3;
  Scalar gradx,grady;

  Scalar exact_u,exact_u2,exact_u3;
  Scalar exact_v,exact_v2,exact_v3;
  Scalar exact_p,exact_p2,exact_p3;
  Scalar exact_rho,exact_rho2,exact_rho3;

  // initalize
  int nx = 115;  // number of points
  int ny = 68;  
  int lx=3;     // length
  int ly=1; 
  
  Scalar dx=Scalar(lx)/Scalar(nx);
  Scalar dy=Scalar(ly)/Scalar(ny);

  masa_init<Scalar>("euler-test","euler_2d");

  // set params
  masa_init_param<Scalar>();
  
  // get vars
  u_0 = masa_get_param<Scalar>("u_0");
  u_x = masa_get_param<Scalar>("u_x");
  u_y = masa_get_param<Scalar>("u_y");

  v_0 = masa_get_param<Scalar>("v_0");
  v_x = masa_get_param<Scalar>("v_x");
  v_y = masa_get_param<Scalar>("v_y");

  rho_0 = masa_get_param<Scalar>("rho_0");
  rho_x = masa_get_param<Scalar>("rho_x");
  rho_y = masa_get_param<Scalar>("rho_y");

  p_0 = masa_get_param<Scalar>("p_0");
  p_x = masa_get_param<Scalar>("p_x");
  p_y = masa_get_param<Scalar>("p_y");

  a_px = masa_get_param<Scalar>("a_px");
  a_py = masa_get_param<Scalar>("a_py");

  a_rhox = masa_get_param<Scalar>("a_rhox");
  a_rhoy = masa_get_param<Scalar>("a_rhoy");

  a_ux = masa_get_param<Scalar>("a_ux");
  a_uy = masa_get_param<Scalar>("a_uy");

  a_vx = masa_get_param<Scalar>("a_vx");
  a_vy = masa_get_param<Scalar>("a_vy");

  Gamma = masa_get_param<Scalar>("Gamma");
  L     = masa_get_param<Scalar>("L");

  // check that all terms have been initialized
  int err = masa_sanity_check<Scalar>();
  if(err != 0)
    {
      cout << "MASA :: Sanity Check Failed!\n";
      exit(1);
    } 

  // evaluate source terms (2D)
  for(int i=0;i<nx;i++)
    for(int j=0;j<ny;j++)    
      {
	x=i*dx;
	y=j*dy;
	
	//evalulate source terms
	ufield = masa_eval_source_rho_u  <Scalar>(x,y);
	vfield = masa_eval_source_rho_v  <Scalar>(x,y);
	efield = masa_eval_source_rho_e  <Scalar>(x,y);
	rho    = masa_eval_source_rho<Scalar>(x,y);
	
	//evaluate analytical terms
	exact_u = masa_eval_exact_u        <Scalar>(x,y);
	exact_v = masa_eval_exact_v        <Scalar>(x,y);
	exact_p = masa_eval_exact_p        <Scalar>(x,y);
	exact_rho = masa_eval_exact_rho    <Scalar>(x,y);

	// eval gradient terms
	gradx = masa_eval_grad_u<Scalar>(x,y,1);
	grady = masa_eval_grad_u<Scalar>(x,y,2);		

	gradx = masa_eval_grad_v<Scalar>(x,y,1);
	grady = masa_eval_grad_v<Scalar>(x,y,2);		

	gradx = masa_eval_grad_p<Scalar>(x,y,1);
	grady = masa_eval_grad_p<Scalar>(x,y,2);		
  
	gradx = masa_eval_grad_rho<Scalar>(x,y,1);
	grady = masa_eval_grad_rho<Scalar>(x,y,2);		

	// check against maple
	ufield2 = SourceQ_u<Scalar> (x,y,u_0,u_x,u_y,v_0,v_x,v_y,rho_0,rho_x,rho_y,p_x,a_px,a_rhox,a_rhoy,a_ux,a_uy,a_vx,a_vy,L);
	vfield2 = SourceQ_v<Scalar>   (x,y,u_0,u_x,u_y,v_0,v_x,v_y,rho_0,rho_x,rho_y,p_y,a_py,a_rhox,a_rhoy,a_ux,a_uy,a_vx,a_vy,L);
	rho2    = SourceQ_rho<Scalar> (x,y,u_0,u_x,u_y,v_0,v_x,v_y,rho_0,rho_x,rho_y,a_rhox,a_rhoy,a_ux,a_uy,a_vx,a_vy,L);  
	efield2 = SourceQ_e<Scalar>   (x,y,u_0,u_x,u_y,v_0,v_x,v_y,rho_0,rho_x,rho_y,p_0,p_x,p_y,a_px,a_py,a_rhox,a_rhoy,a_ux,a_uy,a_vx,a_vy,Gamma,L);
	
	exact_u2   = anQ_u<Scalar>   (x,y,u_0,u_x,u_y,a_ux,a_uy,L);
	exact_v2   = anQ_v<Scalar>   (x,y,v_0,v_x,v_y,a_vx,a_vy,L);
	exact_rho2 = anQ_rho<Scalar> (x,y,rho_0,rho_x,rho_y,a_rhox,a_rhoy,L);
	exact_p2   = anQ_p<Scalar>   (x,y,p_0,p_x,p_y,a_px,a_py,L);

	// test the result is roughly zero
	// choose between abs and rel error
#ifdef MASA_STRICT_REGRESSION

	ufield3 = std::abs(ufield-ufield2);
	vfield3 = std::abs(vfield-vfield2);
	efield3 = std::abs(efield-efield2);
	rho3    = std::abs(rho-rho2);
	
	exact_u3   = std::abs(exact_u-exact_u2);
	exact_v3   = std::abs(exact_v-exact_v2);
	exact_rho3 = std::abs(exact_rho-exact_rho2);
	exact_p3   = std::abs(exact_p-exact_p2);

#else

	ufield3 = std::abs(ufield-ufield2)/std::abs(ufield2);
	vfield3 = std::abs(vfield-vfield2)/std::abs(vfield2);
	efield3 = std::abs(efield-efield2)/std::abs(efield2);
	rho3    = std::abs(rho-rho2)/std::abs(rho2);
	
	exact_u3   = std::abs(exact_u-exact_u2)/std::abs(exact_u2);
	exact_v3   = std::abs(exact_v-exact_v2)/std::abs(exact_v2);
	exact_rho3 = std::abs(exact_rho-exact_rho2)/std::abs(exact_rho2);
	exact_p3   = std::abs(exact_p-exact_p2)/std::abs(exact_p2);

#endif

	threshcheck(ufield3,threshold);
	threshcheck(vfield3,threshold);
	threshcheck(efield3,threshold);
	threshcheck(rho3,threshold);
	
	threshcheck(exact_u3,threshold);
	threshcheck(exact_v3,threshold);
	threshcheck(exact_rho3,threshold);
	threshcheck(exact_p3,threshold);

	nancheck(gradx);
	nancheck(grady);

	if(0 > exact_rho)
	  {
	    cout << "\nMASA REGRESSION TEST FAILED: Euler-2d\n";
	    cout << "Initial Variables are returning non-physical results!\n";
	    cout << "RHO analytical\n";
	    exit(1);
	  }

	if(0 > exact_p)
	  {
	    cout << "\nMASA REGRESSION TEST FAILED: Euler-2d\n";
	    cout << "Initial Variables are returning non-physical results!\n";
	    cout << "Pressure is negative!\n";
	    exit(1);
	  }

      } // done iterating
  
  // reroute stdout for regressions: TODO remove when logger mechanism
  // is used inside masa; these tests currently just verify functions
  // run successfully.
  freopen("/dev/null","w",stdout);

  // test gradient error terms
  Scalar derr = masa_eval_grad_u<Scalar>(0,0,0);
  test_grad<Scalar>(derr);

  derr = masa_eval_grad_u<Scalar>(0,0,3);
  test_grad<Scalar>(derr);

  derr = masa_eval_grad_v<Scalar>(0,0,0);
  test_grad<Scalar>(derr);

  derr = masa_eval_grad_v<Scalar>(0,0,3);
  test_grad<Scalar>(derr);

  // p
  derr = masa_eval_grad_p<Scalar>(0,0,0);
  test_grad<Scalar>(derr);
  
  derr = masa_eval_grad_p<Scalar>(0,0,3);
  test_grad<Scalar>(derr);

  // rho
  derr = masa_eval_grad_rho<Scalar>(0,0,0);
  test_grad<Scalar>(derr);  

  derr = masa_eval_grad_rho<Scalar>(0,0,3);
  test_grad<Scalar>(derr);

  // all tests passed
  return 0;
}

// queue
int main()
{
  int err=0;

  err += run_regression<double>();
  err += run_regression<long double>();

  return err;
}
