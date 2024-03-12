#include "header/prepare_for_tests/prepare_models_for_test.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"

void prepare_psse_sample_model()
{
    prepare_psse_sample_model_buses();
    prepare_psse_sample_model_generators();
    prepare_psse_sample_model_loads();
    prepare_psse_sample_model_lines();
    prepare_psse_sample_model_transformers();
    prepare_psse_sample_model_fixed_shunts();
    prepare_psse_sample_model_switched_shunts();
    prepare_psse_sample_model_2t_lcc_hvdcs();
    prepare_psse_sample_model_areas();
    prepare_psse_sample_model_zones();
    prepare_psse_sample_model_owners();
}

void prepare_psse_sample_model_buses()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    psdb.set_allowed_max_bus_number(100000);

    BUS bus(default_toolkit);
    bus.set_bus_number(101);
    bus.set_bus_name("NUC-A");
    bus.set_base_voltage_in_kV(21.6);
    bus.set_area_number(1);
    bus.set_zone_number(1);
    bus.set_owner_number(1);
    bus.set_bus_type(PV_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.01);
    bus.set_positive_sequence_angle_in_deg(-10.42868);

    psdb.append_bus(bus);

    bus.set_bus_number(102);
    bus.set_bus_name("NUC-B");
    bus.set_positive_sequence_angle_in_deg(-10.78062);

    psdb.append_bus(bus);

    bus.set_bus_number(151);
    bus.set_bus_name("NUCPLNT");
    bus.set_base_voltage_in_kV(500.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.002194);
    bus.set_positive_sequence_angle_in_deg(-13.56857);

    psdb.append_bus(bus);


    bus.set_bus_number(152);
    bus.set_bus_name("MID500");
    bus.set_base_voltage_in_kV(500.0);
    bus.set_zone_number(2);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.043773);
    bus.set_positive_sequence_angle_in_deg(-23.44061);

    psdb.append_bus(bus);

    bus.set_bus_number(153);
    bus.set_bus_name("MID230");
    bus.set_base_voltage_in_kV(230.0);
    bus.set_area_number(1);
    bus.set_zone_number(2);
    bus.set_owner_number(1);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.057206);
    bus.set_positive_sequence_angle_in_deg(-25.19197);

    psdb.append_bus(bus);

    bus.set_bus_number(154);
    bus.set_bus_name("DOWNTN");
    bus.set_base_voltage_in_kV(230.0);
    bus.set_area_number(1);
    bus.set_zone_number(3);
    bus.set_owner_number(1);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(0.9918512);
    bus.set_positive_sequence_angle_in_deg(-32.48673);

    psdb.append_bus(bus);

    bus.set_bus_number(155);
    bus.set_bus_name("FACTS TE");
    bus.set_base_voltage_in_kV(230.0);
    bus.set_area_number(1);
    bus.set_zone_number(4);
    bus.set_owner_number(1);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.017227);
    bus.set_positive_sequence_angle_in_deg(-23.64);

    psdb.append_bus(bus);

    bus.set_bus_number(201);
    bus.set_bus_name("HYDRO");
    bus.set_base_voltage_in_kV(500.0);
    bus.set_area_number(2);
    bus.set_zone_number(7);
    bus.set_owner_number(2);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(0.9900665);
    bus.set_positive_sequence_angle_in_deg(-18.71196);

    psdb.append_bus(bus);

    bus.set_bus_number(202);
    bus.set_bus_name("EAST500");
    bus.set_base_voltage_in_kV(500.0);
    bus.set_area_number(2);
    bus.set_zone_number(2);
    bus.set_owner_number(2);
    bus.set_bus_type(PQ_TYPE);
    bus.set_positive_sequence_voltage_in_pu(1.02108);
    bus.set_positive_sequence_angle_in_deg(-25.67049);

    psdb.append_bus(bus);
}
void prepare_psse_sample_model_generators()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    GENERATOR gen(default_toolkit);
    psdb.append_generator(gen);
}

void prepare_psse_sample_model_loads()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    LOAD load(default_toolkit);
    psdb.append_load(load);
}

void prepare_psse_sample_model_lines()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    LINE line(default_toolkit);
    psdb.append_line(line);
}

void prepare_psse_sample_model_transformers()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    TRANSFORMER trans(default_toolkit);
    psdb.append_transformer(trans);
}

void prepare_psse_sample_model_fixed_shunts()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    FIXED_SHUNT shunt(default_toolkit);
    psdb.append_fixed_shunt(shunt);
}

void prepare_psse_sample_model_switched_shunts()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    ostringstream osstream;
    osstream<<"Switched shunt is not supported.  No Switched shunt will be added to sample model of "<<psdb.get_system_name()<<endl;
    default_toolkit.show_information_with_leading_time_stamp(osstream);
}

void prepare_psse_sample_model_2t_lcc_hvdcs()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    LCC_HVDC2T hvdc(default_toolkit);
    psdb.append_2t_lcc_hvdc(hvdc);
}

void prepare_psse_sample_model_areas()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    AREA area(default_toolkit);
    area.set_area_number(1);
    area.set_area_name("AREA");
    psdb.append_area(area);
}

void prepare_psse_sample_model_zones()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    ZONE zone(default_toolkit);
    zone.set_zone_number(1);
    zone.set_zone_name("ZONE");
    psdb.append_zone(zone);
}

void prepare_psse_sample_model_owners()
{
    POWER_SYSTEM_DATABASE& psdb = default_toolkit.get_power_system_database();
    OWNER owner(default_toolkit);
    psdb.append_owner(owner);
}

