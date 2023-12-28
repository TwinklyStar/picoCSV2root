#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "rapidcsv.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"

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

int main(int argc, char* argv[]){

    string fpath = argv[1];
    string date = fpath.substr(fpath.rfind("/")+1, 8);
    string name_template = date + string("_%05d.csv");
    int fnum = atoi(argv[2]);
    int output_num = atoi(argv[3]);
    string outname_template = fpath.substr(0, fpath.rfind("/")) + string("/RootData/") + date + string ("_%04d.root");
    string outfname = Form(outname_template.c_str(), output_num);
    TFile *ff = new TFile(outfname.c_str(), "RECREATE");
    TTree *tt = new TTree("wfm", "A tree storing waveform data from Picoscope");

    vector<Double_t> time_vec, chA, chB, chC, chD;
    Int_t smp_num;

    // This part is the simplification of saving data
    map<string, vector<Double_t>*> data_map;
    data_map.insert(pair<string, vector<Double_t>*> ("Time", &time_vec));
    data_map.insert(pair<string, vector<Double_t>*> ("Channel A", &chA));
    data_map.insert(pair<string, vector<Double_t>*> ("Channel B", &chB));
    data_map.insert(pair<string, vector<Double_t>*> ("Channel C", &chC));
    data_map.insert(pair<string, vector<Double_t>*> ("Channel D", &chD));
    vector<string> col_title;

    // Waveform Loop
    for (int idx = 1; idx < fnum+1; idx++){

        string fname = Form(name_template.c_str(), idx);
        string full_path = fpath + string("/") + fname;
        rapidcsv::Document doc(full_path);
        doc.SetStartLineNum(3);    // for CSV from picoscope, the first three rows are not data

        if (idx == 1){  // register branch according to channel number
            tt->Branch("SampNum", &smp_num, "SampNum/I");
            tt->Branch("Time", &time_vec);
            col_title.push_back("Time");
            if (doc.GetColumnIdx("Channel A") >= 0){
                tt->Branch("ChA", &chA);
                col_title.push_back("Channel A");
            }
            if (doc.GetColumnIdx("Channel B") >= 0){
                tt->Branch("ChB", &chB);
                col_title.push_back("Channel B");
            }
            if (doc.GetColumnIdx("Channel C") >= 0){
                tt->Branch("ChC", &chC);
                col_title.push_back("Channel C");
            }
            if (doc.GetColumnIdx("Channel D") >= 0){
                tt->Branch("ChD", &chD);
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
        if (!if_exception)
            tt->Fill();

        time_vec.clear();
        chA.clear();
        chB.clear();
        chC.clear();
        chD.clear();

        time_vec.shrink_to_fit();
        chA.shrink_to_fit();
        chB.shrink_to_fit();
        chC.shrink_to_fit();
        chD.shrink_to_fit();
    }

    ff->Write();
    ff->Close();
    std::cout << "\rSave file as: " << outfname << std::endl;
}

