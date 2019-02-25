#coding=utf-8

import pouch
import firmatter
import matplotlib.pyplot as pyplot
import numpy

datafile = 'ieee39_model_PUFLS_trip_generator_39_old'
timeold, valueold, channel = pouch.POUCH_CSV(datafile+'.csv')

datafile = 'ieee39_model_PUFLS_trip_generator_39'
timenew, valuenew, channel = pouch.POUCH_CSV(datafile+'.csv')

pyplot.figure()
chan_index = 2
pyplot.plot(timeold,valueold[:,chan_index], color='r', linestyle='-', linewidth=0.5, label='old')
pyplot.plot(timenew,valuenew[:,chan_index], color='g', linestyle='-', linewidth=0.5, label='new')
#pyplot.xlim(0.0, max(timeold))
#pyplot.ylim(0.0, 180.0)
pyplot.xlabel('Time/s')
#pyplot.ylabel('Angle/deg')
pyplot.legend(loc='best')

pyplot.show()



pyplot.figure()
chan_index = 3*26+6-1
pyplot.plot(timeold,valueold[:,chan_index], color='r', linestyle='-', linewidth=0.5, label='old')
pyplot.plot(timenew,valuenew[:,chan_index], color='g', linestyle='-', linewidth=0.5, label='new')
#pyplot.xlim(0.0, max(timeold))
#pyplot.ylim(0.0, 180.0)
pyplot.xlabel('Time/s')
#pyplot.ylabel('Angle/deg')
pyplot.legend(loc='best')

pyplot.show()


pyplot.figure()
chan_index = 1*26+20-1
pyplot.plot(timeold,valueold[:,chan_index], color='r', linestyle='-', linewidth=0.5, label='old')
pyplot.plot(timenew,valuenew[:,chan_index], color='g', linestyle='-', linewidth=0.5, label='new')
#pyplot.xlim(0.0, max(timeold))
#pyplot.ylim(0.0, 180.0)
pyplot.xlabel('Time/s')
#pyplot.ylabel('Angle/deg')
pyplot.legend(loc='best')

pyplot.show()


