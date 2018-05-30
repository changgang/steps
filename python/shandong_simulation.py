# File:"E:\Klive\Works\STEPS\new_version\bin\Release\shandong_simulation.py", generated on THU, JAN 18 2018  22:03, release 33.05.02
psspy.read(0,r"""E:\Klive\Works\STEPS\new_version\bin\Release\bench_shandong.raw""")
psspy.dyre_new([1,1,1,1],r"""E:\Klive\Works\STEPS\new_version\bin\Release\bench_shandong_with_avr.dyr""","","","")
psspy.fnsl([0,0,0,1,1,0,99,0])
psspy.dynamics_solution_param_2([200,_i,_i,_i,_i,_i,_i,_i],[_f,_f, 0.002,_f,_f,_f,_f,_f])


ierr = psspy.ini2dc()
while True:
    ierr, hvdc_name = psspy.nxt2dc()
    if ierr!=0:
        break
    hvdc = hvdc_name.strip()
    
    ierr, ibus = psspy.dc2int_2(hvdc_name, "RECT")
    ierr = psspy.voltage_channel([-1, -1, -1, ibus], "V_BUS_"+str(ibus))
    ierr, ibus = psspy.dc2int_2(hvdc_name, "INV")
    ierr = psspy.voltage_channel([-1, -1, -1, ibus], "V_BUS_"+str(ibus))
    
    ierr, var = psspy.dc2mind(hvdc_name, "VAR")    
    
    psspy.var_channel([-1,var+1],hvdc+"T_UNBLK_UNBP")
    psspy.var_channel([-1,var+2],hvdc+"VRAMPING")
    psspy.var_channel([-1,var+3],hvdc+"IRAMPING")
    psspy.var_channel([-1,var+4],hvdc+"VDC_COMP")
    psspy.var_channel([-1,var+5],hvdc+"PAC_REC")
    psspy.var_channel([-1,var+6],hvdc+"QAC_REC")
    psspy.var_channel([-1,var+7],hvdc+"PAC_INV")
    psspy.var_channel([-1,var+8],hvdc+"QAC_INV")
    psspy.var_channel([-1,var+9],hvdc+"VDC_INV")
    psspy.var_channel([-1,var+10],hvdc+"VDC_REC")
    psspy.var_channel([-1,var+11],hvdc+"IDC")
    psspy.var_channel([-1,var+12],hvdc+"ALPHA")
    psspy.var_channel([-1,var+13],hvdc+"GAMMA")
    psspy.var_channel([-1,var+14],hvdc+"T_RESWITCH")

psspy.cong(0)
psspy.conl(0,1,1,[0,0],[ 100.0,0.0,0.0, 100.0])
psspy.conl(0,1,2,[0,0],[ 100.0,0.0,0.0, 100.0])
psspy.conl(0,1,3,[0,0],[ 100.0,0.0,0.0, 100.0])
psspy.strt(0,"bench_shandong_fault_82_60.out")
psspy.run(0, 1.0,9999,1,0)
psspy.dist_branch_fault(82,60,r"""1""",1, 525.0,[0.0,-0.2E+3])
psspy.run(0, 1.1,9999,1,0)
psspy.dist_clear_fault(1)
psspy.dist_branch_trip(82,60,r"""1""")
psspy.run(0, 1.5,9999,1,0)
