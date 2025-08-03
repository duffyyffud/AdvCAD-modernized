/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for coverage.cc */

#pragma once
#ifndef WH_INCLUDED_WH_COMMON
#include <WH/common.h>
#define WH_INCLUDED_WH_COMMON
#endif

class WH_CVR_Record;
class WH_CVR_CoverageMonitor;
class WH_CVR_Line;

/* value-based class */
/* heavy weight */
class WH_CVR_Record {
 public:
  WH_CVR_Record 
    (int lineId, bool isExecuted);
  virtual ~WH_CVR_Record ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  int lineId () const;
  
  bool isExecuted () const;
  
  virtual void execute ();
  
  /* derived */
  
 protected:
  int _lineId;
  /* starting from 1 */
  
  bool _isExecuted;

  /* no implementation */
  WH_CVR_Record (const WH_CVR_Record& record);
  const WH_CVR_Record& operator= (const WH_CVR_Record& record);

  /* base */
  
  /* derived */
  
};

class WH_CVR_CoverageMonitor {
 public:
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */

  /* singleton pattern */
  static WH_CVR_CoverageMonitor* coverageMonitor ();

  static void end ();
  /* this must be called just before test execution terminates */
  
  virtual WH_CVR_Record* getRecord 
    (const char* fileNameCstr, int lineId);

  /* derived */
  
 protected:
  static WH_CVR_CoverageMonitor* _coverageMonitor;

  map<string, vector<WH_CVR_Record*>*> _recordSetMap;  /* own */

  WH_CVR_CoverageMonitor ();
  virtual ~WH_CVR_CoverageMonitor ();
  
  /* base */

  virtual void loadDataBase ();

  virtual void saveDataBase ();

  /* derived */
  
};

/* value-based class */
/* heavy weight */
class WH_CVR_Line {
 public:
  WH_CVR_Line 
    (const char* fileNameCstr, int lineId);
  
  /* base */
  WH_CVR_Record* record () const;
  
  virtual void execute ();
  
  /* derived */
  
 protected:
  WH_CVR_Record* _record;

  /* no implementation */
  WH_CVR_Line (const WH_CVR_Line& line);
  const WH_CVR_Line& operator= (const WH_CVR_Line& line);
  
  /* base */
  
  /* derived */
  
};

#ifdef WH_COVERAGE_ENABLED

#define WH_CVR_LINE \
{ \
static WH_CVR_Line cvrLine (__FILE__, __LINE__);  \
cvrLine.execute (); \
}

#else

#define WH_CVR_LINE 

#endif

