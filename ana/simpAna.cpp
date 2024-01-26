void simpAna(TString infname="../demo/RootData/20231228_merged.root"){
    Double_t offset = 150;  // The offset of 4 channels (set in picoscope software)

    TFile* ff = TFile::Open(fpath, "READ");
    TTree* tt = (TTree*) ff->Get("wfm");

	// if the macro has the function name (e.g. drawWaveform), the vector has to be registered in this way (new ...)
    std::vector<Double_t> *time_vec = new std::vector<Double_t>;
    std::vector<Double_t> *ChA_vec  = new std::vector<Double_t>;
    std::vector<Double_t> *ChB_vec  = new std::vector<Double_t>;
    std::vector<Double_t> *ChC_vec  = new std::vector<Double_t>;
    std::vector<Double_t> *ChD_vec  = new std::vector<Double_t>;

    tt->SetBranchAddress("Time", &time_vec);
    tt->SetBranchAddress("ChA", &ChA_vec);  // Depend on the channel configuration in your file
    tt->SetBranchAddress("ChB", &ChB_vec);  // Depend on the channel configuration in your file
    tt->SetBranchAddress("ChC", &ChC_vec);  // Depend on the channel configuration in your file
    tt->SetBranchAddress("ChD", &ChD_vec);  // Depend on the channel configuration in your file

    Long64_t nentries = tt->GetEntries();

    // Event loop
    for (int ientry=0; ientry<nentries; ientry++){
        tt->GetEntry(ientry);

        // Fill your analysis code here

        auto itr_max = TMath::LocMax(ChA_vec->begin(), ChA_vec->end());     // return vector iterator
        auto itr_min = TMath::LocMin(ChA_vec->begin(), ChA_vec->end());
        int idx_max = std::distance(ChA_vec->begin(), itr_max);             // vector iterator to index
        int idx_min = std::distance(ChA_vec->begin(), itr_min);

        std::cout << "\nEvent: " << ientry << std::endl;
        std::cout << "Minimum Voltage in ChA: " << *itr_min << "\t Maximum Voltage in ChA: " << *itr_min << std::endl;
        std::cout << "Index of min: " << idx_min << "\t Index of max: " << idx_max << std::endl;
        }
    }

}
