
def prepare_bus_channels(channel_type):
    if channel_type not in ["VOLT_ANGLE", "FREQ"]:
        return       
        
    ierr = psspy.inibus(0)
    while 1:
        ierr, ibus, busname = psspy.nxtbus()
        if ierr!=0:
            break
        if channel_type=="FREQ":
            psspy.bus_frequency_channel([-1,ibus],"")
            continue
        if channel_type =="VOLT_ANGLE":
            psspy.voltage_and_angle_channel([-1,-1,-1,ibus],["",""])
            
def prepare_machine_channels(channel_type):
    type = 0
    if channel_type == "ANGLE":
        type = 1
    if channel_type == "PELEC":
        type = 2
    if channel_type == "QELEC":
        type = 3
    if channel_type == "EFD":
        type = 5
    if channel_type == "PMECH":
        type = 6
    if channel_type == "SPEED":
        type = 7
    if type not in [1,2,3,5,6,7]:
        return       
        
    ierr = psspy.inibus(0)
    while 1:
        ierr, ibus, busname = psspy.nxtbus()
        if ierr!=0:
            break
        ierr = psspy.inimac(ibus)
        if ierr!=0:
            continue
        while 1:
            ierr, ickt = psspy.nxtmac(ibus)
            if ierr!=0:
                break
            psspy.machine_array_channel([-1, type, ibus],ickt, "")
    

def prepare_load_channels(channel_type):
    type = 0
    if channel_type == "PLOAD":
        type = 1
    if channel_type == "QLOAD":
        type = 2
    if type not in [1,2]:
        return       
        
    ierr = psspy.inibus(0)
    while 1:
        ierr, ibus, busname = psspy.nxtbus()
        if ierr!=0:
            break
        ierr = psspy.inilod(ibus)
        if ierr!=0:
            continue
        while 1:
            ierr, ickt = psspy.nxtlod(ibus)
            if ierr!=0:
                break
            psspy.load_array_channel([-1, type, ibus],ickt, "")  

def prepare_line_channels():        
    ierr = psspy.inibus(0)
    while 1:
        ierr, ibus, busname = psspy.nxtbus()
        if ierr!=0:
            break
        ierr = psspy.inibrn(ibus,1)
        if ierr!=0:
            continue
        while 1:
            ierr, jbus, ickt = psspy.nxtbrn(ibus)
            if ierr!=0:
                break
            ierr, rval = psspy.xfrdat(ibus, jbus, ickt, "RATIO")
            if ierr!=3:
                continue;
            
            ierr = psspy.branch_p_and_q_channel([-1,-1,-1, ibus, jbus], ickt, "")
            ierr = psspy.branch_p_and_q_channel([-1,-1,-1, jbus, ibus], ickt, "")


raw_file = "IEEE39.raw"
dyr_file = "IEEE39_GENROU.dyr"
out_file = "IEEE39_GENROU.out"

psspy.read(0,raw_file)
psspy.solution_parameters_4([_i,_i,_i,_i,10],[_f,_f,_f,_f,_f, 0.001,_f,_f,_f,_f,_f,_f,_f,_f,_f,_f,_f,_f,_f])
psspy.fnsl([0,0,0,1,1,0,99,0])
psspy.dyre_new([1,1,1,1],dyr_file,"","","")
#psspy.set_netfrq(1)
psspy.dynamics_solution_param_2([200,_i,_i,_i,_i,_i,_i,_i],[_f,_f,0.001,0.004,_f,_f,_f,_f])
psspy.cong(0)
psspy.conl(0,1,1,[0,0],[ 100.0,0.0,0.0, 100.0])
psspy.conl(0,1,2,[0,0],[ 100.0,0.0,0.0, 100.0])
psspy.conl(0,1,3,[0,0],[ 100.0,0.0,0.0, 100.0])

prepare_bus_channels("VOLT_ANGLE")
prepare_bus_channels("FREQ")
prepare_machine_channels("ANGLE")
prepare_machine_channels("SPEED")
prepare_machine_channels("EFD")
prepare_machine_channels("PMECH")
prepare_machine_channels("PELEC")
prepare_machine_channels("QELEC")
prepare_load_channels("PLOAD")
prepare_load_channels("QLOAD")
prepare_line_channels()

psspy.strt(0,out_file)
psspy.run(0,0.0,9999,1,0)

"""
psspy.dist_branch_fault(7,5,"1",1, 100.0,[0.0,-0.2E+10])
psspy.run(0, 0.083,9999,1,0)
psspy.dist_clear_fault(1)
psspy.dist_branch_trip(7,5,"1")
"""
psspy.dist_branch_fault(17,18,"1",1, 100.0,[0.0,-0.2E+10])
psspy.run(0, 0.5,9999,1,0)
psspy.dist_clear_fault(1)
psspy.dist_branch_trip(17,18,"1")

psspy.run(0, 5.0,9999,1,0)
