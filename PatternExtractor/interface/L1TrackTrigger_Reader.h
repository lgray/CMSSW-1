#ifndef L1TRACKTRIGGER_READER_H
#define L1TRACKTRIGGER_READER_H

/*****************************

Simple example class showing how to perform an 
analysis using the PatExtractor tools

S.Viret (viret@in2p3.fr): 31/05/11

More info: http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.PHYTuto

 ******************************/

//Include std C++
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

// ROOT stuff
#include "TMath.h"
#include "TTree.h"
#include "TFile.h"

#include "Hit.h"
#include "CMSPatternLayer.h"

class L1TrackTrigger_Reader
{
 public:
  L1TrackTrigger_Reader(TFile *a_file);

  ~L1TrackTrigger_Reader();
  
  //Selection
  int  n_events() {return m_n_events;}
  void getInfo(int ievt); 

  vector<Hit*>& getHits();

  void reset();

  bool isOK() {return m_OK;}
 private:

  TTree* m_tree_L1TrackTrigger;

  // Input file info
 
  vector<Hit*> m_hits;

  bool m_OK;
  int  m_n_events;
  int n_tot_evt;
  int m_stub;
  
  std::vector<float>  *m_stub_pt;
  std::vector<float>  *m_stub_ptMC;
  std::vector<float>  *m_stub_pxGEN;
  std::vector<float>  *m_stub_pyGEN;
  std::vector<float>  *m_stub_etaGEN;
  std::vector<float>  *m_stub_X0;
  std::vector<float>  *m_stub_Y0;
  std::vector<float>  *m_stub_Z0;
  std::vector<float>  *m_stub_PHI0;
  std::vector<int>    *m_stub_disk;
  std::vector<int>    *m_stub_layer;
  std::vector<int>    *m_stub_module;
  std::vector<int>    *m_stub_ladder;
  std::vector<int>    *m_stub_seg;
  std::vector<int>    *m_stub_strip;
  std::vector<float>  *m_stub_x;
  std::vector<float>  *m_stub_y;
  std::vector<float>  *m_stub_z;
  std::vector<int>    *m_stub_clust1;
  std::vector<int>    *m_stub_clust2;
  std::vector<float>  *m_stub_deltas;
  std::vector<int>    *m_stub_tp;
  std::vector<int>    *m_stub_pdg;


};

#endif 
