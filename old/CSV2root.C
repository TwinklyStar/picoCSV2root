{
	auto fileName = "../data/20231221/20231221_00001.csv";
	auto df = ROOT::RDF::MakeCsvDataFrame(fileName);	
	df.Define() // Display some basic information of df
	auto cnt = df.Count()	// Event number in df
	auto s2d = [] (std::string s_num) {return std::stod(s_num);}	
	rdf = df.Redefine("Time", s2d, {"Time"});
}
