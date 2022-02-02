#coding=utf-8
import time
import struct
import csv
import os.path
import math
import platform
try:
    import numpy
except ImportError:
    print("pouch is dependent on module numpy which is missing. please install numpy before use pouch")

def __is_filename_ends_with(filename, surfix):
    filename = filename.upper()
    surfix = surfix.upper()
    if filename.endswith(surfix):
        return True
    else:
        return False

def __save_data(file_name, dy_time, dy_value, dy_channel):
    with open(file_name,'wt') as fid:
        writer = csv.writer(fid)
        channels = ['Time'.encode('utf-8')]
        for chan in dy_channel:
            channels.append(chan.encode('utf-8'))
        writer.writerow(channels)
        for i in range(len(dy_time)):
            value = [dy_time[i]]
            for j in range(len(dy_channel)):
                value.append(dy_value[i,j])
            writer.writerow(value)
def __get_cpu_clock():
    major, minor, patch = platform.python_version().split(".")
    if int(major)>=3 and int(minor)>=9:
        return time.clock()
    else:
        return time.perf_counter()

def POUCH(file_name, type, save_or_not=False, show_log=True):
    type = type.upper()

    if type in ["CSV"]:
        return POUCH_CSV(file_name, show_log)
            
    if type in ["STEPS"]:
        return POUCH_STEPS(file_name, save_or_not, show_log)
    print("Power System Simulator type invalid")
    return numpy.array([]), numpy.array([]), []
        
def POUCH_CSV(file_name, show_log=True):
    """
    Usage:
        dy_time, dy_value, dy_channel = POUCH_CSV(file_name, show_log=True)
        file_name: csv file ending with '.csv'. the first line of the csv file should be channels' name quoted by '"'
        save_or_not: logic. True for saving to csv file, False for ignoring saving
        show_log: logic. True for showing log, False for diabling log
        dy_time: numpy array of simulation time
        dy_value: numpy array of values of all channels
        dy_channel: list of name of all channels
    """
    if show_log==True:
        info  = "CSV File Conversion Program V1.2.1 (2019/07/05)\n"
        info += "Supports: Common CSV files with header, or Power Factory 15.1\n"
        info += "Changgang Li (lichgang@sdu.ed.cn)"

        print(info)    
    
    if not os.path.exists(file_name):
        info = '**** There is no csv file '+file_name+'.\n**** Please check csv file.'
        print(info)
        return numpy.array([]), numpy.array([]), []
        
    dy_time = []
    dy_value = []
    dy_channel = []
    
    if not __is_filename_ends_with(file_name, '.csv'):
        info = '**** '+file_name+' is not ending with .csv. Please check CSV file name.'
        print(info)
        return numpy.array(dy_time), numpy.array(dy_value), dy_channel

    start_time = __get_cpu_clock()
    
    with open(file_name, 'rt') as fid:
        data = fid.readline()
        data = data[0:-1] # remove the last \n char
        data = data.replace('"','')
        channels = data.split(',')
        for i in range(len(channels)):
            channel = channels[i]
            channel = channel.strip('"')
            channel = channel.strip()
            channels[i] = channel
        
        dy_channel = channels[1:len(channels)+1]
        
        reader = csv.reader(fid, quoting=csv.QUOTE_NONNUMERIC)
        for data in reader:
            dy_time.append(data[0])
            dy_value.append(data[1:len(data)+1])
    if show_log==True:
        end_time = __get_cpu_clock()
        time_elapsed = end_time - start_time
        
        info = 'Conversion finished in '+str(float(int(time_elapsed*1000.0))*0.001)+'s'
        print(info)

    return numpy.array(dy_time), (numpy.array(dy_value)), dy_channel
        
def POUCH_STEPS(file_name, save_or_not=False, show_log=True):
    """
    Usage:
        dy_time, dy_value, dy_channel = POUCH_STEPS(file_name, save_or_not=False, show_log=True)
        file_name: STEPS binary file ending with '.bin'
        save_or_not: logic. True for saving to csv file, False for ignoring saving
        show_log: logic. True for showing log, False for diabling log
        dy_time: numpy array of simulation time
        dy_value: numpy array of values of all channels
        dy_channel: list of name of all channels
    """
    if show_log==True:
        info  = "STEPS Bin File Conversion Program V0.0.2 (2019/07/05)\n"
        info += "Supports: STEPS 20190416\n"
        info += "Changgang Li (lichgang@sdu.ed.cn)"
        print(info)
    
    if not os.path.exists(file_name):
        info = '**** There is no bin file '+file_name+'.\n**** Please check STEPS bin file.'
        print(info)
        return numpy.array([]), numpy.array([]), []
        
    dy_time = []
    dy_value = []
    dy_channel = []
    
    if not __is_filename_ends_with(file_name, '.bin'):
        info = '**** '+file_name+' is not ending with .bin. Please check STEPS bin file name.'
        print(info)
        return numpy.array(dy_time), numpy.array(dy_value), dy_channel

    start_time = __get_cpu_clock()
    
    fid = open(file_name, 'rb')
    steps_bin_version = fid.read(4)
    steps_bin_version = struct.unpack('I', steps_bin_version)
    steps_bin_version = steps_bin_version[0]
    fid.close()
    
    if steps_bin_version==0:
        dy_time, dy_value, dy_channel = __POUCH_STEPS_0(file_name, show_log)
        
    if save_or_not == True:
        file_name = file_name+'.csv'
        __save_data(file_name, dy_time, dy_value, dy_channel)
        
        
    end_time = __get_cpu_clock()
    time_elapsed = end_time - start_time
    if show_log==True:
        info = 'Conversion finished in '+str(float(int(time_elapsed*1000.0))*0.001)+'s'
        print(info)
    
    return dy_time, dy_value, dy_channel

def __POUCH_STEPS_0(file_name, show_log=True):
    dy_time = []
    dy_value = []
    dy_channel = []
    
    with open(file_name, 'rb') as fid:
        steps_bin_version = fid.read(4) # get STEPS version
        steps_bin_version = struct.unpack('I', steps_bin_version)
        steps_bin_version = steps_bin_version[0]
        
        case_time = fid.read(4*6) # get case time
        case_time = struct.unpack('6I', case_time)
        case_year = case_time[0]
        case_month = case_time[1]
        case_day = case_time[2]
        case_hour = case_time[3]
        case_minute = case_time[4]
        case_second = case_time[5]
        print(case_time)
        print(case_year, case_month, case_day, case_hour, case_minute, case_second)
        
        float_size = fid.read(4) # get float size
        float_size = struct.unpack('I',float_size)
        float_size = float_size[0]
        
        n_channels = fid.read(4) # get 4 bytes: channel count
        n_channels = struct.unpack('I',n_channels)
        n_channels = n_channels[0]
        n_channel_bytes = fid.read(4) # get 4 bytes: channel name bytes count
        n_channel_bytes = struct.unpack('I',n_channel_bytes)
        n_channel_bytes = n_channel_bytes[0]

        channels = fid.read(n_channel_bytes) # bytes of channel names
        channels = channels.decode("cp936")
        channels = channels.strip()

        channels = channels.split('\n')
        for i in range(1,n_channels):
            dy_channel.append(channels[i])
        
        while True: # loop for each time point
            data = fid.read(n_channels*float_size) # get values
            if len(data)<n_channels*float_size:
                break
            if float_size==4:
                data = struct.unpack('f'*n_channels, data)
            else:
                data = struct.unpack('d'*n_channels, data)
            t = data[0]
            value = data[1:n_channels]
            dy_time.append(t)
            dy_value.append(value)        

    return dy_time, dy_value, dy_channel
