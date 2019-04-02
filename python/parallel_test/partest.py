#coding=utf-8
import time
from multiprocessing import Pool, Process
import func
   
if __name__ =='__main__':   
    for i in range(1,100):
        p = Process(target=func.run, args=(i,))
        p.start()
    p.join()
