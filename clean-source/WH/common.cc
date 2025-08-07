/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* common.cc */

#include "common.h"



void WH_Assert 
(const char* expr, 
 const char* file, int line)
{
  cout << flush;
  cerr << endl 
       << "ASSERTION \"" << expr
       << "\" failed: file \"" << file
       << "\", line " << line 
       << endl; 
  cerr << flush;

  /* this statement should cause segmentation fault, and it is trapped
     by the debugger. */
  *((int*)0) = 1;
}

void WH_AssertMsg 
(const char* expr, 
 const char* file, int line, 
 const char* msg)
{
  cout << flush;
  cerr << endl 
       << "ASSERTION \"" << expr
       << "\" failed for \"" << msg 
       << "\": file \"" << file
       << "\", line " << line 
       << endl; 
  cerr << flush;

  /* this statement should cause segmentation fault, and it is trapped
     by the debugger. */
  *((int*)0) = 1;
}

