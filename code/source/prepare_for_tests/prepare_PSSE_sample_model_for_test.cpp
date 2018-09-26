#include "header/prepare_for_tests/prepare_models_for_test.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"

void prepare_psse_sample_model(POWER_SYSTEM_DATABASE* db)
{
    ostringstream osstream;
    if(db==NULL)
    {
        osstream<<"NULL power system database is provided for preparing PSS/E sample model."<<endl
          <<"Model will not be prepared.";
        show_information_with_leading_time_stamp(osstream);
        return;
    }
    prepare_psse_sample_model_buses(db);
    prepare_psse_sample_model_generators(db);
    prepare_psse_sample_model_loads(db);
    prepare_psse_sample_model_lines(db);
    prepare_psse_sample_model_transformers(db);
    prepare_psse_sample_model_fixed_shunts(db);
    prepare_psse_sample_model_switched_shunts(db);
    prepare_psse_sample_model_hvdcs(db);
    prepare_psse_sample_model_areas(db);
    prepare_psse_sample_model_zones(db);
    prepare_psse_sample_model_owners(db);
}

void prepare_psse_sample_model_buses(POWER_SYSTEM_DATABASE* db)
{
    db->set_allowed_max_bus_number(100000);

    BUS bus;
    bus.set_bus_number(101);
    bus.set_bus_name("NUC-A");
    bus.set_base_voltage_in_kV(21.6);
    bus.set_area_number(1);
    bus.set_zone_number(1);
    bus.set_owner_number(1);
    bus.set_bus_type(PV_TYPE);
    bus.set_voltage_in_pu(1.01);
    bus.set_angle_in_deg(-10.42868);

    db->append_bus(bus);

    bus.set_bus_number(102);
    bus.set_bus_name("NUC-B");
    bus.set_angle_in_deg(-10.78062);

    db->append_bus(bus);

    bus.set_bus_number(151);
    bus.set_bus_name("NUCPLNT");
    bus.set_base_voltage_in_kV(500.0);
    bus.set_bus_type(PQ_TYPE);
    bus.set_voltage_in_pu(1.002194);
    bus.set_angle_in_deg(-13.56857);

    db->append_bus(bus);


    bus.set_bus_number(152);
    bus.set_bus_name("MID500");
    bus.set_base_voltage_in_kV(500.0);
    bus.set_zone_number(2);
    bus.set_bus_type(PQ_TYPE);
    bus.set_voltage_in_pu(1.043773);
    bus.set_angle_in_deg(-23.44061);

    db->append_bus(bus);

    bus.set_bus_number(153);
    bus.set_bus_name("MID230");
    bus.set_base_voltage_in_kV(230.0);
    bus.set_area_number(1);
    bus.set_zone_number(2);
    bus.set_owner_number(1);
    bus.set_bus_type(PQ_TYPE);
    bus.set_voltage_in_pu(1.057206);
    bus.set_angle_in_deg(-25.19197);

    db->append_bus(bus);

    bus.set_bus_number(154);
    bus.set_bus_name("DOWNTN");
    bus.set_base_voltage_in_kV(230.0);
    bus.set_area_number(1);
    bus.set_zone_number(3);
    bus.set_owner_number(1);
    bus.set_bus_type(PQ_TYPE);
    bus.set_voltage_in_pu(0.9918512);
    bus.set_angle_in_deg(-32.48673);

    db->append_bus(bus);

    bus.set_bus_number(155);
    bus.set_bus_name("FACTS TE");
    bus.set_base_voltage_in_kV(230.0);
    bus.set_area_number(1);
    bus.set_zone_number(4);
    bus.set_owner_number(1);
    bus.set_bus_type(PQ_TYPE);
    bus.set_voltage_in_pu(1.017227);
    bus.set_angle_in_deg(-23.64);

    db->append_bus(bus);

    bus.set_bus_number(201);
    bus.set_bus_name("HYDRO");
    bus.set_base_voltage_in_kV(500.0);
    bus.set_area_number(2);
    bus.set_zone_number(7);
    bus.set_owner_number(2);
    bus.set_bus_type(PQ_TYPE);
    bus.set_voltage_in_pu(0.9900665);
    bus.set_angle_in_deg(-18.71196);

    db->append_bus(bus);

    bus.set_bus_number(202);
    bus.set_bus_name("EAST500");
    bus.set_base_voltage_in_kV(500.0);
    bus.set_area_number(2);
    bus.set_zone_number(2);
    bus.set_owner_number(2);
    bus.set_bus_type(PQ_TYPE);
    bus.set_voltage_in_pu(1.02108);
    bus.set_angle_in_deg(-25.67049);

    db->append_bus(bus);
}
void prepare_psse_sample_model_generators(POWER_SYSTEM_DATABASE* db)
{
    GENERATOR gen(db);
    db->append_generator(gen);
}

void prepare_psse_sample_model_loads(POWER_SYSTEM_DATABASE* db)
{
    LOAD load(db);
    db->append_load(load);
}

void prepare_psse_sample_model_lines(POWER_SYSTEM_DATABASE* db)
{
    LINE line(db);
    db->append_line(line);
}

void prepare_psse_sample_model_transformers(POWER_SYSTEM_DATABASE* db)
{
    TRANSFORMER trans(db);
    db->append_transformer(trans);
}

void prepare_psse_sample_model_fixed_shunts(POWER_SYSTEM_DATABASE* db)
{
    FIXED_SHUNT shunt(db);
    db->append_fixed_shunt(shunt);
}

void prepare_psse_sample_model_switched_shunts(POWER_SYSTEM_DATABASE* db)
{
    ostringstream osstream;
    osstream<<"Switched shunt is not supported.  No Switched shunt will be added to sample model of "<<db->get_system_name()<<endl;
    show_information_with_leading_time_stamp(osstream);
}

void prepare_psse_sample_model_hvdcs(POWER_SYSTEM_DATABASE* db)
{
    HVDC hvdc(db);
    db->append_hvdc(hvdc);
}

void prepare_psse_sample_model_areas(POWER_SYSTEM_DATABASE* db)
{
    AREA area(db);
    area.set_area_number(1);
    area.set_area_name("AREA");
    db->append_area(area);
}

void prepare_psse_sample_model_zones(POWER_SYSTEM_DATABASE* db)
{
    ZONE zone(db);
    zone.set_zone_number(1);
    zone.set_zone_name("ZONE");
    db->append_zone(zone);
}

void prepare_psse_sample_model_owners(POWER_SYSTEM_DATABASE* db)
{
    OWNER owner(db);
    db->append_owner(owner);
}

