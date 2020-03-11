#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "thirdparty/cpptest/cpptest.h"

#include "header/basic/model_index.h"
#include "header/basic/utility.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

class MODEL_INDEX_TEST : public Test::Suite
{
    public:
        MODEL_INDEX_TEST()
        {
            cout<<get_system_time_stamp_string()<<" "<<"Testing class MODEL_INDEX."<<endl;
            TEST_ADD(MODEL_INDEX_TEST::test_constructor);
            TEST_ADD(MODEL_INDEX_TEST::test_set_get_device_id);
            TEST_ADD(MODEL_INDEX_TEST::test_get_device_index);
            TEST_ADD(MODEL_INDEX_TEST::test_set_get_icon_index);
            TEST_ADD(MODEL_INDEX_TEST::test_set_get_con_index);
            TEST_ADD(MODEL_INDEX_TEST::test_set_get_state_index);
            TEST_ADD(MODEL_INDEX_TEST::test_set_get_var_index);
        }
    protected:
        virtual void setup()
        {
            myObj = new MODEL_INDEX;
        }
        virtual void tear_down()
        {
            delete myObj;
            myObj = NULL;

            show_test_end_information();
        }
    private:
        void test_constructor()
        {
            TEST_ASSERT(myObj->get_device_id().get_device_terminal().get_bus_count()==0);
            TEST_ASSERT(myObj->get_device_index()==-1);
            TEST_ASSERT(myObj->get_icon_index()==-1);
            TEST_ASSERT(myObj->get_con_index()==-1);
            TEST_ASSERT(myObj->get_state_index()==-1);
            TEST_ASSERT(myObj->get_var_index()==-1);
        }
        void test_set_get_device_id()
        {
            TERMINAL buses = TERMINAL();
            buses.append_bus(1);

            DEVICE_ID device_id = DEVICE_ID();
            device_id.set_device_type("发电机");
            device_id.set_device_terminal(buses);
            device_id.set_device_identifier("1#");

            myObj->set_device_id(device_id);

            DEVICE_ID new_device_id =myObj->get_device_id();
            TEST_ASSERT(new_device_id.get_device_type()=="发电机");
            TERMINAL new_terminal = new_device_id.get_device_terminal();
            vector<size_t> new_buses = new_terminal.get_buses();

            TEST_ASSERT(new_terminal.get_bus_count()==1);
            TEST_ASSERT(new_buses[0]==1);
            TEST_ASSERT(new_device_id.get_device_identifier()=="1#");
        }

        void test_get_device_index()
        {
            TERMINAL buses = TERMINAL();
            buses.append_bus(1);

            DEVICE_ID device_id = DEVICE_ID();
            device_id.set_device_type("发电机");
            device_id.set_device_terminal(buses);
            device_id.set_device_identifier("1#");

            myObj->set_device_id(device_id);
            TEST_ASSERT(myObj->get_device_index()==2);

            device_id.set_device_type("Load");
            device_id.set_device_terminal(buses);
            device_id.set_device_identifier("1#");

            myObj->set_device_id(device_id);
            TEST_ASSERT(myObj->get_device_index()==3);


            device_id.set_device_type("Line");
            buses.append_bus(2);
            device_id.set_device_terminal(buses);
            device_id.set_device_identifier("1#");

            myObj->set_device_id(device_id);
            TEST_ASSERT(myObj->get_device_index()==4);

        }

        void test_set_get_icon_index()
        {
            myObj->set_icon_index(1);
            TEST_ASSERT(myObj->get_icon_index()==1);
            myObj->set_icon_index(-2);
            TEST_ASSERT(myObj->get_icon_index()==-1);
        }
        void test_set_get_con_index()
        {
            myObj->set_con_index(1);
            TEST_ASSERT(myObj->get_con_index()==1);
            myObj->set_con_index(-2);
            TEST_ASSERT(myObj->get_con_index()==-1);
        }
        void test_set_get_state_index()
        {
            myObj->set_state_index(1);
            TEST_ASSERT(myObj->get_state_index()==1);
            myObj->set_state_index(-2);
            TEST_ASSERT(myObj->get_state_index()==-1);
        }
        void test_set_get_var_index()
        {
            myObj->set_var_index(1);
            TEST_ASSERT(myObj->get_var_index()==1);
            myObj->set_var_index(-2);
            TEST_ASSERT(myObj->get_var_index()==-1);
        }

    private:
        MODEL_INDEX* myObj;
};
