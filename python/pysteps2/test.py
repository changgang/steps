import pysteps as p
a = p.STEPS()
b = p.pSTEPS()

a.load_powerflow_data("IEEE9.raw","PSSE")
print(a.get_bus_count())
print(b.get_bus_count())
