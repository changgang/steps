import csv
import numpy
import matplotlib
import matplotlib.pyplot as pyplot

time = []
data = []
with open('data.csv',mode='r') as csvfile:
    names = csvfile.readline();
    names = names[0:-1]
    names = names.split(', ')
    names = names[1:-1]
    datareader = csv.reader(csvfile,delimiter=',', quoting=csv.QUOTE_NONNUMERIC)
    for row in datareader:
        time.append(row[0])
        data.append(row[1:-1])

npdata = numpy.array(data)

#pyplot.figure('ps figure',figsize=(4,3),dpi=300)
fig = pyplot.figure()
i=2
line = pyplot.plot(time,npdata[:,i],label=names[i],color='red')
xlab = pyplot.xlabel('time /s')
ylab = pyplot.ylabel('value')
title = pyplot.title('ps figure')
pyplot.xlim(0, 2.0)
lgd = pyplot.legend()

axs = pyplot.gca()

#all attributes can be found via pyplot.getp(object)
pyplot.setp(title,fontname='times new roman',fontsize=16)
pyplot.setp(xlab, fontname='times new roman', fontsize=14)
pyplot.setp(ylab, fontname='times new roman', fontsize=14)
pyplot.setp(lgd.texts,  fontname='times new roman', fontsize=14,color='r')

pyplot.show()
#pyplot.savefig('speed.png',dpi=200)

fig = pyplot.figure(2)

axs1 = pyplot.subplot(2,2,1)
axs2 = pyplot.subplot(2,2,2)
axs3 = pyplot.subplot(2,2,3)
axs4 = pyplot.subplot(2,2,4)

pyplot.sca(axs1)

i=1
line = pyplot.plot(time,npdata[:,i],label=names[i],color='red')
xlab = pyplot.xlabel('time /s')
ylab = pyplot.ylabel('value')
pyplot.xlim(0, 2.0)
lgd = pyplot.legend()

pyplot.setp(xlab, fontname='times new roman', fontsize=14)
pyplot.setp(ylab, fontname='times new roman', fontsize=14)
pyplot.setp(lgd.texts,  fontname='times new roman', fontsize=14,color='r', frame_on=False)

pyplot.sca(axs2)

i=2
line = pyplot.plot(time,npdata[:,i],label=names[i],color='blue')
xlab = pyplot.xlabel('time /s')
ylab = pyplot.ylabel('value')
pyplot.xlim(0, 2.0)
lgd = pyplot.legend()

pyplot.setp(xlab, fontname='times new roman', fontsize=14)
pyplot.setp(ylab, fontname='times new roman', fontsize=14)
pyplot.setp(lgd.texts,  fontname='times new roman', fontsize=14,color='b', frame_on=False)

pyplot.sca(axs3)

i=3
line = pyplot.plot(time,npdata[:,i],label=names[i],color='green')
xlab = pyplot.xlabel('time /s')
ylab = pyplot.ylabel('value')
pyplot.xlim(0, 2.0)
lgd = pyplot.legend()

pyplot.setp(xlab, fontname='times new roman', fontsize=14)
pyplot.setp(ylab, fontname='times new roman', fontsize=14)
pyplot.setp(lgd.texts,  fontname='times new roman', fontsize=14,color='g', frame_on=False)

pyplot.sca(axs4)

i=5
line = pyplot.plot(time,npdata[:,i],label=names[i],color='black')
xlab = pyplot.xlabel('time /s')
ylab = pyplot.ylabel('value')
pyplot.xlim(0, 2.0)
lgd = pyplot.legend()

pyplot.setp(xlab, fontname='times new roman', fontsize=14)
pyplot.setp(ylab, fontname='times new roman', fontsize=14)
pyplot.setp(lgd.texts,  fontname='times new roman', fontsize=14,color='k', frame_on=False)



#all attributes can be found via pyplot.getp(object)

pyplot.show()
#pyplot.savefig('speed.png',dpi=200)



