#ifndef LOAD_NETWORK_DATA_H_INCLUDED
#define LOAD_NETWORK_DATA_H_INCLUDED

#include <vector>
#include <string>

using namespace std;

int load_network_data();
int load_network_data_PSSE(const string file, int ver, int subver);
int load_network_data_BPA(const string file, int ver, int subver);


int load_network_data_PSSE_Case(vector <string> & fileInRAM);
int load_network_data_PSSE_Bus(vector <string> & fileInRAM);
int load_network_data_PSSE_Load(vector <string> & fileInRAM);
int load_network_data_PSSE_FixedShunt(vector <string> & fileInRAM);
int load_network_data_PSSE_Generator(vector <string> & fileInRAM);
int load_network_data_PSSE_Line(vector <string> & fileInRAM);
int load_network_data_PSSE_Transformer(vector <string> & fileInRAM);
int load_network_data_PSSE_Area(vector <string> & fileInRAM);
int load_network_data_PSSE_HVDC(vector <string> & fileInRAM);
int load_network_data_PSSE_VSCHVDC(vector <string> & fileInRAM);
int load_network_data_PSSE_TransZCorrectionTab(vector <string> & fileInRAM);
int load_network_data_PSSE_Zone(vector <string> & fileInRAM);

#endif // LOAD_NETWORK_DATA_H_INCLUDED
