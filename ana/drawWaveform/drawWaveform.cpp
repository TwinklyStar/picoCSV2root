#include "ChannelReader.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TH1F.h"

void drawWaveform(Int_t evt_num=0){
    Double_t offset = 150;  // The offset of 4 channels (set in picoscope software)
    TString fpath = "../../demo/RootData/20231228_merged.root";

    TFile* ff = TFile::Open(fpath, "READ");
    TTree* tt = (TTree*) ff->Get("wfm");

    // Initialize ChannelReader Class AFTER reading the tree and BEFORE creating any instance
    ChannelReader::Initialize(tt);

    // Input parameter is the channel name that determines which channel the reader will read
    ChannelReader ChA1("ChA1"), ChB1("ChB1"), ChC1("ChC1"), ChD1("ChD1"),
                  ChA2("ChA2"), ChB2("ChB2"), ChC2("ChC2"), ChD2("ChD2");

    tt->GetEntry(evt_num);

    auto cc = new TCanvas("cc", "cc", 1600, 600);
    auto mg = new TMultiGraph;
    auto g1 = new TGraph; g1->SetLineColor(kBlue);      g1->SetLineWidth(3); g1->SetTitle("Channel A");
    auto g2 = new TGraph; g2->SetLineColor(kRed);       g2->SetLineWidth(3); g2->SetTitle("Channel B");
    auto g3 = new TGraph; g3->SetLineColor(kGreen);     g3->SetLineWidth(3); g3->SetTitle("Channel C");
    auto g4 = new TGraph; g4->SetLineColor(kOrange);    g4->SetLineWidth(3); g4->SetTitle("Channel D");
    auto lgd = new TLegend(0.75, 0.74, 0.995, 0.99);

    for (int i=0; i<ChA1.T->size(); i++){
        g1->SetPoint(i, ChA1.T->at(i), ChA1.V->at(i) - offset + 225);
        g2->SetPoint(i, ChB1.T->at(i), ChB1.V->at(i) - offset +  75);
        g3->SetPoint(i, ChC1.T->at(i), ChC1.V->at(i) - offset -  75);
        g4->SetPoint(i, ChD1.T->at(i), ChD1.V->at(i) - offset - 225);
    }

    mg->Add(g1); mg->Add(g2); mg->Add(g3); mg->Add(g4);
    lgd->AddEntry(g1, "Channel A1, Offset =  225", "l");
    lgd->AddEntry(g2, "Channel B1, Offset =   75", "l");
    lgd->AddEntry(g3, "Channel C1, Offset =  -75", "l");
    lgd->AddEntry(g4, "Channel D1, Offset = -225", "l");

    mg->SetTitle(Form("4-Channel Waveforms, Event %d; Time [#mus]; Voltage [mV]", evt_num));
    mg->GetHistogram()->GetXaxis()->SetRangeUser(-0.8, 9.8);
    mg->GetHistogram()->GetYaxis()->SetRangeUser(-450, 250);
    mg->Draw("A");

    lgd->Draw();
//	cc->Draw();
	cc->SaveAs(Form("plots/%05d.png", evt_num));
    delete cc;
}
