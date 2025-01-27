#include <iomanip>

#include "MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias.hh"

#include "Geant4/globals.hh"
#include "Geant4/G4ios.hh"
#include "Geant4/G4SystemOfUnits.hh"
#include "Geant4/G4ParticleDefinition.hh"
#include "Geant4/G4ParticleTable.hh"

#include "Geant4/G4MesonConstructor.hh"
#include "Geant4/G4BaryonConstructor.hh"
#include "Geant4/G4ShortLivedConstructor.hh"

#include "Geant4/G4IonConstructor.hh"

#include "Geant4/G4ComponentGGHadronNucleusXsc.hh"
#include "Geant4/G4CrossSectionInelastic.hh"
#include "Geant4/G4HadronCaptureProcess.hh"
#include "Geant4/G4NeutronRadCapture.hh"
#include "Geant4/G4NeutronCaptureXS.hh"
#include "Geant4/G4ParticleHPCaptureData.hh"
#include "Geant4/G4LFission.hh"

#include "Geant4/G4CrossSectionDataSetRegistry.hh"

#include "Geant4/G4PhysListUtil.hh"

// factoryGeant4/
#include "Geant4/G4PhysicsConstructorFactory.hh"
//
G4_DECLARE_PHYSCONSTR_FACTORY(MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias);

G4ThreadLocal MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias::ThreadPrivate*
MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias::tpdata = 0;

MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias::MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias(G4int)
    :  G4VPhysicsConstructor("hInelastic MyQGSP_BERT_HP_NeutronXSBias")
/*  , theNeutrons(0)
    , theFTFPNeutron(0)
    , theQGSPNeutron(0)
    , theBertiniNeutron(0)
    , theHPNeutron(0)
    , thePiK(0)
    , theFTFPPiK(0)
    , theQGSPPiK(0)
    , theBertiniPiK(0)
    , thePro(0)
    , theFTFPPro(0)
    , theQGSPPro(0)
    , theBertiniPro(0)
    , theHyperon(0)
    , theAntiBaryon(0)
    , theFTFPAntiBaryon(0)
    , xsKaon(0)
    , xsNeutronCaptureXS(0)*/
//    , QuasiElastic(true)
{}

MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias::MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias(const G4String& name, G4bool /*quasiElastic */ )
    :  G4VPhysicsConstructor(name)
/*  , theNeutrons(0)
    , theFTFPNeutron(0)
    , theQGSPNeutron(0)
    , theBertiniNeutron(0)
    , theHPNeutron(0)
    , thePiK(0)
    , theFTFPPiK(0)
    , theQGSPPiK(0)
    , theBertiniPiK(0)
    , thePro(0)
    , theFTFPPro(0)
    , theQGSPPro(0)
    , theBertiniPro(0)
    , theHyperon(0)
    , theAntiBaryon(0)
    , theFTFPAntiBaryon(0)
    , xsKaon(0)
    , xsNeutronCaptureXS(0)*/
//    , QuasiElastic(quasiElastic)
{}

MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias::MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias(const G4String& name, G4bool /*quasiElastic */, G4double XSMultiplier )
    :  G4VPhysicsConstructor(name)
/*  , theNeutrons(0)
    , theFTFPNeutron(0)
    , theQGSPNeutron(0)
    , theBertiniNeutron(0)
    , theHPNeutron(0)
    , thePiK(0)
    , theFTFPPiK(0)
    , theQGSPPiK(0)
    , theBertiniPiK(0)
    , thePro(0)
    , theFTFPPro(0)
    , theQGSPPro(0)
    , theBertiniPro(0)
    , theHyperon(0)
    , theAntiBaryon(0)
    , theFTFPAntiBaryon(0)
    , xsKaon(0)
    , xsNeutronCaptureXS(0)*/
//    , QuasiElastic(quasiElastic)
    , theBertiniNeutronXSMultiplier(XSMultiplier)
{}

void MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias::CreateModels()
{
  G4bool quasiElasticFTF= false;   // Use built-in quasi-elastic (not add-on)
  G4bool quasiElasticQGS= true;    // For QGS, it must use it.

  const G4double minQGSP = 12.0*GeV;
  const G4double maxFTFP = 25.0*GeV;
  const G4double minFTFP =  9.5*GeV;
  const G4double maxBERT =  9.9*GeV;
  const G4double maxHP   = 19.9*MeV;

  tpdata->theNeutrons=new G4NeutronBuilder( true ); // Fission on
  tpdata->theNeutrons->RegisterMe(tpdata->theQGSPNeutron=new G4QGSPNeutronBuilder(quasiElasticQGS));
  tpdata->theQGSPNeutron->SetMinEnergy(minQGSP);
  tpdata->theNeutrons->RegisterMe(tpdata->theFTFPNeutron=new G4FTFPNeutronBuilder(quasiElasticFTF));
  tpdata->theFTFPNeutron->SetMinEnergy(minFTFP);
  tpdata->theFTFPNeutron->SetMaxEnergy(maxFTFP);

  tpdata->theNeutrons->RegisterMe(tpdata->theBertiniNeutron=new G4BertiniNeutronBuilder);
  //tpdata->theNeutrons->RegisterMe(tpdata->theBertiniNeutron=new MyG4BertiniNeutronBuilder);
  //tpdata->theNeutrons->RegisterMe(tpdata->theBertiniNeutron=new MyG4BertiniNeutronBuilder(theBertiniNeutronXSMultiplier) );
  tpdata->theBertiniNeutron->SetMinEnergy(maxHP);
  tpdata->theBertiniNeutron->SetMaxEnergy(maxBERT);

  tpdata->theNeutrons->RegisterMe(tpdata->theHPNeutron=new G4NeutronPHPBuilder);

  tpdata->thePro=new G4ProtonBuilder;
  tpdata->thePro->RegisterMe(tpdata->theQGSPPro=new G4QGSPProtonBuilder(quasiElasticQGS));
  tpdata->theQGSPPro->SetMinEnergy(minQGSP);
  tpdata->thePro->RegisterMe(tpdata->theFTFPPro=new G4FTFPProtonBuilder(quasiElasticFTF));
  tpdata->theFTFPPro->SetMinEnergy(minFTFP);
  tpdata->theFTFPPro->SetMaxEnergy(maxFTFP);
  tpdata->thePro->RegisterMe(tpdata->theBertiniPro=new G4BertiniProtonBuilder);
  tpdata->theBertiniPro->SetMaxEnergy(maxBERT);

  tpdata->thePiK=new G4PiKBuilder;
  tpdata->thePiK->RegisterMe(tpdata->theQGSPPiK=new G4QGSPPiKBuilder(quasiElasticQGS));
  tpdata->theQGSPPiK->SetMinEnergy(minQGSP);
  tpdata->thePiK->RegisterMe(tpdata->theFTFPPiK=new G4FTFPPiKBuilder(quasiElasticFTF));
  tpdata->theFTFPPiK->SetMinEnergy(minFTFP);
  tpdata->theFTFPPiK->SetMaxEnergy(maxFTFP);
  tpdata->thePiK->RegisterMe(tpdata->theBertiniPiK=new G4BertiniPiKBuilder);
  tpdata->theBertiniPiK->SetMaxEnergy(maxBERT);

  tpdata->theHyperon=new G4HyperonFTFPBuilder;

  tpdata->theAntiBaryon=new G4AntiBarionBuilder;
  tpdata->theAntiBaryon->RegisterMe(tpdata->theFTFPAntiBaryon=new G4FTFPAntiBarionBuilder(quasiElasticFTF));
}

MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias::~MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias()
{
  if (!tpdata) return;

   delete tpdata->theHPNeutron;
   delete tpdata->theBertiniNeutron;
   delete tpdata->theQGSPNeutron;
   delete tpdata->theFTFPNeutron;
   delete tpdata->theNeutrons;
   delete tpdata->theBertiniPiK;
   delete tpdata->theQGSPPiK;
   delete tpdata->theFTFPPiK;
   delete tpdata->thePiK;
   delete tpdata->theBertiniPro;
   delete tpdata->theQGSPPro;
   delete tpdata->theFTFPPro;
   delete tpdata->thePro;
   delete tpdata->theFTFPAntiBaryon;
   delete tpdata->theAntiBaryon;
   delete tpdata->theHyperon;

   delete tpdata; tpdata = 0;
}

void MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias::ConstructParticle()
{
  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();

  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();
}

#include "Geant4/G4ProcessManager.hh"
void MyG4HadronPhysicsQGSP_BERT_HP_NeutronXSBias::ConstructProcess()
{
  if ( tpdata == 0 ) tpdata = new ThreadPrivate;
  CreateModels();

  tpdata->theNeutrons->Build();
  // -- Retrieve and multiply the Bertini Neutron's Inelastic XS by the factor
  G4PhysListUtil::FindInelasticProcess(G4Neutron::Neutron())->MultiplyCrossSectionBy(theBertiniNeutronXSMultiplier);
  tpdata->thePro->Build();
  tpdata->thePiK->Build();

  // --- Kaons ---
  tpdata->xsKaon = new G4ComponentGGHadronNucleusXsc();
  G4VCrossSectionDataSet * kaonxs = new G4CrossSectionInelastic(tpdata->xsKaon);
  G4PhysListUtil::FindInelasticProcess(G4KaonMinus::KaonMinus())->AddDataSet(kaonxs);
  G4PhysListUtil::FindInelasticProcess(G4KaonPlus::KaonPlus())->AddDataSet(kaonxs);
  G4PhysListUtil::FindInelasticProcess(G4KaonZeroShort::KaonZeroShort())->AddDataSet(kaonxs);
  G4PhysListUtil::FindInelasticProcess(G4KaonZeroLong::KaonZeroLong())->AddDataSet(kaonxs);

  tpdata->theHyperon->Build();
  tpdata->theAntiBaryon->Build();

  // --- Neutrons ---
  G4HadronicProcess* capture = 0;
  G4HadronicProcess* fission = 0;
  G4ProcessManager* pmanager = G4Neutron::Neutron()->GetProcessManager();
  G4ProcessVector*  pv = pmanager->GetProcessList();
  for ( size_t i=0; i < static_cast<size_t>(pv->size()); ++i ) {
    if ( fCapture == ((*pv)[i])->GetProcessSubType() ) {
      capture = static_cast<G4HadronicProcess*>((*pv)[i]);
    } else if ( fFission == ((*pv)[i])->GetProcessSubType() ) {
      fission = static_cast<G4HadronicProcess*>((*pv)[i]);
    }
  }
  if ( ! capture ) {
    capture = new G4HadronCaptureProcess("nCapture");
    pmanager->AddDiscreteProcess(capture);
  }
  tpdata->xsNeutronCaptureXS = (G4NeutronCaptureXS*)G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4NeutronCaptureXS::Default_Name());
  capture->AddDataSet(tpdata->xsNeutronCaptureXS);
  capture->AddDataSet( new G4ParticleHPCaptureData );
  G4NeutronRadCapture* theNeutronRadCapture = new G4NeutronRadCapture();
  theNeutronRadCapture->SetMinEnergy( 19.9*MeV );
  capture->RegisterMe( theNeutronRadCapture );
  if ( ! fission ) {
    fission = new G4HadronFissionProcess("nFission");
    pmanager->AddDiscreteProcess(fission);
  }
  G4LFission* theNeutronLEPFission = new G4LFission();
  theNeutronLEPFission->SetMinEnergy( 19.9*MeV );
  fission->RegisterMe( theNeutronLEPFission );
}

