#include <istream>
#include <cstdlib>
#include <cstring>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

#include "header/basic/terminal_test.h"
#include "header/basic/device_id_test.h"
#include "header/basic/area_test.h"
#include "header/basic/zone_test.h"
#include "header/basic/owner_test.h"
#include "header/basic/ownership_test.h"
#include "header/basic/rating_test.h"
#include "header/basic/utility_test.h"
#include "header/basic/sparse_matrix_test.h"
#include "header/basic/fault_test.h"
#include "header/basic/timer_test.h"

#include "header/basic/bus_index_test.h"
#include "header/basic/device_index_map_test.h"
#include "header/basic/inphno_test.h"

#include "header/device/bus_test.h"
#include "header/device/source_test.h"
#include "header/device/generator_test.h"
#include "header/device/wt_generator_test.h"
#include "header/device/pv_unit_test.h"
#include "header/device/load_test.h"
#include "header/device/line_test.h"
#include "header/device/transformer_test.h"
#include "header/device/fixed_shunt_test.h"
#include "header/device/hvdc_test.h"
#include "header/device/equivalent_device_test.h"
#include "header/device/engergy_storage_test.h"
//#include "header/device/virtual_generator_load_pair_test.h"

#include "header/power_system_database_test.h"

#include "header/data_imexporter/steps_imexporter_test.h"
#include "header/data_imexporter/psse_imexporter_test.h"
#include "header/data_imexporter/bpa_imexporter_test.h"
#include "header/data_imexporter/equivalent_model_imexporter_test.h"

#include "header/network/network_matrix_test.h"
#include "header/network/jacobian_builder_test.h"

#include "header/toolkit/powerflow_solver/powerflow_solver_test.h"

#include "header/block/proportional_block_test.h"
#include "header/block/integral_block_test.h"
#include "header/block/differential_block_test.h"
#include "header/block/first_order_block_test.h"
#include "header/block/lead_lag_block_test.h"
#include "header/block/pid_block_test.h"
#include "header/block/pi_block_test.h"
#include "header/block/pd_block_test.h"
#include "header/block/saturation_block_test.h"

#include "header/meter/continuous_buffer_test.h"
#include "header/meter/meter_test.h"
#include "header/meter/meter_setter_test.h"


#include "header/toolkit/dynamic_simulator/dynamic_simulator_test.h"

#include "header/model/model_var_table_test.h"

#include "header/model/model_test.h"
#include "header/model/sg_models/sync_generator_model/sync_generator_model_test.h"
#include "header/model/sg_models/sync_generator_model/gencls_test.h"
#include "header/model/sg_models/sync_generator_model/genrou_test.h"
#include "header/model/sg_models/sync_generator_model/gensal_test.h"

#include "header/model/sg_models/compensator_model/COMP_test.h"

#include "header/model/sg_models/exciter_model/SEXS_test.h"
#include "header/model/sg_models/exciter_model/IEEET1_test.h"
#include "header/model/sg_models/exciter_model/PSASPE1_test.h"
#include "header/model/sg_models/exciter_model/PSASPE2_test.h"
#include "header/model/sg_models/exciter_model/CSEET1_test.h"
#include "header/model/sg_models/exciter_model/CSEET2_test.h"
#include "header/model/sg_models/exciter_model/PSASPE13_test.h"

#include "header/model/sg_models/stabilizer_model/stabilizer_model_test.h"
#include "header/model/sg_models/stabilizer_model/IEE2ST_test.h"
#include "header/model/sg_models/stabilizer_model/PSASPS1_test.h"
#include "header/model/sg_models/stabilizer_model/PSASPS2_test.h"
#include "header/model/sg_models/stabilizer_model/PSASPS3_test.h"
#include "header/model/sg_models/stabilizer_model/PSASPS4_test.h"
#include "header/model/sg_models/stabilizer_model/PSASPS5_test.h"

#include "header/model/sg_models/turbine_governor_model/TGOV1_test.h"
#include "header/model/sg_models/turbine_governor_model/IEEEG1_test.h"
#include "header/model/sg_models/turbine_governor_model/IEEEG2_test.h"
#include "header/model/sg_models/turbine_governor_model/IEEEG3_test.h"
#include "header/model/sg_models/turbine_governor_model/IEESGO_test.h"
#include "header/model/sg_models/turbine_governor_model/IEEEG1SB_test.h"

#include "header/model/sg_models/turbine_load_controller_model/lcfb1_test.h"

#include "header/model/load_model/load_model_test.h"
#include "header/model/load_model/IEEL_test.h"

#include "header/model/load_relay_model/UVLS_test.h"
#include "header/model/load_relay_model/UFLS_test.h"
#include "header/model/load_relay_model/PUFLS_test.h"

#include "header/model/hvdc_model/vdcol_test.h"
#include "header/model/hvdc_model/CDC4T_test.h"
#include "header/model/hvdc_model/CDC6T_test.h"


#include "header/model/wtg_models/wt_generator_model/wt3g1_test.h"
#include "header/model/wtg_models/wt_generator_model/wt3g0_test.h"

#include "header/model/wtg_models/wt_aerodynamic_model/aerd0_test.h"

#include "header/model/wtg_models/wt_turbine_model/wt3t0_test.h"

#include "header/model/wtg_models/wt_electrical_model/wt3e0_test.h"

#include "header/model/wtg_models/wt_pitch_model/wt3p0_test.h"

#include "header/model/wtg_models/wind_speed_model/filewind_test.h"

#include "header/toolkit/cct_searcher/cct_searcher_test.h"

#include "header/toolkit/powerflow_case_generator/powerflow_case_generator_test.h"



#include "header/steps_namespace.h"

using namespace std;





enum OutputType
{
	Compiler,
	Html,
	TextTerse,
	TextVerbose
};

static void usage()
{
	cout << "usage: mytest [MODE]\n"
         << "where MODE may be one of:\n"
         << "  --compiler\n"
         << "  --html\n"
         << "  --text-terse (default)\n"
         << "  --text-verbose\n";
	exit(0);
}

static unique_ptr<Test::Output> cmdline(int argc, char* argv[])
{
	if (argc > 2)
        usage(); // will not return

	Test::Output* output = 0;

	if (argc == 1)
        //output =  new Test::HtmlOutput; // only used for gdb
        output = new Test::TextOutput(Test::TextOutput::Verbose);
	else
	{
        const char* arg = argv[1];
        if (strcmp(arg, "--compiler") == 0)
        	output = new Test::CompilerOutput;
        else if (strcmp(arg, "--html") == 0)
        	output =  new Test::HtmlOutput;
        else if (strcmp(arg, "--text-terse") == 0)
        	output = new Test::TextOutput(Test::TextOutput::Terse);
        else if (strcmp(arg, "--text-verbose") == 0)
        	output = new Test::TextOutput(Test::TextOutput::Verbose);
        else
        {
        	cout << "invalid commandline argument: " << arg << endl;
        	usage(); // will not return
        }
	}

	return unique_ptr<Test::Output>(output);
}


// Main test program
//
int main(int argc, char* argv[])
{
    initialize_simulator(); // this function should be called first

	try
	{
        Test::Suite ts;
/*
        ts.add(unique_ptr<Test::Suite>(new TERMINAL_TEST));
        ts.add(unique_ptr<Test::Suite>(new DEVICE_ID_TEST));
        ts.add(unique_ptr<Test::Suite>(new AREA_TEST));
        ts.add(unique_ptr<Test::Suite>(new ZONE_TEST));
        ts.add(unique_ptr<Test::Suite>(new OWNER_TEST));
        ts.add(unique_ptr<Test::Suite>(new OWNERSHIP_TEST));
        ts.add(unique_ptr<Test::Suite>(new RATING_TEST));
        ts.add(unique_ptr<Test::Suite>(new UTILITY_TEST));
        ts.add(unique_ptr<Test::Suite>(new SPARSE_MATRIX_TEST));
        ts.add(unique_ptr<Test::Suite>(new FAULT_TEST));
        ts.add(unique_ptr<Test::Suite>(new BUS_INDEX_TEST));
        ts.add(unique_ptr<Test::Suite>(new DEVICE_INDEX_MAP_TEST));
        ts.add(unique_ptr<Test::Suite>(new INPHNO_TEST));
        ts.add(unique_ptr<Test::Suite>(new TIMER_TEST));

        //
        ts.add(unique_ptr<Test::Suite>(new BUS_TEST));
        ts.add(unique_ptr<Test::Suite>(new SOURCE_TEST));
        ts.add(unique_ptr<Test::Suite>(new GENERATOR_TEST));
        ts.add(unique_ptr<Test::Suite>(new WT_GENERATOR_TEST));
        ts.add(unique_ptr<Test::Suite>(new PV_UNIT_TEST));
        ts.add(unique_ptr<Test::Suite>(new LOAD_TEST));
        ts.add(unique_ptr<Test::Suite>(new LINE_TEST));
        ts.add(unique_ptr<Test::Suite>(new TRANSFORMER_TEST));
        ts.add(unique_ptr<Test::Suite>(new FIXED_SHUNT_TEST));

        ts.add(unique_ptr<Test::Suite>(new HVDC_TEST));

        ts.add(unique_ptr<Test::Suite>(new EQUIVALENT_DEVICE_TEST));

        ts.add(unique_ptr<Test::Suite>(new ENERGY_STORAGE_TEST));

        ts.add(unique_ptr<Test::Suite>(new POWER_SYSTEM_DATABASE_TEST));


        ts.add(unique_ptr<Test::Suite>(new STEPS_IMEXPORTER_TEST));

        ts.add(unique_ptr<Test::Suite>(new PSSE_IMEXPORTER_TEST));

        ts.add(unique_ptr<Test::Suite>(new BPA_IMEXPORTER_TEST));

        //ts.add(unique_ptr<Test::Suite>(new EQUIVALENT_MODEL_IMEXPORTER_TEST));


        ts.add(unique_ptr<Test::Suite>(new NETWORK_MATRIX_TEST));


        ts.add(unique_ptr<Test::Suite>(new JACOBIAN_BUILDER_TEST));*/


        //ts.add(unique_ptr<Test::Suite>(new POWERFLOW_SOLVER_TEST));
/*
        ts.add(unique_ptr<Test::Suite>(new PROPORTIONAL_BLOCK_TEST));
        ts.add(unique_ptr<Test::Suite>(new INTEGRAL_BLOCK_TEST));
        ts.add(unique_ptr<Test::Suite>(new DIFFERENTIAL_BLOCK_TEST));
        ts.add(unique_ptr<Test::Suite>(new FIRST_ORDER_BLOCK_TEST));
        ts.add(unique_ptr<Test::Suite>(new LEAD_LAG_BLOCK_TEST));
        ts.add(unique_ptr<Test::Suite>(new PID_BLOCK_TEST));
        ts.add(unique_ptr<Test::Suite>(new PI_BLOCK_TEST));
        ts.add(unique_ptr<Test::Suite>(new PD_BLOCK_TEST));
        ts.add(unique_ptr<Test::Suite>(new SATURATION_BLOCK_TEST));

        ts.add(unique_ptr<Test::Suite>(new CONTINUOUS_BUFFER_TEST));
        ts.add(unique_ptr<Test::Suite>(new METER_TEST));
        ts.add(unique_ptr<Test::Suite>(new METER_SETTER_TEST));

        ts.add(unique_ptr<Test::Suite>(new MODEL_VAR_TABLE_TEST));

        ts.add(unique_ptr<Test::Suite>(new MODEL_TEST));

        ts.add(unique_ptr<Test::Suite>(new GENCLS_TEST));
        ts.add(unique_ptr<Test::Suite>(new GENROU_TEST));
        ts.add(unique_ptr<Test::Suite>(new GENSAL_TEST));

        ts.add(unique_ptr<Test::Suite>(new COMP_TEST));

        ts.add(unique_ptr<Test::Suite>(new SEXS_TEST));
        ts.add(unique_ptr<Test::Suite>(new IEEET1_TEST));
        ts.add(unique_ptr<Test::Suite>(new PSASPE1_TEST));
        //ts.add(unique_ptr<Test::Suite>(new PSASPE2_TEST));
        ts.add(unique_ptr<Test::Suite>(new CSEET1_TEST));
        ts.add(unique_ptr<Test::Suite>(new CSEET2_TEST));
        ts.add(unique_ptr<Test::Suite>(new PSASPE13_TEST));

        ts.add(unique_ptr<Test::Suite>(new IEE2ST_TEST));
        ts.add(unique_ptr<Test::Suite>(new PSASPS1_TEST));*/
        ts.add(unique_ptr<Test::Suite>(new PSASPS2_TEST));
        ts.add(unique_ptr<Test::Suite>(new PSASPS3_TEST));/*
        ts.add(unique_ptr<Test::Suite>(new PSASPS4_TEST));
        ts.add(unique_ptr<Test::Suite>(new PSASPS5_TEST));

        ts.add(unique_ptr<Test::Suite>(new TGOV1_TEST));
        ts.add(unique_ptr<Test::Suite>(new IEEEG1_TEST));
        ts.add(unique_ptr<Test::Suite>(new IEEEG2_TEST));
        ts.add(unique_ptr<Test::Suite>(new IEEEG3_TEST));
        ts.add(unique_ptr<Test::Suite>(new IEESGO_TEST));
        ts.add(unique_ptr<Test::Suite>(new IEEEG1SB_TEST));

        ts.add(unique_ptr<Test::Suite>(new LCFB1_TEST));

        ts.add(unique_ptr<Test::Suite>(new IEEL_TEST));
        ts.add(unique_ptr<Test::Suite>(new UVLS_TEST));
        ts.add(unique_ptr<Test::Suite>(new UFLS_TEST));
        ts.add(unique_ptr<Test::Suite>(new PUFLS_TEST));

        ts.add(unique_ptr<Test::Suite>(new VDCOL_TEST));
        ts.add(unique_ptr<Test::Suite>(new CDC4T_TEST));
        ts.add(unique_ptr<Test::Suite>(new CDC6T_TEST));
*/
        /*ts.add(unique_ptr<Test::Suite>(new WT3G1_TEST));
        ts.add(unique_ptr<Test::Suite>(new WT3G0_TEST));
        ts.add(unique_ptr<Test::Suite>(new AERD0_TEST));
        ts.add(unique_ptr<Test::Suite>(new WT3T0_TEST));
        ts.add(unique_ptr<Test::Suite>(new WT3E0_TEST));
        ts.add(unique_ptr<Test::Suite>(new WT3P0_TEST));
        ts.add(unique_ptr<Test::Suite>(new FILEWIND_TEST));*/

        ts.add(unique_ptr<Test::Suite>(new DYNAMICS_SIMULATOR_TEST));
        //ts.add(unique_ptr<Test::Suite>(new CCT_SEARCHER_TEST));
        //ts.add(unique_ptr<Test::Suite>(new POWERFLOW_CASE_GENERATOR_TEST));

        // Run the tests
        unique_ptr<Test::Output> output(cmdline(argc, argv));
        ts.run(*output, true);

        redirect_stdout_to_file("STEPS_test_report.html");
        Test::HtmlOutput* const html = dynamic_cast<Test::HtmlOutput*>(output.get());
        if (html)
        	html->generate(cout, true, "STEPS");
        recover_stdout();
	}
	catch (...)
	{
        cout << "unexpected exception encountered\n";
        return EXIT_FAILURE;
	}
	terminate_simulator();
	return EXIT_SUCCESS;
}
