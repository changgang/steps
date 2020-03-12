from xpinyin import Pinyin
from stepspy import STEPS

p = Pinyin()

sim = STEPS(is_default=True, log_file="log")
sim.set_allowed_maximum_bus_number(1000000)

file="sd2010.raw"
sim.load_powerflow_data(file, "PSSE")

buses = sim.get_all_buses()
for bus in buses:
    name = sim.get_bus_data(bus, "S", "name")
    newname = p.get_pinyin(name[0])+"-"+p.get_initials(name[1:],"")
    newname = newname.upper()
    sim.set_bus_data(bus, "S", "name", newname)
    
sim.save_powerflow_data(file+".new", "PSSE")