struct channel_stat{
    Double_t max_v;
    Double_t max_t;
    Double_t min_v;
    Double_t min_t;
    Double_t mean;
    Double_t RMS;
};

void simpAna(TString infname="../demo/RootData/20231228_merged.root"){
    Double_t offset = 150;  // The offset of 4 channels (set in picoscope software)

    TFile* ff = TFile::Open(infname, "READ");
    TTree* tt = (TTree*) ff->Get("wfm");

    // if the macro has the function name (e.g. drawWaveform), the vector has to be registered in this way (new ...)
    std::vector<Double_t> *time_vec = new std::vector<Double_t>;
    std::vector<Double_t> *ChA_vec  = new std::vector<Double_t>;
    std::vector<Double_t> *ChB_vec  = new std::vector<Double_t>;
    std::vector<Double_t> *ChC_vec  = new std::vector<Double_t>;
    std::vector<Double_t> *ChD_vec  = new std::vector<Double_t>;
    channel_stat ChA1_stat, ChB1_stat, ChC1_stat, ChD1_stat;
    Int_t data_length;
    UInt_t evt_num;

    tt->SetBranchAddress("EvtNum", &evt_num);  // For pico output, ChA1_T - ChD1_T are identical
    tt->SetBranchAddress("Data_Length", &data_length);  // For pico output, ChA1_T - ChD1_T are identical
    tt->SetBranchAddress("ChA1_T", &time_vec);  // For pico output, ChA1_T - ChD1_T are identical
    tt->SetBranchAddress("ChA1_V", &ChA_vec);  // Depend on the channel configuration in your file
    tt->SetBranchAddress("ChB1_V", &ChB_vec);  // Depend on the channel configuration in your file
    tt->SetBranchAddress("ChC1_V", &ChC_vec);  // Depend on the channel configuration in your file
    tt->SetBranchAddress("ChD1_V", &ChD_vec);  // Depend on the channel configuration in your file
    tt->SetBranchAddress("ChA1_stat", &ChA1_stat);
    tt->SetBranchAddress("ChB1_stat", &ChB1_stat);
    tt->SetBranchAddress("ChC1_stat", &ChC1_stat);
    tt->SetBranchAddress("ChD1_stat", &ChD1_stat);

    int nentries = tt->GetEntries();

    // Event loop
    for (int ientry=0; ientry<nentries; ientry++){
        tt->GetEntry(ientry);

        // Fill your analysis code here
        if (ientry == 0) std::cout << "Data length is: " << data_length << std::endl;

        if (evt_num % 10 == 0) {
            std::cout << "\nEvent: " << evt_num << std::endl;
            std::cout << "Minimum Voltage in ChA: " << ChA1_stat.min_v << "mV\t at " << ChA1_stat.min_t << "us"
                      << "\nMaximum Voltage in ChA: " << ChA1_stat.max_v << "mV\t at " << ChA1_stat.max_t << "us"
                      << std::endl;     // The unit may change according to the settings in PicoScope software
            std::cout << "The 100th point in ChA is: " << time_vec->at(99) << "us, " << ChA_vec->at(99) << "mV" << std::endl;
        }
    }

}
