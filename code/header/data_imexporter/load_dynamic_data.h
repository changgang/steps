#ifndef LOAD_DYNAMIC_DATA_H_INCLUDED
#define LOAD_DYNAMIC_DATA_H_INCLUDED

#include <vector>
#include <string>

using namespace std;

int load_dynamic_data();
int load_dynamic_data_PSSE(const string file, int ver, int subver);
int load_dynamic_data_BPA(const string file, int ver, int subver);


int load_dynamic_data_PSSE_Generator_related(vector<string> & data);
int load_dynamic_data_PSSE_Load_related(vector<string> & data);
int load_dynamic_data_PSSE_Line_related(vector<string> & data);
int load_dynamic_data_PSSE_HVDC_related(vector<string> & data);

#endif // LOAD_DYNAMIC_DATA_H_INCLUDED
