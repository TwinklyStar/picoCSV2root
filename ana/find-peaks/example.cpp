// This example is developed by Meng Lyu based on PeakFinder class
// GitHub link: https://github.com/claydergc/find-peaks

#include <iostream>
#include <algorithm>
#include "PeakFinder.cpp"
#include "TChain.h"
#include "TH1I.h"
#include "../ChannelReader.h"

void loader(int rate);

void example()
{
    Double_t offset = 150;  // The offset of 4 channels (set in picoscope software)
    Double_t threshold = -10;   // The threshold, below which it won't find peak

	TString infpath = "../../demo_bk/RootData/20231228_merged.root";
	TString outfpath = "demo_peaks.root";

	TChain* tt = new TChain("wfm");
	tt->Add(infpath);

    // Initialize ChannelReader Class AFTER reading the tree and BEFORE creating any instance
    ChannelReader::Initialize(tt);

    // Input parameter is the channel name that determines which channel the reader will read
    ChannelReader ChA1("ChA1"), ChB1("ChB1"), ChC1("ChC1"), ChD1("ChD1"),
                  ChA2("ChA2"), ChB2("ChB2"), ChC2("ChC2"), ChD2("ChD2");

    TFile* fo = TFile::Open(outfpath, "RECREATE");

	int nentries = tt->GetEntries();
	std::cout << "Processing " << nentries << " events..." << std::endl;

	std::vector<int> peakn_A1, peakn_B1, peakn_C1, peakn_D1,
                     peakn_A2, peakn_B2, peakn_C2, peakn_D2;
	for (int ientry=0; ientry<10; ientry++){
        if (ientry % (nentries/100) == 0) loader(ientry/(nentries/100));

		tt->GetEntry(ientry);
		std::vector<float> ChA1_vec_float, ChB1_vec_float, ChC1_vec_float, ChD1_vec_float,  // PeakFinder only supports float
                           ChA2_vec_float, ChB2_vec_float, ChC2_vec_float, ChD2_vec_float;  // PeakFinder only supports float
		for (int i=0; i<ChA1.V->size(); i++){
			ChA1_vec_float.push_back(ChA1.V->at(i) - offset);
			ChB1_vec_float.push_back(ChB1.V->at(i) - offset);
			ChC1_vec_float.push_back(ChC1.V->at(i) - offset);
			ChD1_vec_float.push_back(ChD1.V->at(i) - offset);
		}

		if(ChA1.min_v <= threshold + offset) PeakFinder::findPeaks(ChA1_vec_float, peakn_A1, false, -1);	// -1 is find minimum, 1 is find maximum
        if(ChB1.min_v <= threshold + offset) PeakFinder::findPeaks(ChB1_vec_float, peakn_B1, false, -1);
		if(ChC1.min_v <= threshold + offset) PeakFinder::findPeaks(ChC1_vec_float, peakn_C1, false, -1);
		if(ChD1.min_v <= threshold + offset) PeakFinder::findPeaks(ChD1_vec_float, peakn_D1, false, -1);

		std::cout << std::endl << "Event: " << ientry << std::endl;
		std::cout << "ChA1: " << peakn_A1.size();
		for (auto i : peakn_A1){
            std::cout << "\t" << i;
			std::cout << "\t" << ChA1.T->at(i);
		}
		std::cout << std::endl;
		std::cout << "ChB1: " << peakn_B1.size();
		for (auto i : peakn_B1){
            std::cout << "\t" << i;
			std::cout << "\t" << ChB1.T->at(i);
		}
		std::cout << std::endl;
		std::cout << "ChC1: " << peakn_C1.size();
		for (auto i : peakn_C1){
			std::cout << "\t" << ChC1.T->at(i);
		}
		std::cout << std::endl;
		std::cout << "ChD1: " << peakn_D1.size();
		for (auto i : peakn_D1){
			std::cout << "\t" << ChD1.T->at(i);
		}
		std::cout << std::endl;
        std::cout << std::endl;

		peakn_A1.clear(); peakn_A1.shrink_to_fit();
		peakn_B1.clear(); peakn_B1.shrink_to_fit();
		peakn_C1.clear(); peakn_C1.shrink_to_fit();
		peakn_D1.clear(); peakn_D1.shrink_to_fit();
	}
	fo->Write();
	fo->Close();
	std::cout << "\nSave file as: " << outfpath << std::endl;

}

void loader(int rate)
{
    char proc[22];
    memset(proc, '\0', sizeof(proc));

    for (int i = 0; i < rate/5; i++)
    {
        proc[i] = '#';
    }

    printf("\r[%-20s] [%d%%]", proc, rate);        //C语言格式控制时默认右对齐，所以要在前面加-变成左对齐
    fflush(stdout);                                 //刷新屏幕打印
}
