# stepspy

> stepspy

stepspy is a Python module of Simulation Toolkit for Electrical Power Systems (STEPS).

## Table of Contents

- [Background](#background)
- [Release Note](#release-note)
- [Install](#install)
- [Usage](#usage)
- [Examples](#examples)
- [Maintainers](#maintainers)
- [Contributing](#contributing)
- [License](#license)

## Background

stepspy is a Python module of Simulation Toolkit for Electrical Power Systems (STEPS). It provides wrapper of APIs of STEPS in a dynamic library.

STEPS is a simulation toolkit for powerflow and dynamic simulation of large-scale power systems. It provides detailed models of bus, line, transformer, HVDC, generator, wind turbine generator, load, and fixed shunt. For more information about STEPS, see (https://github.com/changgang/steps).

## Realse Note

- 0.10.2. Nov. 13, 2019. Add new API get/set_hvdc_power_order_in_MW() for changing HVDC power order in dynamic simulation.
- 0.10.1. Oct. 29, 2019. Add new API load_powerflow_result() to load solved powerflow result for new non-flat start powerflow solution.
- 0.10.0. Oct. 26, 2019. Remove loading of libSTEPS-p.so/dll. Use libSTEPS.so/dll for building STEPS.
- 0.9.1. Oct. 21, 2019. Fix bug by changing c_unit to c_uint. Add warning to get_buses_with_constraints if old version is called.
- 0.9.0. Oct. 19, 2019. Add APIs save_powerflow_data_in_keep_mode/ordered_mode/optimized_mode. save_powerflow_data_in_optimized_mode() is suggested for improving performance. Add APIs of get_all_areas/zones/owners. Update get_buses_with_constraints.
- 0.8.6.1. Oct. 12, 2019. Change returned model name as None if dynamic model does not exist when calling get_xxx_related_model_name().
- 0.8.6. Oct. 12, 2019. Add API check_missing_models(), and get_all_xxxs() for getting all devices of the same type. Add new API check_powerflow_data() to replace check_database().
- 0.8.5. Oct.  9, 2019. Add API to retrieve model dynamic data pair, including generator, wt generator, pv unit, load, line, and HVDC.
- 0.8.4. Sep. 27, 2019. Add API to set/get generator voltage/mechanical power reference, and excitation voltage/mechanical power.
- 0.8.3. Sep. 25, 2019. Add API to get generator/wt_generator/pv_unit/loads with constraints of area and zone number.
- 0.8.2. Sep. 22, 2019. Update close/trip line/transformer breaker APIs, set prepare transformer meter APIs in stepspy.
- 0.8.1. Sep. 18, 2019. Update and README.
- 0.8.0. Sep. 18, 2019. Update to enable both parallel and serial library. Since STEPS 0.6.3.
- 0.7.1. Sep. 18, 2019. Fix API to set and get parallel thread number.
- 0.7.0. Sep. 18, 2019. Add new API to set parallel thread number. Update README.
- 0.6.1. Aug. 27, 2019
- 0.6.0. Aug. 25, 2019


## Install

### Install stepspy

To install stepspy, you can run the following codes on your computer or server:

```python
python -m pip install stepspy
```

If you want to manually install stepspy, follow the instructions:

1. Go to https://github.com/changgang/steps to download or fork the latest version of STEPS.
2. Go to python/ folder of STEPS, and copy the latest version of stepspy/ to PYTHONPATH/Lib/site-packages/.

### Install dynamic library

After install the stepspy, you still need to compile and install the dynamic library of STEPS.

1. Go to [steps](https://github.com/changgang/steps) to download or fork the latest version of STEPS. The latest version is usually the [work] branch.
2. Compile STEPS into dynamic library following instructions of STEPS.
3, Move the dynamic library of STEPS to stepspy/libsteps/ in the PYTHONPATH/Lib/site-packages/.
4, If the VC runtime is missing, download and install Microsoft Visual C++ 2017 Redistributable of 32 or 64 bit version.
5, If VC runtime or Mingw Runtime is missing, install vcredit or copy libwinpthread-1.dll from mingw compiler/bin/ to c:/windows/system32 and c:/windows/SysWOW64

Follow README of STEPS if you want to use both serial and parallel library.

## Usage

## Examples


## Maintainers

[@changgang](https://github.com/changgang) <lichgang@sdu.edu.cn> from the School of Electrical Engineering, Shandong University, China

## Contributing

Feel free to dive in! [Open an issue](https://github.com/changgang/steps/issues/new).

## License

[MIT](LICENSE) @ Changgang Li