#ifndef ALL_SUPPORTED_MODELS_H
#define ALL_SUPPORTED_MODELS_H


#include "header/model/sg_models/sync_generator_model/sync_generator_models.h"
#include "header/model/sg_models/compensator_model/compensator_models.h"
#include "header/model/sg_models/exciter_model/exciter_models.h"
#include "header/model/sg_models/stabilizer_model/stabilizer_models.h"
#include "header/model/sg_models/turbine_governor_model/turbine_governor_models.h"
#include "header/model/sg_models/turbine_load_controller_model/turbine_load_controller_models.h"

#include "header/model/load_model/load_models.h"
#include "header/model/load_relay_model/load_voltage_relay_models.h"
#include "header/model/load_relay_model/load_frequency_relay_models.h"

#include "header/model/hvdc_model/hvdc_models.h"

#include "header/model/vsc_hvdc_model/vsc_hvdc_network_model/vsc_hvdc_network_models.h"
#include "header/model/vsc_hvdc_model/vsc_hvdc_converter_model/vsc_hvdc_converter_models.h"

#include "header/model/wtg_models/wt_generator_model/wt_generator_models.h"
#include "header/model/wtg_models/wt_aerodynamic_model/wt_aerodynamic_models.h"
#include "header/model/wtg_models/wt_electrical_model/wt_electrical_models.h"
#include "header/model/wtg_models/wt_turbine_model/wt_turbine_models.h"
#include "header/model/wtg_models/wt_pitch_model/wt_pitch_models.h"
#include "header/model/wtg_models/wind_speed_model/wind_speed_models.h"
#include "header/model/wtg_models/wt_voltage_ride_through_model/wt_voltage_ride_through_models.h"
#include "header/model/wtg_models/wt_relay_model/wt_relay_models.h"

#include "header/model/pvu_models/pv_converter_model/pv_converter_models.h"
#include "header/model/pvu_models/pv_panel_model/pv_panel_models.h"
#include "header/model/pvu_models/pv_electrical_model/pv_electrical_models.h"
#include "header/model/pvu_models/pv_irradiance_model/pv_irradiance_models.h"

#include "header/model/energy_storage_model/energy_storage_models.h"

#include "header/model/equivalent_model/ARXL.h"

#endif // ALL_SUPPORTED_MODELS_H
