#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "rapidcsv.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TMath.h"

using namespace std;

// Progress bar
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

struct channel_stat{
    Double_t max_v;
    Double_t max_t;
    Double_t min_v;
    Double_t min_t;
    Double_t mean;
    Double_t RMS;
};
int main(int argc, char* argv[]){

    string fpath = argv[1];     // /example/20231228 (1)
    string date = fpath.substr(fpath.rfind("/")+1, 8);  // 20231228
    int fnum = atoi(argv[2]);
    int output_num = atoi(argv[3]);
    string infile_name_example = argv[4];   // 20231228 (1)_0001.csv
    UInt_t evt_num = atoi(argv[5]);
    string prefix = infile_name_example.substr(0, infile_name_example.rfind("_"));  // 20231228 (1)
    int infile_num_length = infile_name_example.rfind(".") - infile_name_example.rfind("_") -1; // 4
//    std::cout << "aaa" << std::endl;
//    std::cout << infile_name_length << std::endl;
    string name_template = prefix + string("_%0") + to_string(infile_num_length)+ string("d.csv");  // 20231228 (1)_%04d.csv
//    string name_template = date + string("_%02d.csv");
    string outname_template = fpath.substr(0, fpath.rfind("/")) + string("/RootData/") + date + string ("_%04d.root");
    string outfname = Form(outname_template.c_str(), output_num);
    TFile *ff = new TFile(outfname.c_str(), "RECREATE");
    TTree *tt = new TTree("wfm", "A tree storing waveform data from Picoscope");

    vector<Double_t> time_vec, ChA1_V, ChB1_V, ChC1_V, ChD1_V;
    channel_stat *ChA1_stat = new channel_stat;
    channel_stat *ChB1_stat = new channel_stat;
    channel_stat *ChC1_stat = new channel_stat;
    channel_stat *ChD1_stat = new channel_stat;
    Int_t smp_num;

    // This part is the simplification of saving data
    map<string, vector<Double_t>*> data_map;
    map<string, channel_stat*> ch_map;
    data_map.insert(pair<string, vector<Double_t>*> ("Time", &time_vec));
    data_map.insert(pair<string, vector<Double_t>*> ("Channel A", &ChA1_V));
    data_map.insert(pair<string, vector<Double_t>*> ("Channel B", &ChB1_V));
    data_map.insert(pair<string, vector<Double_t>*> ("Channel C", &ChC1_V));
    data_map.insert(pair<string, vector<Double_t>*> ("Channel D", &ChD1_V));
    ch_map.insert(pair<string, channel_stat*> ("Channel A", ChA1_stat));
    ch_map.insert(pair<string, channel_stat*> ("Channel B", ChB1_stat));
    ch_map.insert(pair<string, channel_stat*> ("Channel C", ChC1_stat));
    ch_map.insert(pair<string, channel_stat*> ("Channel D", ChD1_stat));
    vector<string> col_title;

    // Waveform Loop
    for (int idx = 1; idx < fnum+1; idx++){

        string fname = Form(name_template.c_str(), idx);
//        std::cout << fname << std::endl;
        string full_path = fpath + string("/") + fname;
        rapidcsv::Document doc(full_path);
        doc.SetStartLineNum(3);    // for CSV from picoscope, the first three rows are not data

        if (idx == 1){  // register branch according to channel number
            tt->Branch("EvtNum", &evt_num, "EvtNum/i");
            tt->Branch("Data_Length", &smp_num, "Data_Length/I");
            col_title.push_back("Time");
            if (doc.GetColumnIdx("Channel A") >= 0){
                tt->Branch("ChA1_T", &time_vec);
                tt->Branch("ChA1_V", &ChA1_V);
                tt->Branch("ChA1_stat", ChA1_stat, "max_v/D:max_t/D:min_v/D:min_t/D:mean/D:RMS/D");
                col_title.push_back("Channel A");
            }
            if (doc.GetColumnIdx("Channel B") >= 0){
                tt->Branch("ChB1_T", &time_vec);
                tt->Branch("ChB1_V", &ChB1_V);
                tt->Branch("ChB1_stat", ChB1_stat, "max_v/D:max_t/D:min_v/D:min_t/D:mean/D:RMS/D");
                col_title.push_back("Channel B");
            }
            if (doc.GetColumnIdx("Channel C") >= 0){
                tt->Branch("ChC1_T", &time_vec);
                tt->Branch("ChC1_V", &ChC1_V);
                tt->Branch("ChC1_stat", ChC1_stat, "max_v/D:max_t/D:min_v/D:min_t/D:mean/D:RMS/D");
                col_title.push_back("Channel C");
            }
            if (doc.GetColumnIdx("Channel D") >= 0){
                tt->Branch("ChD1_T", &time_vec);
                tt->Branch("ChD1_V", &ChD1_V);
                tt->Branch("ChD1_stat", ChD1_stat, "max_v/D:max_t/D:min_v/D:min_t/D:mean/D:RMS/D");
                col_title.push_back("Channel D");
            }
            if (output_num == 1) {
                std::cout << "Branches in output tree:" << std::endl;
                for (auto str: col_title) std::cout << str << "\t";
                std::cout << std::endl;
            }
        }

        // progress bar
        if (idx % (fnum/100) == 0) loader(idx/(fnum/100));

//        cout << "Processing file: " << full_path << "..." << std::endl;
        bool if_exception = false;
        for (auto str : col_title){     // iterate valid data (jump unused channels)
            auto itr = data_map.find(str);
            try {
                *itr->second = doc.GetColumn<Double_t>(str);
            }
            catch (exception &e){
                std::cout << "\rInvalid number (e.g. infinity) in: " << full_path << "--" << str << std::endl;
                if_exception = true;
            }
        }
        smp_num = time_vec.size();

        if (!if_exception) {
            // Preliminary data analysis
            for (auto str: col_title) {
                auto itr = data_map.find(str);
                auto itr_max = TMath::LocMax(itr->second->begin(),
                                             itr->second->end());     // return vector iterator
                auto itr_min = TMath::LocMin(itr->second->begin(),
                                             itr->second->end());     // return vector iterator
                int idx_max = std::distance(itr->second->begin(),
                                            itr_max);             // vector iterator to index
                int idx_min = std::distance(itr->second->begin(), itr_min);

                auto stat_itr = ch_map.find(str);
                stat_itr->second->max_v = *itr_max;
                stat_itr->second->min_v = *itr_min;
                stat_itr->second->max_t = time_vec.at(idx_max);
                stat_itr->second->min_t = time_vec.at(idx_min);
                stat_itr->second->mean = TMath::Mean(itr->second->begin(), itr->second->end());
                stat_itr->second->RMS = TMath::RMS(itr->second->begin(), itr->second->end());
            }
            tt->Fill();
        }
        evt_num++;

        // Release memory
        time_vec.clear();
        time_vec.shrink_to_fit();
        ChA1_V.clear();
        ChA1_V.shrink_to_fit();
        ChB1_V.clear();
        ChB1_V.shrink_to_fit();
        ChC1_V.clear();
        ChC1_V.shrink_to_fit();
        ChD1_V.clear();
        ChD1_V.shrink_to_fit();
    }

    ff->Write();
    ff->Close();
    delete ChA1_stat;
    delete ChB1_stat;
    delete ChC1_stat;
    delete ChD1_stat;
    std::cout << "\rSave file as: " << outfname << std::endl;
}

