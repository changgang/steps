#coding=utf-8

import pouch
import firmatter
import matplotlib.pyplot as pyplot
import numpy

datafile = 'sm_pysteps_fault_cleared_0.103s'
time, value, channel = pouch.POUCH_CSV(datafile+'.csv')
for i, ch in enumerate(channel):
    print(str(i)+': '+ch)

pyplot.figure()
chan_index = 18
angle_diff = value[:,chan_index]-value[:,chan_index+1]
pyplot.plot(time,angle_diff, color='r', linestyle='-', linewidth=0.5, label='Generator@bus 1 v.s. bus 2')
pyplot.xlim(0.0, max(time))
pyplot.ylim(0.0, 180.0)
pyplot.xlabel('Time/s')
pyplot.ylabel('Angle/deg')
pyplot.legend(loc='best')
print(max(angle_diff))

formatter = firmatter.prepare_IEEE_half_page_formatter()
formatter.format(pyplot.gcf())
pyplot.savefig(datafile+'_machine_relative_angle.png',dpi=600)

pyplot.figure()
chan_index = 20
speed_diff = value[:,chan_index]-value[:,chan_index+1]
pyplot.plot(time,speed_diff, color='r', linestyle='-', linewidth=0.5, label='Generator@bus 1 v.s. bus 2')
pyplot.xlim(0.0, max(time))
#pyplot.ylim(0.0, 180.0)
pyplot.xlabel('Time/s')
pyplot.ylabel('Speed/pu')
pyplot.legend(loc='best')

formatter = firmatter.prepare_IEEE_half_page_formatter()
formatter.format(pyplot.gcf())
pyplot.savefig(datafile+'_machine_relative_speed.png',dpi=600)

pyplot.figure()
pyplot.plot(angle_diff,speed_diff, color='r', linestyle='-', linewidth=0.5, label='Generator@bus 1 v.s. bus 2')
pyplot.xlim(0.0, 180.0)
#pyplot.ylim(0.0, 180.0)
pyplot.xlabel('Angle/deg')
pyplot.ylabel('Speed/pu')
pyplot.legend(loc='best')

formatter = firmatter.prepare_IEEE_half_page_formatter()
formatter.format(pyplot.gcf())
pyplot.savefig(datafile+'_machine_relative_angle_speed.png',dpi=600)

#formatter = firmatter.prepare_IEEE_full_page_formatter()
#formatter.format(pyplot.gcf())
#pyplot.savefig('IEEE_full_formatter_test.png',dpi=600)


    
