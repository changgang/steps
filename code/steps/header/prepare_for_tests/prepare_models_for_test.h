#ifndef PREPARE_MODELS_FOR_TEST_H
#define PREPARE_MODELS_FOR_TEST_H

#include "header/power_system_database.h"
#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"

void prepare_basic_buses();
void prepare_basic_generators();
void prepare_basic_wt_generators();
void prepare_basic_pv_units();
void prepare_basic_loads();
void prepare_basic_lines();
void prepare_basic_transformers();
void prepare_basic_fixed_shunts();
void prepare_basic_switched_shunts();
void prepare_basic_hvdcs();
void prepare_basic_vsc_hvdcs();
void prepare_basic_equivalent_devices();
void prepare_basic_energy_strorages();


void prepare_Arthur_R_Bergen_3_bus_model();
void prepare_Arthur_R_Bergen_3_bus_model_buses();
void prepare_Arthur_R_Bergen_3_bus_model_generators();
void prepare_Arthur_R_Bergen_3_bus_model_loads();
void prepare_Arthur_R_Bergen_3_bus_model_lines();
void prepare_Arthur_R_Bergen_3_bus_model_transformers();
void prepare_Arthur_R_Bergen_3_bus_model_areas();
void prepare_Arthur_R_Bergen_3_bus_model_zones();
void prepare_Arthur_R_Bergen_3_bus_model_owners();


void prepare_IEEE_9_bus_model();
void prepare_IEEE_9_bus_model_buses();
void prepare_IEEE_9_bus_model_generators();
void prepare_IEEE_9_bus_model_loads();
void prepare_IEEE_9_bus_model_lines();
void prepare_IEEE_9_bus_model_transformers();
void prepare_IEEE_9_bus_model_areas();
void prepare_IEEE_9_bus_model_zones();
void prepare_IEEE_9_bus_model_owners();
void prepare_IEEE_9_bus_model_dynamic_model();
void prepare_IEEE_9_bus_model_classical_dynamic_model();
void prepare_IEEE_9_bus_model_complete_dynamic_model();


void prepare_psse_sample_model();
void prepare_psse_sample_model_buses();
void prepare_psse_sample_model_generators();
void prepare_psse_sample_model_loads();
void prepare_psse_sample_model_lines();
void prepare_psse_sample_model_transformers();
void prepare_psse_sample_model_fixed_shunts();
void prepare_psse_sample_model_switched_shunts();
void prepare_psse_sample_model_hvdcs();
void prepare_psse_sample_model_areas();
void prepare_psse_sample_model_zones();
void prepare_psse_sample_model_owners();

void prepare_ISO_New_England_39_bus_model();
void prepare_ISO_New_England_39_bus_model_buses();
void prepare_ISO_New_England_39_bus_model_generators();
void prepare_ISO_New_England_39_bus_model_loads();
void prepare_ISO_New_England_39_bus_model_lines();
void prepare_ISO_New_England_39_bus_model_transformers();
void prepare_ISO_New_England_39_bus_model_areas();
void prepare_ISO_New_England_39_bus_model_zones();
void prepare_ISO_New_England_39_bus_model_owners();
#endif // PREPARE_MODELS_FOR_TEST_H
