#ifndef MACRO_G4USER_C
#define MACRO_G4USER_C

#include <fun4all/Fun4AllServer.h>
//#include "/direct/sphenix+tg+tg01/hf/zshi/FastMLUConn/NewBuild/FastMLUConn/HFMLTriggerCodeMB/AntiTrigger/AntiTrigger.h"
#include "../../../../../AntiTrigger/AntiTrigger.h"

#include "G4_Input.C"
#include "ForceD0Setting.h"

	R__LOAD_LIBRARY(libfun4all.so)
	R__LOAD_LIBRARY(libhfmltrigger.so)
R__LOAD_LIBRARY(libantitrigger.so)

class PHG4Reco;

namespace Enable
{
// if you want this to run by default, initialize this to true
// Otherwise you have to use Enable::USER = true; in your macro
  bool USER = false;
  int USER_VERBOSITY = 0;
}

namespace G4USER
{
// here you can set parameters in your macro via
// G4USER::myparam = 1;
// add as many as you need
  int myparam = 0;
}

// This initializes the G4 part if you have a detector implemented
// You need to tell its dimensions to the surrounding black hole
void UserInit()
{
  // set the black hole dimensions surrounding the detector
  // XXX: maximum radius of your detector
  // YYY: maximum extension in z
  // ZZZ: maximum extension in -z (use -ZZZ)
  //BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, XXX);
  //BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, YYY);
  //BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, ZZZ);
}

// If you have a detector - here goes the setup
void UserDetector(PHG4Reco *g4Reco)
{
  return;
}

// Any analysis goes here (registering your module with Fun4All)
void UserAnalysisInit()
{
  Fun4AllServer* se = Fun4AllServer::instance();
  gSystem->Load("libhfmltrigger.so");
	
  if(Enable::ccBarSim == false && Enable::bbBarSim == false){

		AntiTrigger* myFinder = new AntiTrigger("myTestAntiTrigger");
		myFinder->Verbosity(1);
		std::vector<std::string> particleList = {"D0", "D+", "Ds+", "Lambdac+", "B+", "B0", "Bs0", "Lambdab0"};
		myFinder->setParticleList(particleList);
		se->registerSubsystem(myFinder);
  }

  bool IsForceD0 = ForceD0;
  bool IsIncD0 = IncD0;

	HFMLTriggerHepMCTrigger * Test = new HFMLTriggerHepMCTrigger("D0toPiKInAcceptance","Test",Enable::ccBarSim,Enable::bbBarSim,IsForceD0, IncD0);
	Test->Verbosity(0);
	se->registerSubsystem(Test);

	HFMLTriggerInterface * Test2 = new HFMLTriggerInterface("bbbarHFML");
	Test2->Verbosity(0);
	std::cout << "Right Before registerSubsystem" << std::endl;
	se->registerSubsystem(Test2);

  return;
}

#endif
