#include "../interface/L1TrackTrigger_Reader.h"

L1TrackTrigger_Reader::L1TrackTrigger_Reader(TFile *a_file)
{
  std::cout << "Entering TkLayout Translator" << std::endl;


  m_OK = false;

  m_stub_pt     = new  std::vector<float>;
  m_stub_ptMC   = new  std::vector<float>; 
  m_stub_etaGEN = new  std::vector<float>; 
  m_stub_pxGEN  = new  std::vector<float>; 
  m_stub_pyGEN  = new  std::vector<float>; 
  m_stub_PHI0   = new  std::vector<float>; 
  m_stub_X0     = new  std::vector<float>; 
  m_stub_Y0     = new  std::vector<float>; 
  m_stub_Z0     = new  std::vector<float>; 
  m_stub_layer  = new  std::vector<int>; 
  m_stub_module = new  std::vector<int>;  
  m_stub_ladder = new  std::vector<int>; 
  m_stub_seg    = new  std::vector<int>;  
  m_stub_strip  = new  std::vector<int>; 
  m_stub_x      = new  std::vector<float>;  
  m_stub_y      = new  std::vector<float>;  
  m_stub_z      = new  std::vector<float>;  
  m_stub_clust1 = new  std::vector<int>;  
  m_stub_clust2 = new  std::vector<int>;  
  m_stub_deltas = new  std::vector<float>;  
  m_stub_tp     = new  std::vector<int>;  
  m_stub_pdg    = new  std::vector<int>;  

  reset();

  m_tree_L1TrackTrigger = dynamic_cast<TTree*>(a_file->Get("L1TrackTrigger"));
  
  /// Branches definition


  if (!m_tree_L1TrackTrigger)
  {
    std::cout << "This tree doesn't exist!!!" << std::endl;
    return;
  }

  m_OK = true;

  m_tree_L1TrackTrigger->SetBranchAddress("evt",            &n_tot_evt);         // Simple evt number or event ID

  m_tree_L1TrackTrigger->SetBranchAddress("STUB_n",         &m_stub);
  m_tree_L1TrackTrigger->SetBranchAddress("STUB_pt",        &m_stub_pt);
  m_tree_L1TrackTrigger->SetBranchAddress("STUB_ptMC",      &m_stub_ptMC);
  m_tree_L1TrackTrigger->SetBranchAddress("STUB_etaGEN",    &m_stub_etaGEN);
  m_tree_L1TrackTrigger->SetBranchAddress("STUB_pxGEN",     &m_stub_pxGEN);
  m_tree_L1TrackTrigger->SetBranchAddress("STUB_pyGEN",     &m_stub_pyGEN);
  m_tree_L1TrackTrigger->SetBranchAddress("STUB_layer",     &m_stub_layer);
  m_tree_L1TrackTrigger->SetBranchAddress("STUB_module",    &m_stub_module);
  m_tree_L1TrackTrigger->SetBranchAddress("STUB_ladder",    &m_stub_ladder);
  m_tree_L1TrackTrigger->SetBranchAddress("STUB_seg",       &m_stub_seg);
  m_tree_L1TrackTrigger->SetBranchAddress("STUB_strip",     &m_stub_strip);
  m_tree_L1TrackTrigger->SetBranchAddress("STUB_x",         &m_stub_x);
  m_tree_L1TrackTrigger->SetBranchAddress("STUB_y",         &m_stub_y);
  m_tree_L1TrackTrigger->SetBranchAddress("STUB_z",         &m_stub_z);
  m_tree_L1TrackTrigger->SetBranchAddress("STUB_clust1",    &m_stub_clust1);
  m_tree_L1TrackTrigger->SetBranchAddress("STUB_clust2",    &m_stub_clust2);
  m_tree_L1TrackTrigger->SetBranchAddress("STUB_deltas",    &m_stub_deltas);
  m_tree_L1TrackTrigger->SetBranchAddress("STUB_tp",        &m_stub_tp);
  m_tree_L1TrackTrigger->SetBranchAddress("STUB_pdgID",     &m_stub_pdg);
  m_tree_L1TrackTrigger->SetBranchAddress("STUB_X0",        &m_stub_X0);
  m_tree_L1TrackTrigger->SetBranchAddress("STUB_Y0",        &m_stub_Y0);
  m_tree_L1TrackTrigger->SetBranchAddress("STUB_Z0",        &m_stub_Z0);
  m_tree_L1TrackTrigger->SetBranchAddress("STUB_PHI0",      &m_stub_PHI0);

  /// Analysis settings (you define them in your python script)


  m_n_events = m_tree_L1TrackTrigger->GetEntries();

  L1TrackTrigger_Reader::reset();
}

L1TrackTrigger_Reader::~L1TrackTrigger_Reader()
{
  reset();
  delete m_stub_pt;
  delete m_stub_ptMC;
  delete m_stub_etaGEN;
  delete m_stub_pxGEN;
  delete m_stub_pyGEN;
  delete m_stub_PHI0;
  delete m_stub_X0;
  delete m_stub_Y0;
  delete m_stub_Z0;
  delete m_stub_layer;
  delete m_stub_module;
  delete m_stub_ladder;
  delete m_stub_seg;
  delete m_stub_strip;
  delete m_stub_x;
  delete m_stub_y;
  delete m_stub_z;
  delete m_stub_clust1;
  delete m_stub_clust2;
  delete m_stub_deltas;
  delete m_stub_tp;
  delete m_stub_pdg;
}

void L1TrackTrigger_Reader::reset()
{
  for(unsigned int i=0;i<m_hits.size();i++){
    delete m_hits[i];
  }
  m_hits.clear();

  m_stub = 0;
  
  m_stub_X0->clear();     
  m_stub_Y0->clear();     
  m_stub_Z0->clear();     
  m_stub_PHI0->clear();     
  m_stub_tp->clear();     
  m_stub_pt->clear();     
  m_stub_ptMC->clear();   
  m_stub_etaGEN->clear();  
  m_stub_pxGEN->clear();  
  m_stub_pyGEN->clear();  
  m_stub_layer->clear();  
  m_stub_module->clear(); 
  m_stub_ladder->clear(); 
  m_stub_seg->clear();    
  m_stub_strip->clear(); 
  m_stub_x->clear(); 
  m_stub_y->clear(); 
  m_stub_z->clear(); 
  m_stub_clust1->clear(); 
  m_stub_clust2->clear(); 
  m_stub_deltas->clear(); 
  m_stub_pdg->clear();
  
}

//
// Method getting the info from an input file
//

void L1TrackTrigger_Reader::getInfo(int ievt) 
{
  m_tree_L1TrackTrigger->GetEntry(ievt); 
  
  for(int i=0;i<m_stub;i++){
    int layer = m_stub_layer->at(i);
    int module = -1;
    module = CMSPatternLayer::getModuleCode(layer, m_stub_module->at(i));
    if(module<0)// the stub is on the third Z position on the other side of the tracker -> out of range
      continue;
    int ladder = CMSPatternLayer::getLadderCode(layer, m_stub_ladder->at(i));
    int segment =  CMSPatternLayer::getSegmentCode(layer, ladder, m_stub_seg->at(i));
    if(segment<0 || segment>1){
      cout<<"Invalid segment on event "<<m_n_events<<endl;
      continue;
    }
    int strip = m_stub_strip->at(i);
    int tp = m_stub_tp->at(i);
    float eta = m_stub_etaGEN->at(i);
    float phi0 = m_stub_PHI0->at(i);
    float spt = sqrt(m_stub_pxGEN->at(i)*m_stub_pxGEN->at(i)+m_stub_pyGEN->at(i)*m_stub_pyGEN->at(i));
    float x = m_stub_x->at(i);
    float y = m_stub_y->at(i);
    float z = m_stub_z->at(i);
    float x0 = m_stub_X0->at(i);
    float y0 = m_stub_Y0->at(i);
    float z0 = m_stub_Z0->at(i);
    
    //cout<<layer<<" "<<module<<" "<<ladder<<" "<<segment<<" "<<strip<<endl;
    
    float ip = sqrt(m_stub_X0->at(i)*m_stub_X0->at(i)+m_stub_Y0->at(i)*m_stub_Y0->at(i));
    
    Hit* h = new Hit(layer,ladder, module, segment, strip, i, tp, spt, ip, eta, phi0, x, y, z, x0, y0, z0);
    m_hits.push_back(h);
  }
  
}

vector<Hit*>&  L1TrackTrigger_Reader::getHits(){
  return m_hits;
}
