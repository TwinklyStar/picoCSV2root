void drawWaveform(Int_t evt_num=0){
    Double_t offset = 150;  // The offset of 4 channels (set in picoscope software)
    TString fpath = "../../demo/RootData/20231228_merged.root";

    TFile* ff = TFile::Open(fpath, "READ");
    TTree* tt = (TTree*) ff->Get("wfm");

	// if the macro has the function name (e.g. drawWaveform), the vector has to be registered in this way (new ...)
    std::vector<Double_t> *time_vec = new std::vector<Double_t>;
    std::vector<Double_t> *ChA_vec  = new std::vector<Double_t>;
    std::vector<Double_t> *ChB_vec  = new std::vector<Double_t>;
    std::vector<Double_t> *ChC_vec  = new std::vector<Double_t>;
    std::vector<Double_t> *ChD_vec  = new std::vector<Double_t>;

    tt->SetBranchAddress("ChA1_T", &time_vec);  // For pico output, ChA1_T - ChD1_T are identical
    tt->SetBranchAddress("ChA1_V", &ChA_vec);  // Depend on the channel configuration in your file
    tt->SetBranchAddress("ChB1_V", &ChB_vec);  // Depend on the channel configuration in your file
    tt->SetBranchAddress("ChC1_V", &ChC_vec);  // Depend on the channel configuration in your file
    tt->SetBranchAddress("ChD1_V", &ChD_vec);  // Depend on the channel configuration in your file

    tt->GetEntry(evt_num);

    auto cc = new TCanvas("cc", "cc", 1600, 600);
    auto mg = new TMultiGraph;
    auto g1 = new TGraph; g1->SetLineColor(kBlue);      g1->SetLineWidth(3); g1->SetTitle("Channel A");
    auto g2 = new TGraph; g2->SetLineColor(kRed);       g2->SetLineWidth(3); g2->SetTitle("Channel B");
    auto g3 = new TGraph; g3->SetLineColor(kGreen);     g3->SetLineWidth(3); g3->SetTitle("Channel C");
    auto g4 = new TGraph; g4->SetLineColor(kOrange);    g4->SetLineWidth(3); g4->SetTitle("Channel D");
    auto lgd = new TLegend(0.85, 0.84, 0.995, 0.99);

    for (int i=0; i<time_vec->size(); i++){
        g1->SetPoint(i, time_vec->at(i), ChA_vec->at(i) - offset + 225);
        g2->SetPoint(i, time_vec->at(i), ChB_vec->at(i) - offset +  75);
        g3->SetPoint(i, time_vec->at(i), ChC_vec->at(i) - offset -  75);
        g4->SetPoint(i, time_vec->at(i), ChD_vec->at(i) - offset - 225);
    }

    mg->Add(g1); mg->Add(g2); mg->Add(g3); mg->Add(g4);
    lgd->AddEntry(g1, "Channel A, Offset =  225", "l");
    lgd->AddEntry(g2, "Channel B, Offset =   75", "l");
    lgd->AddEntry(g3, "Channel C, Offset =  -75", "l");
    lgd->AddEntry(g4, "Channel D, Offset = -225", "l");

    mg->SetTitle(Form("4-Channel Waveforms, Event %d; Time [#mus]; Voltage [mV]", evt_num));
    mg->GetHistogram()->GetXaxis()->SetRangeUser(-0.8, 9.8);
    mg->GetHistogram()->GetYaxis()->SetRangeUser(-450, 250);
    mg->Draw("A");

    lgd->Draw();
//	cc->Draw();
	cc->SaveAs(Form("plots/%05d.png", evt_num));
    delete cc;
}
