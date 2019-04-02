#coding=utf-8
import time
from multiprocessing import Pool, Process
import func
   
if __name__ =='__main__':   
    processes = []
    for i in range(1,10):
        p = Process(target=func.run, args=(i,))
        processes.append(p)
        p.start()
    for p in processes: # wait all processes to finish before execute new codes
        p.join()
