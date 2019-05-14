1, Copy folder pysteps2/ to PYTHONPATH/Lib/site-packages/
2, If you are using 32 bit Python, rename file pysteps2/libsteps/libSTEPS32.dll to libSTEPS.dll
3, If you are using 64 bit Python, rename file pysteps2/libsteps/libSTEPS64.dll to libSTEPS.dll
4, If the VC runtime is missing, download and install Microsoft Visual C++ 2017 Redistributable of 32 or 64 bit version.
5, To import the pysteps2, use 
    import pysteps2
   or
    from pysteps2 import STEPS