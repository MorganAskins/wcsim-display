void wc_createScanNtuple(char *filename=NULL, char *outputname=NULL){
	// Please read:  the following code takes a WCSim root file, copies the current WCSim and WCSimGeo tree 
	// and adds Andy's vertex and direction truth information. 
	
	// Coder: M. Bergevin (Modification of wc_eventreco.C from A. Blake)
	// Direct angry email to : mbergevin@ucdavis.edu
		
	// Input  :  filename.root
	// Output : fitted_filename.root

	// To load on example file :> root -l -x -q 'wc_createScanNtuple.C("test.root","fitted_test.root")'
	
	// Load libraries
	// ==============
	gSystem->Load("libGeom");
	gSystem->Load("libEve");
	gSystem->Load("libMinuit");
	gSystem->Load("libCintex");
	Cintex::Enable();
	gSystem->Load("libWCSimRoot.so");
	gSystem->Load("libWCSimAnalysis.so");
	
	// Fit Data Type
	// =============	
   	
	Double_t vertex[3];								//////  Information needed by scanner team
	Double_t direction[3];
	
	
	// If output name is the same as the input name, the original file is destroyed.  This is to prevent this.  
	if (outputname==filename) {
		printf("Same name for input and output file, this is not allowed.  Breaking. \n");
		break;
	}
	TFile fNew(outputname,"RECREATE");    /////// The output file is loaded here
		
	
	// Initiate trees to be filled
	// ===========================
	
	TTree* tree = new TTree("wcsimT","WCSim Tree");
	WCSimRootEvent *wcsimrootsuperevent = new WCSimRootEvent(); //empty list
	wcsimrootsuperevent->Initialize(); // make at least one event
	Int_t branchStyle = 1; //new style by default
	TTree::SetBranchStyle(branchStyle);
	Int_t bufsize = 64000;
	TBranch *branch = tree->Branch("wcsimrootevent", "WCSimRootEvent", &wcsimrootsuperevent, bufsize,2);
	geoTree = new TTree("wcsimGeoT","WCSim Geometry Tree");
	WCSimRootGeom *wcsimrootgeom = new WCSimRootGeom();
	TBranch *geoBranch = geoTree->Branch("wcsimrootgeom", "WCSimRootGeom", &wcsimrootgeom, bufsize,0);
	
	TTree *TRUTH = new TTree("TRUTH", "WCSim truth reconstruction information");
	TTree *FITA = new TTree("FITA", "FITter Andy reconstruction information");
	TRUTH->Branch("vertex", vertex, "vertex[3]/D");					
	TRUTH->Branch("direction", direction, "direction[3]/D");
	FITA->Branch("vertex", vertex, "vertex[3]/D");
	FITA->Branch("direction", direction, "direction[3]/D");
		
	
	// Load file whose trees are to be copied
	// ======================================
	
	if (filename==NULL){
		printf("You need to input a file name, this is breaking!\n");break;}else{
		printf("Trying to load file %s\n",filename);
		TFile *fCopy = new TFile(filename);          
	}
	if (!fCopy->IsOpen()){
		cout << "Error, could not open input file: " << filename << endl;
		break;
		return -1;
	}

	// Obtain Original Tree to be copied
	// =================================
	
	TTree *wcsimTO = (TTree*)fCopy->Get("wcsimT");
	TTree *wcsimGeoTO = (TTree*)fCopy->Get("wcsimGeoT");
	
	// Initialize Reconstruction
	// =========================
	
	WCSimInterface::LoadData(filename);		  
	WCSimReco* myReco = WCSimRecoFactory::Instance()->MakeReco();	

	// Do Andy's fit and record. Also record truth information
	// =======================================================
	
	Int_t ntot = wcsimTO->GetEntries();
		
	for (int j = 0; j<ntot; j++){ 
		fCopy->cd();
		WCSimInterface::LoadEvent(j);
		WCSimRecoEvent* recoEvent = WCSimInterface::RecoEvent();
		WCSimTrueEvent* trueEvent = WCSimInterface::TrueEvent();
		myReco->Run(recoEvent);
		
		wcsimTO->GetEntry(j);
		fNew.cd();
		
		vertex[0] = recoEvent->GetVtxX();
		vertex[1] = recoEvent->GetVtxY();
		vertex[2] = recoEvent->GetVtxZ();
		direction[0] = recoEvent->GetDirX();
		direction[1] = recoEvent->GetDirY();
		direction[2] = recoEvent->GetDirZ();
		FITA->Fill();
		FITA->Write("",TObject::kOverwrite); //Added so only one tree makes it to the final file

		vertex[0] = trueEvent->GetVtxX();
		vertex[1] = trueEvent->GetVtxY();
		vertex[2] = trueEvent->GetVtxZ();
		direction[0] = trueEvent->GetDirX();
		direction[1] = trueEvent->GetDirY();
		direction[2] = trueEvent->GetDirZ();
		TRUTH->Fill();
		TRUTH->Write("",TObject::kOverwrite);
	}
	
	// Write out original WCSim and WCSimGeo tree
	// ==========================================
	
	tree = wcsimTO->CloneTree();
	geoTree = wcsimGeoTO->CloneTree();
	tree->Write();
	geoTree->Write();
	
	// You are done, close files
	// =========================
	
	fNew.Close();
	fCopy->Close();
}