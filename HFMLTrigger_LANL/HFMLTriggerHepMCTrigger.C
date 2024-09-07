#include "HFMLTriggerHepMCTrigger.h"

#include <fun4all/Fun4AllReturnCodes.h>
#include <phhepmc/PHGenIntegral.h>
#include <phool/PHCompositeNode.h>
#include <phool/PHTimeServer.h>
#include <phool/PHTimer.h>
#include <phool/getClass.h>

#include <phool/PHCompositeNode.h>

#include <pdbcalbase/PdbParameterMap.h>

#include <g4main/PHG4Hit.h>
#include <g4main/PHG4Particle.h>
#include <g4main/PHG4TruthInfoContainer.h>


#include <g4detectors/PHG4Cell.h>
#include <g4detectors/PHG4CellContainer.h>
#include <g4detectors/PHG4CylinderCellGeom.h>
#include <g4detectors/PHG4CylinderCellGeomContainer.h>
#include <g4detectors/PHG4CylinderGeom.h>
#include <g4detectors/PHG4CylinderGeomContainer.h>

//#include <trackbase_historic/SvtxCluster.h>
//#include <trackbase_historic/SvtxClusterMap.h>
//#include <trackbase_historic/SvtxHit.h>
//#include <trackbase_historic/SvtxHitMap.h>
//#include <trackbase_historic/SvtxTrack.h>
//#include <trackbase_historic/SvtxTrackMap.h>
//#include <trackbase_historic/SvtxVertex.h>
//#include <trackbase_historic/SvtxVertexMap.h>

#include <g4eval/SvtxEvalStack.h>

//#include <HepMC/GenRanges.h>
#include <ffaobjects/FlagSavev1.h>
#include <phhepmc/PHHepMCGenEvent.h>
#include <phhepmc/PHHepMCGenEventMap.h>




#include <TDatabasePDG.h>
#include <TFile.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TLorentzVector.h>
#include <TTree.h>

#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

#include <boost/format.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <cassert>
#include <cmath>
#include <cstddef>
#include <iostream>

#include <map>

using namespace std;

std::multimap<std::vector<int>, int> decaymap;

HFMLTriggerHepMCTrigger::HFMLTriggerHepMCTrigger(const std::string& moduleName,
		const std::string& filename
		, bool IsccBar
		, bool IsbbBar,
		bool IsForceD0Ki,
		bool IsIncD0)
	: SubsysReco(moduleName)
	, _ievent(0)
	, m_RejectReturnCode(Fun4AllReturnCodes::ABORTEVENT)
	, _f(nullptr)
	, _eta_min(-1)
	, _eta_max(1)
	, _embedding_id(1)
	, m_Flags(nullptr)
	, m_hNorm(nullptr)
	  , m_DRapidity(nullptr)
{
	_foutname = filename;
	//DoccBar = IsccBar;
	DobbBar = IsbbBar;
	DoccBar = IsccBar;
	ForceD0KPi = IsForceD0Ki;
	IncD0 = IsIncD0;

	cout << "INSIDE:: DoccBar = " << DoccBar << "   DobbBar = " << DobbBar << "   ForceD0KPi: " << ForceD0KPi << "    IncD0: " << IncD0  << endl;

}

int HFMLTriggerHepMCTrigger::Init(PHCompositeNode* topNode)
{
	_ievent = 0;

	cout << "INSIDE SUCK:: DoccBar = " << DoccBar  << endl;


	_f = new TFile((_foutname + string(".root")).c_str(), "RECREATE");

	m_hNorm = new TH1D("hNormalization",  //
			"Normalization;Items;Summed quantity", 10, .5, 10.5);
	int i = 1;
	m_hNorm->GetXaxis()->SetBinLabel(i++, "IntegratedLumi");
	m_hNorm->GetXaxis()->SetBinLabel(i++, "Event");
	m_hNorm->GetXaxis()->SetBinLabel(i++, "D0");
	m_hNorm->GetXaxis()->SetBinLabel(i++, "D0->PiK");
	m_hNorm->GetXaxis()->SetBinLabel(i++, "D0-Pair");
	m_hNorm->GetXaxis()->SetBinLabel(i++, "D0->PiK-Pair");
	m_hNorm->GetXaxis()->SetBinLabel(i++, "Accepted");

	m_hNorm->GetXaxis()->LabelsOption("v");

	m_DRapidity = new TH2F("hDRapidity",  //
			"hDRapidity;Rapidity of D0 meson;Accepted", 1000, -5, 5, 2, -.5, 1.5);

	return Fun4AllReturnCodes::EVENT_OK;
}

int HFMLTriggerHepMCTrigger::InitRun(PHCompositeNode* topNode)
{

	std::cout << "HFMLTriggerHepMCTrigger - Called?" << std::endl;

	m_truth_info = findNode::getClass<PHG4TruthInfoContainer>(topNode, "G4TruthInfo");
	if (!m_truth_info)
	{
		std::cout << PHWHERE << " - Fatal error - missing node G4TruthInfo" << std::endl;
		return Fun4AllReturnCodes::ABORTRUN;
	}


	PHNodeIterator iter(topNode);

	PHCompositeNode* dstNode = dynamic_cast<PHCompositeNode*>(iter.findFirst("PHCompositeNode", "DST"));
	if (!dstNode)
	{
		cout << PHWHERE << "DST Node missing, doing nothing." << std::endl;
		throw std::runtime_error(
				"Failed to find DST node in RawTowerBuilder::CreateNodes");
	}

	m_Flags = findNode::getClass<PdbParameterMap>(dstNode, "HFMLTrigger_HepMCTriggerFlags");
	if (m_Flags == nullptr)
	{
		m_Flags = new PdbParameterMap();

		dstNode->addNode(new PHDataNode<PHObject>(m_Flags, "HFMLTrigger_HepMCTriggerFlags", "PHObject"));
	}

	decaymap.insert({{-321, 211}, 0});
	decaymap.insert({{-211,321}, 1});



	return Fun4AllReturnCodes::EVENT_OK;
}

int HFMLTriggerHepMCTrigger::process_event(PHCompositeNode* topNode)   //Now it becomes HFMLG4TruthInfoTrigger :)
{


	m_truth_info = findNode::getClass<PHG4TruthInfoContainer>(topNode, "G4TruthInfo");



	bool acceptEvent = false;

	int PDGID = -1;

	assert(m_hNorm);
	m_hNorm->Fill("Event", 1);

	unsigned int nD0(0);
	unsigned int nD0PiK(0);

	std::vector<int> ParentTrkInfo;
	std::vector<std::vector<int>> DaughterInfo;
	std::vector<std::vector<float>> DaughterRapInfo;


	std::cout << "Pass 1" << std::endl;
	acceptEvent = false;



	if(DoccBar && ForceD0KPi){

		acceptEvent = false;

	}else{

		acceptEvent = true;
	}

	/*
	   PHG4TruthInfoContainer::ConstRange range = m_truth_info->GetParticleRange();
	   for (PHG4TruthInfoContainer::ConstIterator iter = range.first;
	   iter != range.second; ++iter)
	   {

	   PHG4Particle *g4particle = iter->second;
	   PDGID = g4particle->get_pid();
	   if(abs(PDGID) == 421) acceptEvent = true;

	   }






	   if(acceptEvent) std::cout << "Indeed - This event contains ccbar" << std::endl;
	   */



	PHG4TruthInfoContainer::ConstRange range = m_truth_info->GetParticleRange();
	for (PHG4TruthInfoContainer::ConstIterator iter = range.first;
			iter != range.second; ++iter)
	{

		//		float ParentE = 0;
		//		float ParentPx = 0;
		//		float ParentPy = 0;
		//		float ParentPz = 0;
		//float ParenrtPt = 0;
		int ParentTrkId = 0;

		float E;
		//		float Px;
		//		float Py;
		float Pz;
		//		float Pt;

		double rapidity;


		PHG4Particle *g4particle = iter->second;
		PHG4Particle *mother = nullptr;

		PDGID = g4particle->get_pid();
		E = g4particle->get_e();
		//Px = g4particle->get_px();
		//Py = g4particle->get_py();
		Pz = g4particle->get_pz();

		rapidity = 0.5 * TMath::Log((E+Pz)/(E-Pz));
		//		cout << "E = " << E << "   pz = " << Pz << "   rapidity = " << rapidity << endl;

		if (g4particle->get_parent_id() == 0)
		{
			ParentPDGID = 0;
		}
		else
		{
			mother = m_truth_info->GetParticle(g4particle->get_parent_id());
			ParentPDGID = mother->get_pid();

			//		ParentE = mother->get_e();
			//		ParentPx = mother->get_px();
			//		ParentPy = mother->get_py();
			//		ParentPz = mother->get_pz();
			ParentTrkId = mother->get_track_id();

		}

		//ParentPt = sqrt(ParentPx * ParentPx + ParentPy * ParentPy);

		int VtxSize = ParentTrkInfo.size();
		bool NewVtx = true;
		int Index = -1;

		for (int i = 0; i < VtxSize; i++)
		{
			if (ParentTrkId != 0 && ParentTrkId == ParentTrkInfo[i])
			{
				NewVtx = false;
				Index = i;
			}
		}

		bool VtxToQA = false;
		if(DobbBar) VtxToQA = true;
		if(DoccBar && !ForceD0KPi) VtxToQA = true;
		if(DoccBar && ForceD0KPi){
			if(abs(ParentPDGID) == 421) VtxToQA = true;
		}


		//if (DoccBar && abs(ParentPDGID) == 421) VtxToQA = true;

		//		cout << "PDGID = " << PDGID << "   ParentPDGID = " << ParentPDGID << "   rapidity = " << rapidity << endl;

		if ((ParentTrkId > 0 || abs(PDGID) == abs(ParentPDGID)) && VtxToQA == true){
			if(PDGID == 22){
				std::cout << "Extra Radiated Photons, Not count them" << std::endl;
				continue;
			}
			if (NewVtx)
			{
				ParentTrkInfo.push_back(ParentTrkId);
				std::vector<int> Daughters;
				Daughters.push_back(PDGID);
				DaughterInfo.push_back(Daughters);



				std::vector<float> DaughtersRap;
				DaughtersRap.push_back(rapidity);
				DaughterRapInfo.push_back(DaughtersRap);


				nD0 = nD0 + 1;

			}
			if (!NewVtx)
			{

				DaughterInfo[Index].push_back(PDGID);
				DaughterRapInfo[Index].push_back(rapidity);

			}

		}


		if(DoccBar && IncD0){
			if(abs(PDGID) == 421) acceptEvent = true;
		}

		if(DoccBar && !IncD0){
			if(abs(PDGID) == 421) acceptEvent = false;
		}



	}




	int VtxSizeFinal = DaughterInfo.size();



	for (int q = 0; q < VtxSizeFinal; q++){

		sort(DaughterInfo[q].begin(), DaughterInfo[q].end());
		bool RapAcc = true;
		int DaughterSize = DaughterInfo[q].size();



		for(int s = 0; s < DaughterSize; s++){

			if(abs(DaughterRapInfo[q][s]) > 1) RapAcc = false;
			//	cout << "PDGID =  " << DaughterInfo[q][s] <<  "   rapidity = " << DaughterRapInfo[q][s]  << endl;
		}

		int key = -1;
		if (decaymap.find({DaughterInfo[q]}) != decaymap.end()) key = decaymap.find({DaughterInfo[q]})->second;
		//cout << "key = " << key << "   RapAcc = " << RapAcc << endl;
		RapAcc = true; //Accept what ever we have //
		if(key > -1 && RapAcc == true){
			m_hNorm->Fill("D0->PiK", 1);
			++nD0PiK;
			//m_DRapidity->Fill(rapidity, 1);
			acceptEvent = true;			
		}

	}






	if (nD0 >= 2)
	{
		cout <<"HFMLTriggerHepMCTrigger::process_event - D0-Pair with nD0 = "<<nD0  << "   Is that fuckin accepted?  " << acceptEvent <<endl;
		m_hNorm->Fill("D0-Pair", nD0 * (nD0 - 1) / 2);
	}
	if (nD0PiK >= 2)
	{
		m_hNorm->Fill("D0->PiK-Pair", nD0PiK * (nD0PiK - 1) / 2);
	}



	

	assert(m_Flags);
	m_Flags->set_int_param(Name(), acceptEvent);
	/*
	   if (acceptEvent)
	   {
	   m_hNorm->Fill("Accepted", 1);
	   return Fun4AllReturnCodes::EVENT_OK;
	   }
	   else
	   return m_RejectReturnCode;
	   */
	if (acceptEvent)
	{
		m_hNorm->Fill("Accepted", 1);

	}


	if (acceptEvent)
	{

		return Fun4AllReturnCodes::EVENT_OK;


	}else{

		return m_RejectReturnCode;


	}

}

int HFMLTriggerHepMCTrigger::End(PHCompositeNode* topNode)
{
	PHGenIntegral* integral_node = findNode::getClass<PHGenIntegral>(topNode, "PHGenIntegral");
	if (integral_node)
	{
		assert(m_hNorm);
		m_hNorm->Fill("IntegratedLumi", integral_node->get_Integrated_Lumi());
	}

	if (_f)
	{
		_f->cd();
		_f->Write();
		//_f->Close();

		//    m_hitLayerMap->Write();
	}
	cout << "Done" << endl;

	return Fun4AllReturnCodes::EVENT_OK;
}
