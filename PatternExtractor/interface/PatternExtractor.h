#ifndef PatternExtractor_h
#define PatternExtractor_h

// -*- C++ -*-
//
// Package:    PatternExtractor
// Class:      PatternExtractor
// 
//
// Original Author:  Guillaume Baulieu
//         Created:  Thu Jun 13 09:35:13 CEST 2013
// $Id$
//
//


// system include files
#include <memory>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "../interface/L1TrackTrigger_Reader.h"
#include "../interface/PatternFinder.h"
#include "../interface/SectorTree.h"
#include "../interface/Hit.h"

#include "TFile.h"
#include "TRFIOFile.h"

#ifndef __APPLE__
BOOST_CLASS_EXPORT_IMPLEMENT(CMSPatternLayer) 
#endif


//
// class declaration
//

class PatternExtractor : public edm::EDAnalyzer {
public:
  PatternExtractor(const edm::ParameterSet&);
  ~PatternExtractor();
  
  void beginJob();
  void endJob();

  
  void beginRun(edm::Run const&, edm::EventSetup const&);
  void endRun(edm::Run const&, edm::EventSetup const&);
  
  /// Method called once per event
  virtual void analyze(const edm::Event&, const edm::EventSetup& );

  void retrieve() ;
  void initialize() ;
  void reset();
  void doAna();
  void getInfo(int ievent);

private:

  // ----------member data ---------------------------
  SectorTree              m_st;
  PatternFinder           *m_pf;
  TFile                   *m_infile;
  L1TrackTrigger_Reader   *m_TK;
  TTree                   *m_tree_L1PatternReco;
  TFile                   *m_outfile;

  std::string inFilename_;
  std::string outFilename_;
  std::string inBankname_;
  int  threshold_;
  int  nevts_;
  int  skip_;
  int  keepID_;

  bool do_TK_;
  int nevent_tot;
  int nevent;
  int m_patt;
  
  std::vector<int> stub_IDs;

  std::vector<int>                 *m_patt_secid;
  std::vector< std::vector<int> >  *m_patt_links;
};



#endif
