#include "HFMLTriggerInterface.h"

#include <fun4all/Fun4AllReturnCodes.h>
#include <pdbcalbase/PdbParameterMap.h>
#include <phool/PHCompositeNode.h>
#include <phool/PHTimeServer.h>
#include <phool/PHTimer.h>
#include <phool/getClass.h>

#include <phool/PHCompositeNode.h>

#include <g4main/PHG4Hit.h>
#include <g4main/PHG4Particle.h>
#include <g4main/PHG4HitContainer.h>
#include <g4main/PHG4TruthInfoContainer.h>

#include <trackbase/TrkrHitTruthAssocv1.h>
#include <trackbase/TrkrHitSetContainer.h>
//#include <trackbase/TrkrClusterHitAssoc.h>
#include <trackbase/TrkrHitSet.h>
#include <trackbase/TrkrHit.h>
//#include <trackbase/TrkrCluster.h>

#include <trackbase/MvtxDefs.h>
#include <mvtx/CylinderGeom_Mvtx.h>

#include <trackbase/InttDefs.h>
#include <intt/CylinderGeomIntt.h>

#include <g4detectors/PHG4Cell.h>
#include <g4detectors/PHG4CellContainer.h>
#include <g4detectors/PHG4CylinderCellGeom.h>
#include <g4detectors/PHG4CylinderCellGeomContainer.h>
#include <g4detectors/PHG4CylinderGeom.h>
#include <g4detectors/PHG4CylinderGeomContainer.h>

//#include <g4eval/SvtxEvalStack.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <HepMC/GenEvent.h>
#include <HepMC/GenVertex.h>
#pragma GCC diagnostic pop
#include <phhepmc/PHHepMCGenEvent.h>
#include <phhepmc/PHHepMCGenEventMap.h>

#include <TDatabasePDG.h>
#include <TFile.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TLorentzVector.h>
#include <TString.h>
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
#include <trackbase/ActsSurfaceMaps.h>

using namespace std;

HFMLTriggerInterface::HFMLTriggerInterface(std::string filename)
	: SubsysReco("HFMLTriggerInterface")
	, _ievent(0)
	, _f(nullptr)
	, _eta_min(-1)
	, _eta_max(1)
	, _embedding_id(1)
	, _nlayers_maps(3)
	, m_hitsets(nullptr)
	, m_GenEventMap(nullptr)
	, m_truthInfo(nullptr)
	  , m_g4hits_mvtx(nullptr)
	  //	, _svtxevalstack(nullptr)
	, m_hit_truth_map(nullptr)
	, m_Flags(nullptr)
	, m_Geoms(nullptr)
	, m_hitStaveLayer(nullptr)
	, m_hitModuleHalfStave(nullptr)
	, m_hitChipModule(nullptr)
	, m_hitLayerMap(nullptr)
	, m_hitPixelPhiMap(nullptr)
	, m_hitPixelPhiMapHL(nullptr)
	  , m_hitPixelZMap(nullptr)
{
	_foutname = filename;
	cout << "HFMLTriggerInterface - OK" << endl;
}

TFile * DebugFile;
TTree *RawHitTree;
TTree * TruthTrackTree;



std::vector<float> MVTXHitX;
std::vector<float> MVTXHitY;
std::vector<float> MVTXHitZ;

std::vector<float> MVTXHitTruthT;

std::vector<float> MVTXChipHitX;
std::vector<float> MVTXChipHitY;
std::vector<float> MVTXChipHitZ;

std::vector<float> MVTXLocalHitX;
std::vector<float> MVTXLocalHitY;
std::vector<float> MVTXLocalHitZ;


std::vector<float> INTTHitX;
std::vector<float> INTTHitY;
std::vector<float> INTTHitZ;
std::vector<float> INTTHitTruthT;

std::vector<int> MVTXTrkID; 
std::vector<int> INTTTrkID;
std::vector<int> LayerID;
std::vector<int> LadderZIdSave;
std::vector<int> LadderPhiIdSave;
std::vector<int> RowSave;
std::vector<int> ColSave;


std::vector<int> TruthTrkID;
std::vector<int> TruthTrkPDGID;
std::vector<int> ParentPDGIDSave;
std::vector<int> ParentTrkIDSave;

std::vector<float> TrackVx;
std::vector<float> TrackVy;
std::vector<float> TrackVz;
std::vector<float> TrackT;

std::vector<float> TrackPx;
std::vector<float> TrackPy;
std::vector<float> TrackPz;
std::vector<float> TrackE;
float PVx;
float PVy;
float PVz;



int HitSize;
int MVTXHitSize;
int EvtID;
int INTTHitSize;
int NTruthTrks;




unsigned int long long NEGED;

int HFMLTriggerInterface::Init(PHCompositeNode* topNode)
{

	std::cout << "Initial Bri" << std::endl;
	_ievent = 0;

	DebugFile = new TFile("DebugFile.root","RECREATE");

	DebugFile->cd();

	RawHitTree = new TTree("RawHitTree","RawHitTree");
	/*
	   RawHitTree->Branch("EvtID",&EvtID,"EvtID/I");
	   RawHitTree->Branch("MVTXHitSize",&MVTXHitSize,"MVTXHitSize/I");
	   RawHitTree->Branch("MVTXHitX",MVTXHitX,"MVTXHitX[MVTXHitSize]/F");
	   RawHitTree->Branch("MVTXHitY",MVTXHitY,"MVTXHitY[MVTXHitSize]/F");
	   RawHitTree->Branch("MVTXHitZ",MVTXHitZ,"MVTXHitZ[MVTXHitSize]/F");
	   RawHitTree->Branch("MVTXTrkID",MVTXTrkID,"MVTXTrkID[MVTXHitSize]/I");


	   RawHitTree->Branch("INTTHitSize",&INTTHitSize,"INTTHitSize/I");
	   RawHitTree->Branch("INTTHitX",INTTHitX,"INTTHitX[INTTHitSize]/F");
	   RawHitTree->Branch("INTTHitY",INTTHitY,"INTTHitY[INTTHitSize]/F");
	   RawHitTree->Branch("INTTHitZ",INTTHitZ,"INTTHitZ[INTTHitSize]/F");
	   RawHitTree->Branch("INTTTrkID",INTTTrkID,"INTTTrkID[MVTXHitSize]/I");



	   RawHitTree->Branch("LadderZIdSave",LadderZIdSave,"LadderZIdSave[INTTHitSize]/I");
	   RawHitTree->Branch("LadderPhiIdSave",LadderPhiIdSave,"LadderPhiIdSave[INTTHitSize]/I");
	   RawHitTree->Branch("RowSave",RowSave,"RowSave[INTTHitSize]/I");
	   RawHitTree->Branch("ColSave",ColSave,"ColSave[INTTHitSize]/I");

	   RawHitTree->Branch("HitSize",&HitSize,"HitSize/I");
	   RawHitTree->Branch("LayerID",LayerID,"LayerID[HitSize]/F");
	   */

	RawHitTree->Branch("EvtID",&EvtID,"EvtID/I");
	RawHitTree->Branch("MVTXHitSize",&MVTXHitSize);
	RawHitTree->Branch("MVTXHitX",&MVTXHitX);
	RawHitTree->Branch("MVTXHitY",&MVTXHitY);
	RawHitTree->Branch("MVTXHitZ",&MVTXHitZ);
	RawHitTree->Branch("MVTXHitTruthT",&MVTXHitTruthT);

	RawHitTree->Branch("MVTXLocalHitX",&MVTXLocalHitX);
	RawHitTree->Branch("MVTXLocalHitY",&MVTXLocalHitY);
	RawHitTree->Branch("MVTXLocalHitZ",&MVTXLocalHitZ);

	RawHitTree->Branch("MVTXChipHitX",&MVTXLocalHitX);
	RawHitTree->Branch("MVTXChipHitY",&MVTXLocalHitY);
	RawHitTree->Branch("MVTXChipHitZ",&MVTXLocalHitZ);


	RawHitTree->Branch("MVTXTrkID",&MVTXTrkID);


	RawHitTree->Branch("INTTHitSize",&INTTHitSize,"INTTHitSize/I");
	RawHitTree->Branch("INTTHitX",&INTTHitX);
	RawHitTree->Branch("INTTHitY",&INTTHitY);
	RawHitTree->Branch("INTTHitZ",&INTTHitZ);
	RawHitTree->Branch("INTTHitTruthT",&INTTHitTruthT);

	RawHitTree->Branch("INTTTrkID",&INTTTrkID);



	RawHitTree->Branch("LadderZIdSave",&LadderZIdSave);
	RawHitTree->Branch("LadderPhiIdSave",&LadderPhiIdSave);
	RawHitTree->Branch("RowSave",&RowSave);
	RawHitTree->Branch("ColSave",&ColSave);

	RawHitTree->Branch("HitSize",&HitSize,"HitSize/I");
	RawHitTree->Branch("LayerID",&LayerID);


	TruthTrackTree = new TTree("TruthTrackTree","TruthTrackTree");


	TruthTrackTree->Branch("EvtID",&EvtID,"EvtID/I");
	TruthTrackTree->Branch("PVx",&PVx,"PVx/F");
	TruthTrackTree->Branch("PVy",&PVy,"PVy/F");
	TruthTrackTree->Branch("PVz",&PVz,"PVz/F");


	TruthTrackTree->Branch("NTruthTrks",&NTruthTrks,"NTruthTrks/I");
	TruthTrackTree->Branch("TruthTrkID",&TruthTrkID);
	TruthTrackTree->Branch("TruthTrkPDGID",&TruthTrkPDGID);

	TruthTrackTree->Branch("ParentPDGIDSave",&ParentPDGIDSave);
	TruthTrackTree->Branch("ParentTrkIDSave",&ParentTrkIDSave);
	//	TruthTrackTree->Branch("PDGID",&PDGID);



	TruthTrackTree->Branch("TrackVx",&TrackVx);
	TruthTrackTree->Branch("TrackVy",&TrackVy);
	TruthTrackTree->Branch("TrackVz",&TrackVz);
	TruthTrackTree->Branch("TrackT",&TrackT);

	TruthTrackTree->Branch("TrackPx",&TrackPx);
	TruthTrackTree->Branch("TrackPy",&TrackPy);
	TruthTrackTree->Branch("TrackPz",&TrackPz);
	TruthTrackTree->Branch("TrackE",&TrackE);



	_f = new TFile((_foutname + string(".root")).c_str(), "RECREATE");

	m_jsonOut.open((_foutname + string(".json")).c_str(), fstream::out);

	m_jsonOut << "{" << endl;
	m_jsonOut << "\"Events\" : [" << endl;

	//  _h2 = new TH2D("h2", "", 100, 0, 100.0, 40, -2, +2);
	//  _h2_b = new TH2D("h2_b", "", 100, 0, 100.0, 40, -2, +2);
	//  _h2_c = new TH2D("h2_c", "", 100, 0, 100.0, 40, -2, +2);
	//  _h2all = new TH2D("h2all", "", 100, 0, 100.0, 40, -2, +2);

	m_hitLayerMap = new TH3F("hitLayerMap", "hitLayerMap", 600, -10, 10, 600, -10, 10, 10, -.5, 9.5);
	m_hitLayerMap->SetTitle("hitLayerMap;x [mm];y [mm];Half Layers");

	m_hitPixelPhiMap = new TH3F("hitPixelPhiMap", "hitPixelPhiMap", 16000, -.5, 16000 - .5, 600, -M_PI, M_PI, 10, -.5, 9.5);
	m_hitPixelPhiMap->SetTitle("hitPixelPhiMap;PixelPhiIndex in layer;phi [rad];Half Layers Index");
	m_hitPixelPhiMapHL = new TH3F("hitPixelPhiMapHL", "hitPixelPhiMapHL", 16000, -.5, 16000 - .5, 600, -M_PI, M_PI, 10, -.5, 9.5);
	m_hitPixelPhiMapHL->SetTitle("hitPixelPhiMap;PixelPhiIndex in half layer;phi [rad];Half Layers Index");
	m_hitPixelZMap = new TH3F("hitPixelZMap", "hitPixelZMap", 16000, -.5, 16000 - .5, 600, 15, 15, 10, -.5, 9.5);
	m_hitPixelZMap->SetTitle("hitPixelZMap;hitPixelZMap;z [cm];Half Layers");

	m_hitStaveLayer = new TH2F("hitStaveLayer", "hitStaveLayer", 100, -.5, 100 - .5, 10, -.5, 9.5);
	m_hitStaveLayer->SetTitle("hitStaveLayer;Stave index;Half Layers");
	m_hitModuleHalfStave = new TH2F("hitModuleHalfStave", "hitModuleHalfStave", 100, -.5, 100 - .5, 10, -.5, 9.5);
	m_hitModuleHalfStave->SetTitle("hitModuleHalfStave;Module index;Half Stave");
	m_hitChipModule = new TH2F("hitChipModule", "hitChipModule", 100, -.5, 100 - .5, 10, -.5, 9.5);
	m_hitChipModule->SetTitle("hitChipModule;Chip;Module");

	return Fun4AllReturnCodes::EVENT_OK;
}

int HFMLTriggerInterface::InitRun(PHCompositeNode* topNode)
{
	m_Geoms =
		findNode::getClass<PHG4CylinderGeomContainer>(topNode, "CYLINDERGEOM_MVTX");
	if (!m_Geoms)
	{
		//std::cout << PHWHERE << "ERROR: Can't find node CYLINDERCELLGEOM_MVTX" << std::endl;
		return Fun4AllReturnCodes::ABORTRUN;
	}

	m_Geoms2 =
		findNode::getClass<PHG4CylinderGeomContainer>(topNode, "CYLINDERGEOM_INTT");
	if (!m_Geoms2)
	{
		//std::cout << PHWHERE << "ERROR: Can't find node CYLINDERCELLGEOM_INTT" << std::endl;
		return Fun4AllReturnCodes::ABORTRUN;
	}

	m_truthInfo = findNode::getClass<PHG4TruthInfoContainer>(topNode, "G4TruthInfo");
	if (!m_truthInfo)
	{
		//std::cout << PHWHERE << "ERROR: Can't find node G4TruthInfo" << std::endl;
		return Fun4AllReturnCodes::ABORTRUN;
	}

	m_Flags = findNode::getClass<PdbParameterMap>(topNode, "HFMLTrigger_HepMCTriggerFlags");
	if (!m_Flags)
	{
		cout << "HFMLTriggerInterface::InitRun - WARNING - missing HFMLTrigger_HepMCTriggerFlags" << endl;
	}

	m_tGeometry = findNode::getClass<ActsGeometry>(topNode, "ActsGeometry");
	if (!m_tGeometry)
	{
		cout << "HFMLTriggerInterface::InitRun - WARNING - missing m_tGeometry" << endl;
	}


	return Fun4AllReturnCodes::EVENT_OK;
}

int HFMLTriggerInterface::process_event(PHCompositeNode* topNode)
{

	assert(m_tGeometry);

	//	cout << "ROCK - Runed bro" << endl;
	// load nodes
	auto res = load_nodes(topNode);
	//	cout << "ROCK - Runed bro - Pass 1.2 " << endl;

	if (res != Fun4AllReturnCodes::EVENT_OK)
		return res;
	//	cout << "ROCK - Runed bro - Pass 1.4 " << endl;
	/*
	   PHHepMCGenEvent* genevt = nullptr;
	   if(m_GenEventMap->get(_embedding_id)) genevt = m_GenEventMap->get(_embedding_id);
	   if(!genevt) cout << "NO genevt bro" <<endl;
	   cout << "Pass 1.9" << endl;
	   */


	/*
	   if (!_svtxevalstack)
	   {
	   _svtxevalstack = new SvtxEvalStack(topNode);
	   _svtxevalstack->set_strict(0);
	   _svtxevalstack->set_verbosity(Verbosity() + 1);
	   SvtxTruthEval* trutheval = _svtxevalstack->get_truth_eval();
	   }
	   else
	   {
	   _svtxevalstack->next_event(topNode);
	   }
	   */

	//  SvtxVertexEval* vertexeval = _svtxevalstack->get_vertex_eval();
	//  SvtxTrackEval* trackeval = _svtxevalstack->get_track_eval();
	// SvtxClusterEval* clustereval = _svtxevalstack->get_cluster_eval();
	//SvtxHitEval* hiteval = _svtxevalstack->get_hit_eval();
	/*


	   assert(m_GenEventMap);

	   PHHepMCGenEvent* genevt = m_GenEventMap->get(_embedding_id);
	   if (!genevt)
	   {
	   std::cout << PHWHERE << " - Fatal error - node PHHepMCGenEventMap missing subevent with embedding ID " << _embedding_id;
	//std::cout << ". Print PHHepMCGenEventMap:";
	m_GenEventMap->identify();
	//return Fun4AllReturnCodes::ABORTEVENT;
	}

	HepMC::GenEvent* theEvent = genevt->getEvent();


	assert(theEvent);
	if (Verbosity())
	{
	cout << "HFMLTriggerInterface::process_event - process HepMC::GenEvent with signal_process_id = "
	<< theEvent->signal_process_id();
	if (theEvent->signal_process_vertex())
	{
	cout << " and signal_process_vertex : ";
	theEvent->signal_process_vertex()->print();
	}
	cout << "  - Event record:" << endl;
	theEvent->print();
	}
	*/
	// property tree preparation
	//  using boost::property_tree::ptree;
	//  ptree pt;

	rapidjson::Document d;
	//	cout << "Pass 2" << endl;

	d.SetObject();
	rapidjson::Document::AllocatorType& alloc = d.GetAllocator();

	auto loadCoordinate = [&](double x, double y, double z) {
		//    ptree vertexTree;
		rapidjson::Value vertexTree(rapidjson::kArrayType);

		//    ptree vertexX;
		//    vertexX.put("", x);
		//    vertexTree.push_back(make_pair("", vertexX));

		//    ptree vertexY;
		//    vertexY.put("", y);
		//    vertexTree.push_back(make_pair("", vertexY));

		//    ptree vertexZ;
		//    vertexZ.put("", z);
		//    vertexTree.push_back(make_pair("", vertexZ));

		vertexTree.PushBack(x, alloc).PushBack(y, alloc).PushBack(z, alloc);

		return vertexTree;
	};

	// Create a root
	//  ptree pTree;

	// meta data
	//  ptree metaTree;
	rapidjson::Value metaTree(rapidjson::kObjectType);
	//	cout << "Pass 3" << endl;

	metaTree.AddMember("Description", "These are meta data for this event. Not intended to use in ML algorithm", alloc);
	metaTree.AddMember("EventID", _ievent, alloc);
	metaTree.AddMember("Unit", "cm", alloc);
	/*	
		if(genevt){
		metaTree.AddMember("CollisionVertex",
		loadCoordinate(genevt->get_collision_vertex().x(),
		genevt->get_collision_vertex().y(),
		genevt->get_collision_vertex().z()),
		alloc);
		}
		else{

		std::cout << "No SUCK - genevt bro" << std::endl;
		}
		*/
	metaTree.AddMember("Layer_Count", _nlayers_maps, alloc);
	metaTree.AddMember("PixelHalfLayerIndex_Count", _nlayers_maps * 2, alloc);

	for (unsigned int layer = 0; layer < _nlayers_maps; ++layer)
	{
		CylinderGeom_Mvtx* geom = dynamic_cast<CylinderGeom_Mvtx*>(m_Geoms->GetLayerGeom(layer));
		assert(geom);

		//    ptree layerDescTree;
		rapidjson::Value layerDescTree(rapidjson::kObjectType);

		static const unsigned int nChip(9);

		layerDescTree.AddMember("PixelPhiIndexInLayer_Count", geom->get_N_staves() * geom->get_NX(), alloc);
		layerDescTree.AddMember("PixelPhiIndexInHalfLayer_Count", geom->get_N_staves() * geom->get_NX() / 2, alloc);
		layerDescTree.AddMember("PixelZIndex_Count", nChip * geom->get_NZ(), alloc);
		layerDescTree.AddMember("HalfLayer_Count", 2, alloc);
		layerDescTree.AddMember("Stave_Count", geom->get_N_staves(), alloc);
		layerDescTree.AddMember("Chip_Count", nChip, alloc);
		layerDescTree.AddMember("Pixel_Count", geom->get_NX() * geom->get_NZ(), alloc);

		//    metaTree.AddMember(
		//        str(boost::format{"Layer%1%"} % layer).c_str(),
		//        layerDescTree, alloc);
		rapidjson::Value keyName(str(boost::format{"Layer%1%"} % layer).c_str(), alloc);
		metaTree.AddMember(keyName,
				layerDescTree, alloc);
	}

	//  ptree truthTriggerFlagTree;
	rapidjson::Value truthTriggerFlagTree(rapidjson::kObjectType);
	//	cout << "Pass 4" << endl;

	truthTriggerFlagTree.AddMember("Description",
			"These are categorical true/false MonteCalo truth tags for the event. These are only known in training sample. This would be trigger output in real data processing.",
			alloc);
	//    truthTriggerFlagTree.AddMember("ExampleSignal1", true, alloc);
	//    truthTriggerFlagTree.AddMember("ExampleSignal2", false, alloc);
	rapidjson::Value flagsTree(rapidjson::kObjectType);
	if (m_Flags)
	{
		auto range = m_Flags->get_iparam_iters();

		for (auto flagIter = range.first; flagIter != range.second; ++flagIter)
		{
			//      rapidjson::Value aFlag(rapidjson::kObjectType);

			const string& name = flagIter->first;
			rapidjson::Value keyName(name.c_str(), alloc);
			const bool flag = flagIter->second > 0 ? true : false;
			//const bool flag = false;

			flagsTree.AddMember(keyName, flag, alloc);
			//      flagsTree.PushBack(aFlag, alloc);
		}
	}
	truthTriggerFlagTree.AddMember("Flags", flagsTree, alloc);

	// Raw hits
	//  ptree rawHitTree;
	rapidjson::Value rawHitTree(rapidjson::kObjectType);
	rawHitTree.AddMember("Description",
			"Raw data in the event in an unordered set of hit ID. To help in visualization stage, the coordinate of the hit is also appended. These would be input in real data.",
			alloc);

	//  rawHitTree.put("LayerRage", "0-2");

	//  ptree rawHitsTree;
	rapidjson::Value rawHitsTree(rapidjson::kArrayType);



	assert(m_hitsets);
	unsigned int hitID(0);
	//	vector<vector<int>> HitInfo;
	//	vector<vector<int>> HitInfo_INTT;
	vector<int> TrkIDVec;
	vector<int> HitIDVec;
	vector<uint64_t> KeyIDVec;

	MVTXHitSize = 0;
	HitSize = 0;

	//int IndexID;

	NEGED = 0;



	cout << "Pass IN Why 5" << endl;

	auto hitset_range = m_hitsets->getHitSets(TrkrDefs::TrkrId::mvtxId);
	for (auto hitset_iter =  hitset_range.first; hitset_iter != hitset_range.second; ++hitset_iter)
	{
		auto hitsetkey = hitset_iter->first;
		auto hit_range = hitset_iter->second->getHits();
		for (auto hit_iter = hit_range.first;
				hit_iter != hit_range.second;
				++hit_iter)
		{
			auto hitkey = hit_iter->first;
			auto hit = hit_iter->second;
			assert(hit);
			// get truth hit assoc
			int trkid;
			float MVTXHitTime = -1;

			TrkrHitTruthAssoc::MMap g4hit_map;
			m_hit_truth_map->getG4Hits(hitsetkey, hitkey, g4hit_map);
			//	int SUCK = 0;
			if (g4hit_map.size() != 1)
			{
				//std::cout << "More than one (" << g4hit_map.size() <<") g4Hit associated to " << hitkey << std::endl;
				//			SUCK = 1;
			}
			rapidjson::Value truthHitTree(rapidjson::kArrayType);
			for (auto truth_iter = g4hit_map.begin(); truth_iter != g4hit_map.end(); ++truth_iter)
			{
				// g4hit key
				const auto g4hit_key = truth_iter->second.second;
				truthHitTree.PushBack(static_cast<uint64_t>(g4hit_key), alloc);


				//cout << "static_cast<uint64_t>(g4hit_key) = " << static_cast<uint32_t>(g4hit_key) << "    static_cast<uint64_t>(g4hit_key) = " << static_cast<uint64_t>(g4hit_key) << endl;


				PHG4Hit *g4hit = m_g4hits_mvtx->findHit( static_cast<uint64_t>(g4hit_key) );

				trkid =  g4hit->get_trkid();

				MVTXHitTime = g4hit->get_t(0);


				//		std::cout << "MVTXHitTime = " << MVTXHitTime << std::endl; 

				//	if(trkid < 0)// cout << "trkid = " << trkid << "   static_cast<uint64_t>(g4hit_key) =  " <<  static_cast<uint64_t>(g4hit_key) << endl;
				//	if(SUCK == 1)// cout << "trkid = " << trkid << "   static_cast<uint64_t>(g4hit_key) =  " <<  static_cast<uint64_t>(g4hit_key) << endl;
				if(trkid < 0 ) NEGED = static_cast<uint64_t>(g4hit_key);

				//	auto hitid =  g4hit->get_hit_id();
				HitIDVec.push_back(hitID);
				TrkIDVec.push_back(trkid);

				KeyIDVec.push_back(static_cast<uint64_t>(g4hit_key));

				//cout << "MVTX: trkid =  " << trkid << "  hitID =  " << hitID<< endl;



				//cout << "trkid =  " << trkid << "  hitid =  " << hitid<< endl;


			}

			unsigned int layer = TrkrDefs::getLayer(hitset_iter->first);
			if (layer < _nlayers_maps)
			{
				CylinderGeom_Mvtx* geom = dynamic_cast<CylinderGeom_Mvtx*>(m_Geoms->GetLayerGeom(layer));
				assert(geom);

				unsigned int pixel_x = MvtxDefs::getRow(hitkey);
				unsigned int pixel_z = MvtxDefs::getCol(hitkey);
				assert((int)pixel_x < geom->get_NX());
				assert((int)pixel_z < geom->get_NZ());
				unsigned int chip = MvtxDefs::getChipId(hitsetkey);
				unsigned int stave = MvtxDefs::getStaveId(hitsetkey);
				auto surface = m_tGeometry->maps().getSiliconSurface(hitsetkey);


				//cout << "hitsetkey = " << hitsetkey << "     chip = " << chip << "    stave = " << stave << "    layer = " << layer << endl;
				TVector3 local_coords = geom->get_local_coords_from_pixel(pixel_x, pixel_z);

				//Add Surface and Act Maps Properties


				/*
				   TVector3 world_coords = geom->get_world_from_local_coords(stave,
				   0,
				   0,
				   chip,
				   local_coords);
				   */


				TVector2 LocalUse;
				LocalUse.SetX(local_coords.x());
				LocalUse.SetY(local_coords.z());

				TVector3 world_coords = geom->get_world_from_local_coords(surface, m_tGeometry ,LocalUse);



				//unsigned int halflayer = (int) pixel_x >= geom->get_NX() / 2 ? 0 : 1;

				//unsigned int halfLayerIndex(layer * 2 + halflayer);
				//unsigned int pixelPhiIndex(
				//  cell->get_stave_index() * geom->get_NX() + pixel_x);
				//unsigned int pixelPhiIndexHL(
				//  cell->get_stave_index() * geom->get_NX() / 2 + pixel_x % (geom->get_NX() / 2));
				// unsigned int pixelZIndex(cell->get_chip_index() * geom->get_NZ() + pixel_z);

				//      ptree hitTree;
				rapidjson::Value hitTree(rapidjson::kObjectType);

				//      ptree hitIDTree;
				rapidjson::Value hitIDTree(rapidjson::kObjectType);
				hitIDTree.AddMember("HitSequenceInEvent", hitID, alloc);
				hitIDTree.AddMember("G4HitAssoc", truthHitTree, alloc);
				hitIDTree.AddMember("MVTXTrkID", trkid, alloc);
				hitIDTree.AddMember("MVTXHitTime", MVTXHitTime, alloc);

				//hitIDTree.AddMember("PixelHalfLayerIndex", halfLayerIndex, alloc);
				//hitIDTree.AddMember("PixelPhiIndexInLayer", pixelPhiIndex, alloc);
				//hitIDTree.AddMember("PixelPhiIndexInHalfLayer", pixelPhiIndexHL, alloc);
				//hitIDTree.AddMember("PixelZIndex", pixelZIndex, alloc);

				hitIDTree.AddMember("Layer", layer, alloc);
				//hitIDTree.AddMember("HalfLayer", halflayer, alloc);
				hitIDTree.AddMember("Stave", stave, alloc);
				//      hitIDTree.put("HalfStave", cell->get_half_stave_index());
				//      hitIDTree.put("Module", cell->get_module_index());
				hitIDTree.AddMember("Chip", chip, alloc);
				hitIDTree.AddMember("Pixel_x", pixel_x, alloc);
				hitIDTree.AddMember("Pixel_z", pixel_z, alloc);
				hitTree.AddMember("ID", hitIDTree, alloc);

				//			double PixelGlobalX = world_coords.x()+ local_coords.x();
				//			double PixelGlobalY = world_coords.y()+ local_coords.y();
				//			double PixelGlobalZ = world_coords.z()+ local_coords.z();

				double PixelGlobalX = world_coords.x();
				double PixelGlobalY = world_coords.y();
				double PixelGlobalZ = world_coords.z();

				hitTree.AddMember("Coordinate",
						loadCoordinate(PixelGlobalX,
							PixelGlobalY,
							PixelGlobalZ),
						alloc);


				hitTree.AddMember("ChipGlobalCoordinate",
						loadCoordinate(world_coords.x(),
							world_coords.y(),
							world_coords.z()),
						alloc);

				hitTree.AddMember("PixelLocalCoordinate",
						loadCoordinate(local_coords.x(),
							local_coords.y(),
							local_coords.z()),
						alloc);


				//      rawHitsTree.add_child("MVTXHit", hitTree);
				rawHitsTree.PushBack(hitTree, alloc);

				m_hitStaveLayer->Fill(stave, layer);
				m_hitModuleHalfStave->Fill(stave, layer);
				m_hitChipModule->Fill(chip, stave);

				m_hitLayerMap->Fill(world_coords.x(), world_coords.y(), layer);
				//m_hitPixelPhiMap->Fill(pixelPhiIndex, atan2(world_coords.y(), world_coords.x()), layer);
				//m_hitPixelPhiMapHL->Fill(pixelPhiIndexHL, atan2(world_coords.y(), world_coords.x()), layer);
				//m_hitPixelZMap->Fill(pixelZIndex, world_coords.z(), halfLayerIndex);
				/*
				   MVTXHitX[MVTXHitSize] = world_coords.x();
				   MVTXHitY[MVTXHitSize] = world_coords.y();
				   MVTXHitZ[MVTXHitSize] = world_coords.z();
				   MVTXTrkID[MVTXHitSize] = trkid;
				   LayerID[HitSize] = layer;
				   ++hitID;
				   */

				//			MVTXHitX.push_back(world_coords.x() + local_coords.x());
				//			MVTXHitY.push_back(world_coords.y() + local_coords.y());
				//			MVTXHitZ.push_back(world_coords.z() + local_coords.z());

				MVTXHitX.push_back(world_coords.x() );
				MVTXHitY.push_back(world_coords.y() );
				MVTXHitZ.push_back(world_coords.z() );
				MVTXHitTruthT.push_back(MVTXHitTime);

				MVTXLocalHitX.push_back(local_coords.x());
				MVTXLocalHitY.push_back(local_coords.y());
				MVTXLocalHitZ.push_back(local_coords.z());

				MVTXChipHitX.push_back(world_coords.x());
				MVTXChipHitY.push_back(world_coords.y());
				MVTXChipHitZ.push_back(world_coords.z());



				MVTXTrkID.push_back(trkid);
				LayerID.push_back(layer);

				hitID++;
				MVTXHitSize = MVTXHitSize + 1;
				HitSize = HitSize + 1;
			}  //    if (layer < _nlayers_maps)
		}  //   for (TrkrHitSetContainer::ConstIterator hitset_iter = hitset_range.first;
	}  //   for (TrkrHitSet::ConstIterator hit_iter = hit_range.first;

	rawHitTree.AddMember("MVTXHits", rawHitsTree, alloc);
	rapidjson::Value rawHitsTree2(rapidjson::kArrayType);


	TrkrHitSetContainer::ConstRange hitset_range_intt = m_hitsets->getHitSets(TrkrDefs::TrkrId::inttId);

	int INTTHITS = 0;
	unsigned int hitIDINTT(0);

	vector<int> TrkIDVecINTT;
	vector<int> HitIDVecINTT;
	vector<uint64_t> KeyIDVecINTT;


	INTTHitSize = 0;


	////      cout << "Pass 1 new" << endl;

	for (TrkrHitSetContainer::ConstIterator hitset_iter = hitset_range_intt.first;
			hitset_iter != hitset_range_intt.second;
			++hitset_iter)
	{
		//	  TrkrDefs::hitsetkey hitSetKey = hitset_iter->first;
		auto hitsetkey = hitset_iter->first;

		TrkrHitSet::ConstRange hit_range = hitset_iter->second->getHits();
		for (TrkrHitSet::ConstIterator hit_iter = hit_range.first;
				hit_iter != hit_range.second;
				++hit_iter)
		{

			//      cout << "Pass 1" << endl;
			TrkrDefs::hitkey hitKey = hit_iter->first;
			TrkrHit* hit = hit_iter->second;
			assert(hit);
			unsigned int layer = TrkrDefs::getLayer(hitset_iter->first);
			//      cout << "Pass 2" << "   layer = " << layer  << endl;


			int trkid;		
			float INTTHitTime = -1;
			// get truth hit assoc
			TrkrHitTruthAssoc::MMap g4hit_map;
			m_hit_truth_map->getG4Hits(hitsetkey, hitKey, g4hit_map);
			if (g4hit_map.size() != 1)
			{
				//std::cout << "More than one (" << g4hit_map.size() <<") g4Hit associated to " << hitKey << std::endl;
			}
			rapidjson::Value truthHitTree(rapidjson::kArrayType);
			for (auto truth_iter = g4hit_map.begin(); truth_iter != g4hit_map.end(); ++truth_iter)
			{
				// g4hit key
				const auto g4hit_key = truth_iter->second.second;
				truthHitTree.PushBack(static_cast<uint64_t>(g4hit_key), alloc);

				PHG4Hit *g4hit = m_g4hits_intt->findHit( static_cast<uint64_t>(g4hit_key) );

				INTTHitTime = g4hit->get_t(0);

				trkid =  g4hit->get_trkid();
				HitIDVecINTT.push_back(hitIDINTT);	
				TrkIDVecINTT.push_back(trkid);
				KeyIDVecINTT.push_back(static_cast<uint64_t>(g4hit_key) );

				//	auto hitid =  g4hit->get_hit_id();

				//	HitInfo_INTT[trkid].push_back(hitIDINTT);
				//cout << "INTT:: trkid =  " << trkid << endl;

				//	cout << "INTT: trkid =  " << trkid << "  hitIDINTT =  " << hitIDINTT << endl;

			}


			if(layer > _nlayers_maps-1){

				CylinderGeomIntt* geom = dynamic_cast<CylinderGeomIntt*>(m_Geoms2->GetLayerGeom(layer));
				//      cout << "Pass 3" << endl;


				unsigned int Row = InttDefs::getRow(hitKey);
				unsigned int Col = InttDefs::getCol(hitKey);
				unsigned int LadderZId = InttDefs::getLadderZId(hitsetkey);
				unsigned int LadderPhiId = InttDefs::getLadderPhiId(hitsetkey);

				//cout << "INTT:  layer =    " << layer << "   LadderZId =  " << LadderZId << "   LadderPhiId = " << LadderPhiId << "      hitsetkey = " << hitsetkey << endl;


				//      cout << "Pass 3.5" << endl;


				//			TVector2 hit_location(0.0,0.0,0.0);
				double hit_location[3] = {0.0,0.0,0.0};

				/*
				   geom->find_strip_center(LadderZId,
				   LadderPhiId,
				   Col,
				   Row,
				   hit_location);
				   */



				auto surface = m_tGeometry->maps().getSiliconSurface(hitsetkey);

				//			geom->find_strip_center_localcoords(LadderZId, Row, Col,location);



				//TVector3 world_coord =  get_world_from_local_coords(, hit_location)
				geom->find_strip_center(surface,m_tGeometry,LadderZId,LadderPhiId,Col,Row,hit_location);

				//geom->find_strip_center(surface, m_tGeometry, LadderZId, LadderPhiId, Row, Col, hit_location);

				//Use Surface for INTT too

				rapidjson::Value hitTree2(rapidjson::kObjectType);
				//      cout << "Pass 4" << endl;

				rapidjson::Value hitIDTree2(rapidjson::kObjectType);

				//int inttlayer = layer - _nlayers_maps;
				hitIDTree2.AddMember("HitSequenceInEvent", hitIDINTT, alloc);
				hitIDTree2.AddMember("G4HitAssoc", truthHitTree, alloc);	
				hitIDTree2.AddMember("INTTTrkID", trkid, alloc);
				hitIDTree2.AddMember("INTTHitTime", INTTHitTime, alloc);

				hitIDTree2.AddMember("Layer", layer, alloc);
				hitIDTree2.AddMember("Row", Row, alloc);
				hitIDTree2.AddMember("Col", Col, alloc);
				hitIDTree2.AddMember("LadderZId", LadderZId, alloc);
				hitIDTree2.AddMember("LadderPhiId", LadderPhiId, alloc);

				hitTree2.AddMember("ID", hitIDTree2, alloc);

				/*
				// cout << "layer = " << layer << endl;
				// cout << "Pixel_x = " << pixel_x << endl;
				// cout << "Pixel_z = " << pixel_z << endl;
				// cout << "LadderZId = " << LadderZId << endl;
				// cout << "LadderPhiId = " << LadderPhiId << endl;
				*/


				hitTree2.AddMember("Coordinate",	loadCoordinate(hit_location[0], hit_location[1], hit_location[2]),	alloc);



				//cout << "Pass 4" << endl;
				rawHitsTree2.PushBack(hitTree2, alloc);


				//		rawHitTree.PushBack(hitTree2, alloc);

				m_hitLayerMap->Fill(hit_location[0], hit_location[1], layer);
				/*
				   INTTHitX[INTTHitSize] = hit_location[0];
				   INTTHitY[INTTHitSize] = hit_location[1];
				   INTTHitZ[INTTHitSize] = hit_location[2];
				   INTTTrkID[INTTHitSize] = trkid;
				   LadderZIdSave[INTTHitSize] = LadderZId;
				   LadderPhiIdSave[INTTHitSize] = LadderPhiId;
				   RowSave[INTTHitSize] = Row;
				   ColSave[INTTHitSize] = Col;

				   LayerID[HitSize] = layer;
				   */

				INTTHitX.push_back(hit_location[0]);
				INTTHitY.push_back(hit_location[1]);
				INTTHitZ.push_back(hit_location[2]);
				INTTHitTruthT.push_back(INTTHitTime);

				INTTTrkID.push_back(trkid);
				LadderZIdSave.push_back(LadderZId);
				LadderPhiIdSave.push_back(LadderPhiId);
				RowSave.push_back(Row);
				ColSave.push_back(Col);
				LayerID.push_back(layer);

				hitIDINTT++;
				INTTHitSize = INTTHitSize + 1;
				HitSize = HitSize + 1;

			}

			//      cout << "Pass 5" << endl;

		}

		INTTHITS = INTTHITS + 1;

	}

	rawHitTree.AddMember("INTTHITS", rawHitsTree2, alloc);

	//cout << "Pass 6" << endl;

	//	cout << "ZZ Check: INTTHITS = " << INTTHITS << endl;




	vector<int> IndexVec;
	vector<int> IndexVecINTT;


	// Truth hits
	//  ptree truthHitTree;
	rapidjson::Value truthHitTree(rapidjson::kObjectType);
	truthHitTree.AddMember("Description", "From the MonteCalo truth information, pairs of track ID and subset of RawHit that belong to the track. These are not presented in real data. The track ID is arbitary.",
			alloc);


	PHG4VtxPoint * PV = m_truthInfo->GetPrimaryVtx( m_truthInfo->GetPrimaryVertexIndex() );

	float primaryvx = PV->get_x();
	float primaryvy = PV->get_y();
	float primaryvz = PV->get_z();

	truthHitTree.AddMember("PVx", primaryvx, alloc);
	truthHitTree.AddMember("PVy", primaryvy, alloc);
	truthHitTree.AddMember("PVz", primaryvz, alloc);

	PVx = primaryvx;
	PVy = primaryvy;
	PVz = primaryvz;

	//MVTX

	assert(m_g4hits_mvtx);
	std::multimap<const int, const unsigned long long> m_track_g4hits_map;
	for (auto g4hit_iter = m_g4hits_mvtx->getHits().first;
			g4hit_iter != m_g4hits_mvtx->getHits().second;
			++g4hit_iter)
	{
		PHG4Hit *g4hit = g4hit_iter->second;

		//	if(g4hit->get_hit_id() == NEGED)// cout << "g4hit->get_hitid() = " << g4hit->get_hit_id() << "      g4hit->get_trkid()  = " << g4hit->get_trkid() << endl;

		//int trkidCheck  = g4hit->get_trkid();

		//PHG4Particle * ParticleDebug =  m_truthInfo->GetParticle(trkidCheck);

		//		cout << "g4hit->get_hitid() = " << g4hit->get_hit_id() << "      g4hit->get_trkid()  = " << g4hit->get_trkid() << "   ParticleDebug->get_pid() =  " << ParticleDebug->get_pid() << endl;

		m_track_g4hits_map.insert(make_pair(g4hit->get_trkid(), g4hit_iter->first));
	}


	//INTT


	assert(m_g4hits_intt);
	std::multimap<const int, const unsigned long long> m_track_g4hits_map_intt;
	for (auto g4hit_iter = m_g4hits_intt->getHits().first;
			g4hit_iter != m_g4hits_intt->getHits().second;
			++g4hit_iter)
	{
		PHG4Hit *g4hit = g4hit_iter->second;
		m_track_g4hits_map_intt.insert(make_pair(g4hit->get_trkid(), g4hit_iter->first));
	}


	//  ptree truthTracksTree;
	rapidjson::Value truthTracksTree(rapidjson::kArrayType);
	// get set of primary particle
	assert(m_truthInfo);
	//	auto pp_range = m_truthInfo->GetPrimaryParticleRange();
	auto pp_range = m_truthInfo->GetParticleRange();

	NTruthTrks = 0;
	int ParentPDGID;
	int ParentTrkID;
	int GrandParentPDGID;
	int GrandParentTrkID;
	int trktype;

	PHG4Particle *mother = nullptr;
	PHG4Particle *grandmother = nullptr;


	for (auto pp_iter = pp_range.first;
			pp_iter != pp_range.second;
			++pp_iter)
	{

		PHG4Particle *g4particle = pp_iter->second;
		assert(g4particle);
		//  vtx = trutheval->get_vertex(g4particle);

		ParentPDGID = 999;
		ParentTrkID = -999;
		GrandParentPDGID = 999;
		GrandParentTrkID = -999;


		if (g4particle->get_parent_id() == 0)
		{
			ParentPDGID = 0;
		}
		else
		{
			mother = m_truthInfo->GetParticle(g4particle->get_parent_id());
			ParentPDGID = mother->get_pid();
			ParentTrkID = mother->get_track_id();

		}

		if(mother){

			if (mother->get_parent_id() == 0)
			{
				GrandParentPDGID = 0;

			}else
			{

				grandmother = m_truthInfo->GetParticle(mother->get_parent_id());
				GrandParentPDGID = grandmother->get_pid();
				GrandParentTrkID = grandmother->get_track_id();

			}
		}else{

			cout << "No Mother Bad" << endl;
		}



		//if(abs(ParentPDGID) > 400 && abs(ParentPDGID)  < 500) cout << "ParentPDGID = " << ParentPDGID << "   ParentTrkID = " << ParentTrkID << std::endl; 
		if(abs(ParentPDGID) == 421) cout << "ParentPDGID = " << ParentPDGID << "   ParentTrkID = " << ParentTrkID << std::endl; 
		if(ParentPDGID == 0) trktype = 0;
		else if((abs(ParentPDGID) == 521 || abs(ParentPDGID) == 531 || abs(ParentPDGID) == 511 || abs(ParentPDGID) == 5122 || abs(ParentPDGID) == 553)) trktype = 1;	
		else if((abs(GrandParentPDGID) == 521 || abs(GrandParentPDGID) == 531 || abs(GrandParentPDGID) == 511 || abs(GrandParentPDGID) == 5122 || abs(GrandParentPDGID) == 553) && (abs(ParentPDGID) == 421 || abs(ParentPDGID) == 431 || abs(ParentPDGID) == 411 || abs(ParentPDGID) == 4122 || abs(ParentPDGID) == 443)) trktype = 2;
		else if((GrandParentPDGID == 0) && (abs(ParentPDGID) == 421 || abs(ParentPDGID) == 431 || abs(ParentPDGID) == 411 || abs(ParentPDGID) == 4122 || abs(ParentPDGID) == 443)) trktype = 3;
		else if((GrandParentPDGID == 0) && (abs(ParentPDGID) == 310 || abs(ParentPDGID) == 130 || abs(ParentPDGID) == 2122 || abs(ParentPDGID) == 3122)) trktype = 4;
		else if((GrandParentPDGID == 0) && (abs(ParentPDGID) == 13 || abs(ParentPDGID) == 211 || abs(ParentPDGID) == 321)) trktype = 5;
		else trktype = 6;



		vtx = m_truthInfo->GetVtx(g4particle->get_vtx_id());



		if (! m_track_g4hits_map.count(g4particle->get_track_id()))
		{
			if (Verbosity() >= VERBOSITY_MORE)
				//		std::cout << "WARNING: G4 particle " << g4particle->get_track_id() << " does not hit any MVTX layer." << std::endl;
				continue;
		}
		if (! m_track_g4hits_map_intt.count(g4particle->get_track_id()))
		{
			if (Verbosity() >= VERBOSITY_MORE)
				//  std::cout << "WARNING: G4 particle " << g4particle->get_track_id() 			<< " does not hit any INTT layer." << std::endl;
				continue;
		}


		//    ptree trackHitTree;OriginX
		//rapidjson::Document trackHitTree;

		rapidjson::Value trackHitTree(rapidjson::kArrayType);



		rapidjson::Value MVTXHitIDTree(rapidjson::kArrayType);
		rapidjson::Value MVTXHitIDTree2(rapidjson::kArrayType);


		int trkid = g4particle->get_track_id();
		//	int pid = g4particle->get_pid();
		//	int parentid = g4particle->get_parent_id();
		//	int primaryid = g4particle->get_primary_id();
		//	int vtxid = g4particle->get_vtx_id();

		//if(trkid < 0)  cout << "pid = " << pid << "   parentid = " << parentid  << "   primaryid = " << primaryid << "   vtxid = " << vtxid << endl;

		//MVTX
		int LayerHere = 0;
		auto g4hits_iter = m_track_g4hits_map.equal_range(g4particle->get_track_id());

		//if(g4particle->get_track_id() < 0)	cout << "g4particle->get_track_id() = " << g4particle->get_track_id() << endl;
		//	cout << "g4particle->get_track_id() = " << g4particle->get_track_id() << endl;

		for (auto& g4hit_iter = g4hits_iter.first;
				g4hit_iter != g4hits_iter.second; ++g4hit_iter)
		{
			//          ptree hitIDTree;
			//          hitIDTree.put("", g4hit_key);
			//	int Info[2];
			//	HitInFoVec.first = LayerHere;
			//	HitInFoVec.second =  static_cast<uint64_t>(g4hit_iter->second);




			//	cout << "HitInfo[trkid].size() = " << HitInfo[trkid].size() << endl;

			rapidjson::Value trackHitTree2(rapidjson::kArrayType);

			trackHitTree2.PushBack(LayerHere, alloc);
			trackHitTree2.PushBack(static_cast<uint64_t>(g4hit_iter->second), alloc);

			/*
			   PHG4Hit *g4hit = m_g4hits_mvtx->findHit( static_cast<uint64_t>(g4hit_iter->second) );
			   if(g4hit->get_trkid() < 0){
			// cout << "g4hit->get_hitid() = " << g4hit->get_hit_id() << "      g4hit->get_trkid()  = " << g4hit->get_trkid() << "  g4particle->get_track_id() = " << g4particle->get_track_id() << "   g4particle->get_pid() = " <<  g4particle->get_pid() << endl;


			}
			*/
			trackHitTree.PushBack(trackHitTree2, alloc);

			//    rapidjson::Value LayerID(LayerHere, alloc);
			//    rapidjson::Value TruthHitKey(static_cast<uint64_t>(g4hit_iter->second), alloc);
			//	trackHitTree.AddMember(LayerID,TruthHitKey, alloc);
			LayerHere = LayerHere + 1;


			IndexVec.clear();

			int KeyIDVecSize = KeyIDVec.size();

			for(int i = 0; i < KeyIDVecSize; i++){


				if(static_cast<uint64_t>(g4hit_iter->second) == KeyIDVec[i]) IndexVec.push_back(i);

			}
			int	HitSize = IndexVec.size();
			//		cout << "HitSize = " << HitSize << endl;

			for(int i = 0; i < HitSize; i++){

				int IndexID = IndexVec[i];

				MVTXHitIDTree2.PushBack(HitIDVec[IndexID],alloc);


			}





		}  //   for (auto& clus_key : clus_keys)





		MVTXHitIDTree.PushBack(MVTXHitIDTree2, alloc);














		//INTT
		//      cout << "Pass 6.6" << endl;


		rapidjson::Value INTTHitIDTree(rapidjson::kArrayType);
		rapidjson::Value INTTHitIDTree2(rapidjson::kArrayType);

		//      cout << "Pass 6.7" << endl;

		auto g4hits_iter_intt = m_track_g4hits_map_intt.equal_range(g4particle->get_track_id());









		LayerHere = 3;

		for (auto& g4hit_iter = g4hits_iter_intt.first;
				g4hit_iter != g4hits_iter_intt.second; ++g4hit_iter)
		{
			//          ptree hitIDTree;
			//          hitIDTree.put("", g4hit_key);
			//	int Info[2];
			//	HitInFoVec.first = LayerHere;
			//	HitInFoVec.second =  static_cast<uint64_t>(g4hit_iter->second);




			//	cout << "HitInfo[trkid].size() = " << HitInfo[trkid].size() << endl;

			rapidjson::Value trackHitTree2(rapidjson::kArrayType);

			trackHitTree2.PushBack(LayerHere, alloc);
			trackHitTree2.PushBack(static_cast<uint64_t>(g4hit_iter->second), alloc);

			trackHitTree.PushBack(trackHitTree2, alloc);

			//    rapidjson::Value LayerID(LayerHere, alloc);
			//    rapidjson::Value TruthHitKey(static_cast<uint64_t>(g4hit_iter->second), alloc);
			//	trackHitTree.AddMember(LayerID,TruthHitKey, alloc);
			LayerHere = LayerHere + 1;


			IndexVecINTT.clear();




			//      cout << "Pass 6.10" << endl;


			int KeyIDVecSize_intt = KeyIDVecINTT.size();






			//      cout << "Pass 6.11" << endl;

			for(int i = 0; i < KeyIDVecSize_intt; i++){


				if(static_cast<uint64_t>(g4hit_iter->second) == KeyIDVecINTT[i]) IndexVecINTT.push_back(i);

			}

			//      cout << "Pass 6.12" << endl;

			int	HitSize_intt = IndexVecINTT.size();

			//		cout << "HitSize_intt = " << HitSize_intt << endl;

			for(int i = 0; i < HitSize_intt; i++){

				int IndexID = IndexVecINTT[i];


				INTTHitIDTree2.PushBack(HitIDVecINTT[IndexID],alloc);

			}





		}  //   for (auto& clus_key : clus_keys)





		INTTHitIDTree.PushBack(INTTHitIDTree2, alloc);



		//      cout << "Pass 6.13" << endl;



		/*

		   TruthTrkID[NTruthTrks] = g4particle->get_track_id();
		   TruthTrkPDGID[NTruthTrks] = g4particle->get_pid();

		   TrackVx[NTruthTrks] = vtx->get_x();
		   TrackVy[NTruthTrks] = vtx->get_y();
		   TrackVz[NTruthTrks] = vtx->get_z();

		   TrackPx[NTruthTrks] = g4particle->get_px();
		   TrackPy[NTruthTrks] = g4particle->get_py();
		   TrackPz[NTruthTrks] = g4particle->get_pz();
		   TrackE[NTruthTrks] = g4particle->get_e();

		   ParentPDGIDSave[NTruthTrks] = ParentPDGID;
		   ParentTrkIDSave[NTruthTrks] = ParentTrkID;


		   NTruthTrks = NTruthTrks + 1;
		   */

		TruthTrkID.push_back( g4particle->get_track_id());
		TruthTrkPDGID.push_back( g4particle->get_pid());
		TrackVx.push_back( vtx->get_x());
		TrackVy.push_back( vtx->get_y());
		TrackVz.push_back( vtx->get_z());

		TrackT.push_back( vtx->get_t());



		TrackPx.push_back( g4particle->get_px());
		TrackPy.push_back( g4particle->get_py());
		TrackPz.push_back( g4particle->get_pz());
		TrackE.push_back( g4particle->get_e());

		ParentPDGIDSave.push_back(ParentPDGID);
		ParentTrkIDSave.push_back(ParentTrkID);


		NTruthTrks = NTruthTrks + 1;



		//      ptree trackTree;
		rapidjson::Value trackTree(rapidjson::kObjectType);
		trackTree.AddMember("OriginVertexPoint", loadCoordinate(vtx->get_x(),vtx->get_y(),vtx->get_z()), alloc);
		trackTree.AddMember("TrackSequenceInEvent", g4particle->get_track_id(), alloc);
		trackTree.AddMember("HitInTruthTrack", trackHitTree, alloc);
		trackTree.AddMember("TrackID", trkid, alloc);		
		trackTree.AddMember("TrackType", trktype, alloc);		

		trackTree.AddMember("MVTXHitID", MVTXHitIDTree, alloc);
		trackTree.AddMember("INTTHitID", INTTHitIDTree, alloc);

		trackTree.AddMember("ParentPDGID", ParentPDGID, alloc);
		trackTree.AddMember("ParentTrkID", ParentTrkID, alloc);
		trackTree.AddMember("GrandParentPDGID", GrandParentPDGID, alloc);
		trackTree.AddMember("GrandParentTrkID", GrandParentTrkID, alloc);

		trackTree.AddMember("ParticleTypeID", g4particle->get_pid(), alloc);
		trackTree.AddMember("TrackMomentum",
				loadCoordinate(g4particle->get_px(),
					g4particle->get_py(),
					g4particle->get_pz()),
				alloc);
		trackTree.AddMember("TrackEnergy",g4particle->get_e() , alloc);

		//      trackTree.put("TrackDCA3DXY", track->get_dca3d_xy());
		//      trackTree.put("TrackDCA3DZ", track->get_dca3d_z());

		//      trackTree.add_child("TruthHit", trackHitTree);

		//      truthTracksTree.add_child("TruthTrack", trackTree);
		truthTracksTree.PushBack(trackTree, alloc);

		//      cout << "Pass 6.14" << endl;




		//cout << "g4particle->get_pid() = " << g4particle->get_pid() << "   g4particle->get_Z() = " << g4particle->get_Z()  << endl;
	}  //  for (PHG4TruthInfoContainer::ConstIterator iter = range.first;

	truthHitTree.AddMember("TruthTracks", truthTracksTree, alloc);

	//output
	d.AddMember("MetaData", metaTree, alloc);
	d.AddMember("TruthTriggerFlag", truthTriggerFlagTree, alloc);
	d.AddMember("RawHit", rawHitTree, alloc);
	d.AddMember("TruthHit", truthHitTree, alloc);

	assert(m_jsonOut.is_open());

	if (_ievent > 0)
	{
		m_jsonOut << "," << endl;
	}

	//  write_json(m_jsonOut, pTree);
	//  write_xml(m_jsonOut, jsonTree);

	//  d.AddMember("Test", 1, d.GetAllocator());

	rapidjson::OStreamWrapper osw(m_jsonOut);
	rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);

	d.Accept(writer);


	EvtID = _ievent;
	RawHitTree->Fill();
	TruthTrackTree->Fill();

	++_ievent;

	MVTXHitX.clear();
	MVTXHitY.clear();
	MVTXHitZ.clear();
	MVTXHitTruthT.clear();



	INTTHitX.clear();
	INTTHitY.clear();
	INTTHitZ.clear();
	INTTHitTruthT.clear();

	MVTXTrkID.clear(); 
	INTTTrkID.clear();
	LayerID.clear();
	LadderZIdSave.clear();
	LadderPhiIdSave.clear();
	RowSave.clear();
	ColSave.clear();


	TruthTrkID.clear();
	TruthTrkPDGID.clear();
	ParentPDGIDSave.clear();
	ParentTrkIDSave.clear();

	TrackVx.clear();
	TrackVy.clear();
	TrackVz.clear();
	TrackT.clear();

	TrackPx.clear();
	TrackPy.clear();
	TrackPz.clear();
	TrackE.clear();



	return Fun4AllReturnCodes::EVENT_OK;
}

int HFMLTriggerInterface::End(PHCompositeNode* topNode)
{

	DebugFile->cd();
	RawHitTree->Write();
	TruthTrackTree->Write();
	//	DebugFile->Write();
	DebugFile->Close();

	if (m_jsonOut.is_open())
	{
		m_jsonOut << "]" << endl;
		m_jsonOut << "}" << endl;

		m_jsonOut.close();
	}

	cout << "HFMLTriggerInterface::End - output to " << _foutname << ".*" << endl;

	return Fun4AllReturnCodes::EVENT_OK;
}

int HFMLTriggerInterface::load_nodes(PHCompositeNode* topNode)
{
	//   cout << "Loading Nodes Bro" << endl;
	m_hitsets = findNode::getClass<TrkrHitSetContainer>(topNode, "TRKR_HITSET");
	if (!m_hitsets)
	{
		std::cout << PHWHERE << "ERROR: Can't find node TRKR_HITSET" << std::endl;
		return Fun4AllReturnCodes::ABORTEVENT;
	}
	//    cout << "Loading Nodes Bro - Pass 1" << endl;

	m_hit_truth_map = findNode::getClass<TrkrHitTruthAssoc>(topNode, "TRKR_HITTRUTHASSOC");
	if (!m_hit_truth_map)
	{
		std::cout << PHWHERE << " unable to find DST node TRKR_HITTRUTHASSOC" << std::endl;
		return Fun4AllReturnCodes::ABORTEVENT;
	}
	//    cout << "Loading Nodes Bro - Pass 2" << endl;

	//m_cluster_hit_map = findNode::getClass<TrkrClusterHitAssoc>(topNode, "TRKR_CLUSTERHITASSOC");
	//if (!m_cluster_hit_map)
	//{
	//  //std::cout << PHWHERE << " unable to find DST node TRKR_CLUSTERHITASSOC" << std::endl;
	//  return Fun4AllReturnCodes::ABORTEVENT;
	//}

	m_g4hits_mvtx = findNode::getClass<PHG4HitContainer>(topNode, "G4HIT_MVTX");
	if (!m_g4hits_mvtx)
	{
		//std::cout << PHWHERE << " unable to find DST node G4HIT_MVTX" << std::endl;
		return Fun4AllReturnCodes::ABORTEVENT;
	}

	//   cout << "Loading Nodes Bro - Pass 3" << endl;

	m_g4hits_intt = findNode::getClass<PHG4HitContainer>(topNode, "G4HIT_INTT");
	if (!m_g4hits_intt)
	{
		//std::cout << PHWHERE << " unable to find DST node G4HIT_INTT" << std::endl;
		return Fun4AllReturnCodes::ABORTEVENT;
	}

	//    cout << "Loading Nodes Bro - Pass 4" << endl;
	/*
	   m_GenEventMap = findNode::getClass<PHHepMCGenEventMap>(topNode, "PHHepMCGenEventMap");
	   if (!m_GenEventMap)
	   {
	//std::cout << PHWHERE << " - Fatal error - missing node PHHepMCGenEventMap" << std::endl;
	return Fun4AllReturnCodes::ABORTEVENT;
	}
	*/	
	//	cout << "Pass All Brio" << endl;

	return Fun4AllReturnCodes::EVENT_OK;
}
