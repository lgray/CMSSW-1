#include "../interface/PatternExtractor.h"

using namespace std;
using namespace edm;


PatternExtractor::PatternExtractor(const edm::ParameterSet& iConfig):
  inFilename_     (iConfig.getParameter<std::string>("inputRootFile")),
  outFilename_    (iConfig.getParameter<std::string>("outputRootFile")),
  inBankname_     (iConfig.getParameter<std::string>("inputBankFile")),
  threshold_      (iConfig.getParameter<int>("threshold")),
  nevts_          (iConfig.getUntrackedParameter<int>("n_events",       10000)),
  skip_           (iConfig.getUntrackedParameter<int>("skip_events",    0)),
  keepID_         (iConfig.getUntrackedParameter<int>("keepID",    0))
{
   //now do what ever initialization is needed

  PatternExtractor::initialize();

}


PatternExtractor::~PatternExtractor()
{}



void PatternExtractor::beginJob()
{
  // First of all we load the pattern bank file
  // this is done once

  cout << "Loading pattern bank file : " << endl;
  cout << inBankname_ << endl;

  std::ifstream ifs(inBankname_.c_str());
  boost::archive::text_iarchive ia(ifs);
  ia >> m_st;
  
  // Then we create the pattern reco object  

  m_pf = new PatternFinder(m_st.getSuperStripSize(), threshold_, &m_st, "", "");

  // And retrieve info from the input file
  cout << "Opening input file" << endl;
  PatternExtractor::retrieve();

  if (do_TK_) cout << m_TK->n_events() << " events found in the input file" << endl;
}

// ------------ method called once each job just after ending the event loop  ------------
void PatternExtractor::endJob() 
{ 
  delete(m_pf);
  
  std::cout << "Total # of events treated for this job   = "<< nevent_tot     << std::endl;

  m_outfile->Write();
  m_outfile->Close();
  m_infile->Close();
}


// ------------ method called for each event  ------------
void PatternExtractor::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
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

void PatternExtractor::beginRun(edm::Run const&, edm::EventSetup const&)
{
  nevent = 0;

  std::cout << "Enter BeginRun" << std::endl;
  
  if (do_TK_ && m_TK->n_events())
  {    
    nevent = min(skip_+nevts_,m_TK->n_events()); 

    for (int i=skip_;i<nevent;++i)
    {
      if (i%1000 == 0)
	std::cout << "Processing " << i << "th event" << std::endl;

      PatternExtractor::reset();   // Free memory for this event
      PatternExtractor::getInfo(i);// Retrieve the info from an existing ROOTuple      
      PatternExtractor::doAna();   // Then do the analysis on request  

      (keepID_==1)
	? nevent_tot = m_TK->event_num() // Do we use the event id or not?
	: nevent_tot = i; 
    }
  }

  std::cout << "Exit BeginRun" << std::endl;
  
}


void PatternExtractor::doAna() 
{  
  if(!do_TK_) return;
  
  vector<Sector*> patternsSectors = m_pf->find(m_TK->getHits());
  vector<Hit*> hits; 

  for(unsigned int i=0;i<patternsSectors.size();i++)
  {      
    vector<GradedPattern*> pl = patternsSectors[i]->getPatternTree()->getLDPatterns();

    if (pl.size()==0) continue;
      
    //      cout<<"Found "<<pl.size()<<" patterns"<<endl;
      
    m_patt = pl.size();

    int secID = patternsSectors[i]->getOfficialID();

    //delete the GradedPattern objects
    for(unsigned j=0;j<pl.size();j++) 
    {
      m_patt_secid->push_back(secID);

      hits.clear();
      stub_IDs.clear();

      hits = pl[j]->getHits();

      for(unsigned k=0;k<hits.size();k++) 
      {
	stub_IDs.push_back(hits[k]->getID());
      }
   
      //      for(unsigned k=0;k<hits.size();k++) delete hits[k]; 

      m_patt_links->push_back(stub_IDs);
      
      delete pl[j];
    }

    //delete the Sectors
    delete patternsSectors[i];

    m_tree_L1PatternReco->Fill(); 
  }
}


// ------------ method called when ending the processing of a run  ------------

void PatternExtractor::endRun(edm::Run const&, edm::EventSetup const&)
{
  std::cout << "Total # of events for this run   = "<< nevent  << std::endl;
}

//
// In this method one initialize all the infos which will be stored in
// the final data ntuple
//

void PatternExtractor::initialize()
{  
  nevent_tot    = 0;

  m_outfile     = new TFile(outFilename_.c_str(),"RECREATE");
  m_patt_links  = new  std::vector< std::vector<int> >; 
  m_patt_secid  = new  std::vector<int>; 


  // Then create the rootuple

  m_tree_L1PatternReco = new TTree("L1PatternReco","L1PatternReco Analysis info");  
  
  /// Branches definition

  m_tree_L1PatternReco->Branch("evt",            &nevent_tot); // Simple evt number or event ID
  m_tree_L1PatternReco->Branch("PATT_n",         &m_patt);
  m_tree_L1PatternReco->Branch("PATT_links",     &m_patt_links);
  m_tree_L1PatternReco->Branch("PATT_secID",     &m_patt_secid);
}


void PatternExtractor::getInfo(int ievent) 
{
  if (do_TK_)  m_TK->reset();
  if (do_TK_)  m_TK->getInfo(ievent);
}

void PatternExtractor::reset() 
{
  m_patt = 0;
  m_patt_links->clear();
  m_patt_secid->clear();
}

DEFINE_FWK_MODULE(PatternExtractor);
