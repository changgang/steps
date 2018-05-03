#ifndef PREPARE_MODELS_FOR_TEST_H
#define PREPARE_MODELS_FOR_TEST_H

#include "header/power_system_database.h"
#include "header/toolkit/dynamic_simulator/dynamic_simulator.h"

void prepare_basic_buses(POWER_SYSTEM_DATABASE* db);
void prepare_basic_generators(POWER_SYSTEM_DATABASE* db);
void prepare_basic_wt_generators(POWER_SYSTEM_DATABASE* db);
void prepare_basic_loads(POWER_SYSTEM_DATABASE* db);
void prepare_basic_lines(POWER_SYSTEM_DATABASE* db);
void prepare_basic_transformers(POWER_SYSTEM_DATABASE* db);
void prepare_basic_fixed_shunts(POWER_SYSTEM_DATABASE* db);
void prepare_basic_switched_shunts(POWER_SYSTEM_DATABASE* db);
void prepare_basic_hvdcs(POWER_SYSTEM_DATABASE* db);
void prepare_basic_equivalent_devices(POWER_SYSTEM_DATABASE* db);


void prepare_Arthur_R_Bergen_3_bus_model(POWER_SYSTEM_DATABASE* db);
void prepare_Arthur_R_Bergen_3_bus_model_buses(POWER_SYSTEM_DATABASE* db);
void prepare_Arthur_R_Bergen_3_bus_model_generators(POWER_SYSTEM_DATABASE* db);
void prepare_Arthur_R_Bergen_3_bus_model_loads(POWER_SYSTEM_DATABASE* db);
void prepare_Arthur_R_Bergen_3_bus_model_lines(POWER_SYSTEM_DATABASE* db);
void prepare_Arthur_R_Bergen_3_bus_model_transformers(POWER_SYSTEM_DATABASE* db);
void prepare_Arthur_R_Bergen_3_bus_model_areas(POWER_SYSTEM_DATABASE* db);
void prepare_Arthur_R_Bergen_3_bus_model_zones(POWER_SYSTEM_DATABASE* db);
void prepare_Arthur_R_Bergen_3_bus_model_owners(POWER_SYSTEM_DATABASE* db);


void prepare_IEEE_9_bus_model(POWER_SYSTEM_DATABASE* db);
void prepare_IEEE_9_bus_model_buses(POWER_SYSTEM_DATABASE* db);
void prepare_IEEE_9_bus_model_generators(POWER_SYSTEM_DATABASE* db);
void prepare_IEEE_9_bus_model_loads(POWER_SYSTEM_DATABASE* db);
void prepare_IEEE_9_bus_model_lines(POWER_SYSTEM_DATABASE* db);
void prepare_IEEE_9_bus_model_transformers(POWER_SYSTEM_DATABASE* db);
void prepare_IEEE_9_bus_model_areas(POWER_SYSTEM_DATABASE* db);
void prepare_IEEE_9_bus_model_zones(POWER_SYSTEM_DATABASE* db);
void prepare_IEEE_9_bus_model_owners(POWER_SYSTEM_DATABASE* db);
void prepare_IEEE_9_bus_model_dynamic_model(POWER_SYSTEM_DATABASE* db);
void prepare_IEEE_9_bus_model_classical_dynamic_model(POWER_SYSTEM_DATABASE* db);
void prepare_IEEE_9_bus_model_complete_dynamic_model(POWER_SYSTEM_DATABASE* db);


void prepare_psse_sample_model(POWER_SYSTEM_DATABASE* db);
void prepare_psse_sample_model_buses(POWER_SYSTEM_DATABASE* db);
void prepare_psse_sample_model_generators(POWER_SYSTEM_DATABASE* db);
void prepare_psse_sample_model_loads(POWER_SYSTEM_DATABASE* db);
void prepare_psse_sample_model_lines(POWER_SYSTEM_DATABASE* db);
void prepare_psse_sample_model_transformers(POWER_SYSTEM_DATABASE* db);
void prepare_psse_sample_model_fixed_shunts(POWER_SYSTEM_DATABASE* db);
void prepare_psse_sample_model_switched_shunts(POWER_SYSTEM_DATABASE* db);
void prepare_psse_sample_model_hvdcs(POWER_SYSTEM_DATABASE* db);
void prepare_psse_sample_model_areas(POWER_SYSTEM_DATABASE* db);
void prepare_psse_sample_model_zones(POWER_SYSTEM_DATABASE* db);
void prepare_psse_sample_model_owners(POWER_SYSTEM_DATABASE* db);

void prepare_ISO_New_England_39_bus_model(POWER_SYSTEM_DATABASE* db);
void prepare_ISO_New_England_39_bus_model_buses(POWER_SYSTEM_DATABASE* db);
void prepare_ISO_New_England_39_bus_model_generators(POWER_SYSTEM_DATABASE* db);
void prepare_ISO_New_England_39_bus_model_loads(POWER_SYSTEM_DATABASE* db);
void prepare_ISO_New_England_39_bus_model_lines(POWER_SYSTEM_DATABASE* db);
void prepare_ISO_New_England_39_bus_model_transformers(POWER_SYSTEM_DATABASE* db);
void prepare_ISO_New_England_39_bus_model_areas(POWER_SYSTEM_DATABASE* db);
void prepare_ISO_New_England_39_bus_model_zones(POWER_SYSTEM_DATABASE* db);
void prepare_ISO_New_England_39_bus_model_owners(POWER_SYSTEM_DATABASE* db);
#endif // PREPARE_MODELS_FOR_TEST_H
