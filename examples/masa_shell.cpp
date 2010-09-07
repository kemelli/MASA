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
// $Author$
// $Id$
//
// masa_shell.cpp: for users to grow familiar with the masa namespace
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//

#include <masa.h>
#include <iostream>

using namespace std;
using namespace MASA;

void masa_shell_choose_solution()
{
  char userstring[100],masterstring[100];
  void* ptr; // initialize null

  int q = 1;
  int dimension;

  double x,y,z;
  double dbl;
  double dbl2;
  double field;
  double blah;

  string str;
  
  printf("\n\n Now type solution to initialize.");
  printf("\n Remember to place underscores between parameters (e.g. steady_heatequation)");
  printf("\n\n");
  cin >> userstring;

  //masa_map_dimension(userstring,masterstring);
  masa_init("dummy",userstring);
  masa_get_name(&str);
  cout << endl << "User has selected: " << str << endl;

  // now let user register variables, etc.

  while(q>0)
    {
      printf("\n\n 0) Exit and drop currently selected manufactured solution");
      printf("\n 1) Show all variables and registered values");
      printf("\n 2) Register variable values");
      printf("\n 3) Set to default values");
      printf("\n 4) Evaluate\n\n");
      cin >> q;

      // switch statement based on user input
      switch(q)
	{
	case 0:
	  printf("\n Exiting.");
	  break;

	case 1:
	  printf("\n User Selected 1: Display all variables\n");
	  masa_display_param();
	  break;
	  
	case 2:
	  printf("\n User Selected 2: Register Variable");
	  printf("\n Input variable name:\n");
	  cin >> userstring;
	  masa_get_param(userstring,&dbl2);
	  cout << "currently set to:" << dbl2 << endl;
	  cout << "\nInput new value (double)" << endl;
	  cin >> dbl;
	  masa_set_param(userstring,dbl);
	  masa_get_param(userstring,&dbl2);
	  cout << endl << userstring << " is now set to:" << dbl2 << endl;
	  break;

	case 3:
	  printf("\n User Selected 3: Set to default values\n");
	  masa_init_param();
	  break;
	  
	case 4:
	  printf("\n User Selected 4: Evaluate");
	  masa_sanity_check();
	  masa_get_dimension(&dimension);	 
	  switch(dimension)
	    {
	    case 1:
	      cout << "\nplease input x location: \n";
	      cin >> x;
	      
	      masa_eval_t_source(x,&field);
	      cout << "source term is:" << field;
	      break;

	    case 2:
	      cout << "\nplease input x location: \n";
	      cin >> x;

	      cout << "\nplease input y location: \n";
	      cin >> y;

	      masa_eval_u_source(x,y,&field);
	      cout << "source term is:" << field;
	      break;

	    case 3:
	      cout << "\nplease input x location: \n";
	      cin >> x;

	      cout << "\nplease input y location: \n";
	      cin >> y;

	      cout << "\nplease input z location: \n";
	      cin >> z;

	      masa_eval_u_source(x,y,z,&field);
	      cout << "source term is:" << field << endl << endl;
	      break;
	      
	    default: 
	      printf("\n Error: Undefined dimension for class, please try again.\n");
	      break;

	    }//done with switch
	  break; // done with case 3
	  
	default:
	  printf("\n Error: Undefined input, please try again.\n");
	  break;
	} // end switch
     
    }

}// end masa_shell_choose_solution

void masa_shell_print_avail()
{
  // this function prints the available manufactured solutions -- 
  // obviously needs to get a handle on the manufactured solutions class here
  masa_printid();
}

int main()
{
  int q; // this defines the while loop

  // welcome message
  printf("\n\n* ----------------------------------------------------------------------------- *");
  printf("\n* Welcome to the MASA (Manufactured Analytical Solutions Abstraction) Library.");
  printf("\n* This is a software shell interface which will provide access to all");
  printf("\n* manufactured solutions provided in this library.");
  printf("\n* ----------------------------------------------------------------------------- *\n\n");

  q=1600;

  //enter shell
  while(q > 0)
    {
      
      // user chooses option

      printf("\n 1) Display Available Solutions");
      printf("\n 2) Choose and Initialize Solution");
      printf("\n Select option (0 to exit)");
      printf("\n input: ");
      cin >> q;
      
      // switch statement based on user input
      switch(q)
	{
	case 0:
	  printf("\n Exiting.");
	  break;

	case 1:
	  printf("\n User Selected 1: Display Available Solutions");
	  masa_shell_print_avail();
	  break;
	  
	case 2:
	  printf("\n User Selected 2: Choose and Initialize Solution");
	  masa_shell_choose_solution();
	  break;
	  
	default:
	  printf("\n Error: Undefined input, please try again.\n");
	  break;
	} // end switch
      
    } // end shell

  printf("\n Thank you for using the MASA shell. Have a nice day.");
  printf("\n* ----------------------------------------------------------------------------- *\n\n");
} // end main


