#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <HepMC/GenEvent.h>
#include <HepMC/GenVertex.h>
#pragma GCC diagnostic pop
#include <HepMC/GenRanges.h>


void TestHepMC(){

	gSystem−>Load ("libRflxHepMCdict");
	ROOT::Cintex::Cintex::Enable();
	HepMC::GenEvent * evt = new HepMC::GenEvent ;
	HepMC:: GenParticle * part = new HepMC : : GenParticle(HepMC::FourVector (10,  20, 30, 4 0 ), 9 9);
    HepMC:: GenVertex * vtx = new HepMC::GenVertex();
    vtx−>addparticleout(part);
    evt−>addvertex( vtx );
    std::string stevt = "Event1";
    const char * chevt = stevt.cstr();
    TFile * fo = new TFile("writeEvtKey.root","RECREATE");
	fo−>WriteObject(evt, chevt);

}
