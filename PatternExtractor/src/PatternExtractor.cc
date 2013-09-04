// -*- C++ -*-
//
// Package:    PatternExtractor
// Class:      PatternExtractor
// 
/**\class PatternExtractor PatternExtractor.cc AM/PatternExtractor/src/PatternExtractor.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Guillaume Baulieu
//         Created:  Thu Jun 13 09:35:13 CEST 2013
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "../interface/L1TrackTrigger_Reader.h"

#include "PatternTree.h"
#include "PatternGenerator.h"
#include "PatternFinder.h"
#include "SectorTree.h"
#include "Detector.h"
#include "PrincipalTrackFitter.h"
#include "PrincipalFitGenerator.h"

#ifndef __APPLE__
BOOST_CLASS_EXPORT_IMPLEMENT(CMSPatternLayer) 
BOOST_CLASS_EXPORT_IMPLEMENT(PrincipalTrackFitter) 
#endif

using namespace std;

//
// class declaration
//

class PatternExtractor : public edm::EDAnalyzer {
public:
  explicit PatternExtractor(const edm::ParameterSet&);
  ~PatternExtractor();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  void retrieve() ;
  void reset();
  void doAna();
  void getInfo(int ievent);
  
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  //virtual void endRun(edm::Run const&, edm::EventSetup const&);
  //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  
  // ----------member data ---------------------------
  SectorTree m_st;
  PatternFinder *m_pf;
  TFile* m_infile;
  L1TrackTrigger_Reader*      m_TK;
  
  bool use_file_;
  std::string inFilename_;
  std::string inBankname_;
  int threshold_;
  bool do_TK_;
  int nevent_tot;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
PatternExtractor::PatternExtractor(const edm::ParameterSet& iConfig):
  use_file_       (iConfig.getUntrackedParameter<bool>("useFile", true)),
  inFilename_    (iConfig.getParameter<std::string>("inputRootFile")),
  inBankname_    (iConfig.getParameter<std::string>("inputBankFile")),
  threshold_    (iConfig.getParameter<int>("threshold")),
  do_TK_         (iConfig.getUntrackedParameter<bool>("doTranslation", false))
{
   //now do what ever initialization is needed

}


PatternExtractor::~PatternExtractor()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
PatternExtractor::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;



#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
PatternExtractor::beginJob()
{
  std::cout << "Enter BeginJob" << std::endl;
  
  // If do_fill is set to True, you extract the whole data, otherwise you start 
  // from a file already extracted (inFilename_)

  cout<<"Loading pattern bank..."<<endl;
  {
    std::ifstream ifs(inBankname_.c_str());
    boost::archive::text_iarchive ia(ifs);
    ia >> m_st;
  }
  
  m_pf = new PatternFinder(m_st.getSuperStripSize(), threshold_, &m_st, "", "");

  if(use_file_) 
    PatternExtractor::retrieve();

  std::cout << "End BeginJob" << std::endl;
  if(do_TK_)
    cout<<m_TK->n_events()<<" events found"<<endl;
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PatternExtractor::endJob() 
{ 

  delete(m_pf);
  
  //std::cout << "Total # of events for this job   = "<< nevent_tot     << std::endl;

  if (!use_file_) 
  {
    //m_outfile->Write();
    //m_outfile->Close();
  }
  else
  {
    m_infile->Close();
    //m_outfile->Write();
    //m_outfile->Close();
  }
}

void PatternExtractor::retrieve() 
{
  m_infile     = m_infile->Open(inFilename_.c_str());
  
  m_TK         = new L1TrackTrigger_Reader(m_infile);
  
  // We set some variables wrt the info retrieved 
  // (if the tree is not there, don't go further...)  
  
  do_TK_       = m_TK->isOK();

}

// ------------ method called when starting to processes a run  ------------

void PatternExtractor::beginRun(edm::Run const&, edm::EventSetup const&){
  
  std::cout << "Enter BeginRun" << std::endl;
  
  if (use_file_ && do_TK_ && m_TK->n_events()){    
    // If you start from an extracted file, the number of events you want to loop on
    // is defined as an option, not in CMSSW...
    
    for (int i=0;i<m_TK->n_events();++i){
      if (i%1 == 0)
	std::cout << "Processing " << i << "th event" << std::endl;

      PatternExtractor::getInfo(i);// Retrieve the info from an existing ROOTuple      
      PatternExtractor::doAna();   // Then do the analysis on request  
      PatternExtractor::reset();// Free memory for this event

      ++nevent_tot; 
    }
  }

  std::cout << "Exit BeginRun" << std::endl;
  
}

void PatternExtractor::getInfo(int ievent) 
{
  if (do_TK_)       m_TK->getInfo(ievent);
}

void PatternExtractor::reset() 
{
  if (do_TK_)       m_TK->reset();
}

void PatternExtractor::doAna() 
{
  if(do_TK_){
    vector<Sector*> patternsSectors = m_pf->find(m_TK->getHits());
    for(unsigned int i=0;i<patternsSectors.size();i++){
      vector<GradedPattern*> pl = patternsSectors[i]->getPatternTree()->getLDPatterns();
      cout<<"Found "<<pl.size()<<" patterns in sector "<<patternsSectors[i]->getOfficialID()<<endl;
      //delete the GradedPattern objects
      for(unsigned j=0;j<pl.size();j++){
	delete pl[j];
      }
      //delete the Sectors
      delete patternsSectors[i];
    }
  }
}



// ------------ method called when ending the processing of a run  ------------
/*
void 
PatternExtractor::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
PatternExtractor::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
PatternExtractor::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
PatternExtractor::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(PatternExtractor);
