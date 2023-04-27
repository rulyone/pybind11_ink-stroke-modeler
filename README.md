# Pybind11 bindings for the Ink Stroke Modeler Google Library

*Why?*

C++ libraries are fast. Really fast. If they're well written with a good interface (such as this case), using python bindings is best to maintain C++ performance while enabling its usage in Python code through pybind11 instead of rewriting it. 

Also, I'm expanding my expertise in advanced Python concepts, including the integration of C++ interfaces through bindings. This enables me to utilize high-performance libraries in Artificial Intelligence projects, where Python is widely adopted.

## Requirements

* Python 3.10 (other versions could work, but not tested)
* CMake 3.19 or higher (tested with 3.25)

## Build the bindings

```bash
git clone https://github.com/rulyone/pybind11_ink-stroke-modeler
cd pybind11_ink-stroke-modeler
mkdir build
cd build
cmake ..
make
```

If everything goes _smooth_, you should see a 100% of built targets without errors, and also a new file named `smooth_lines.cpython-311-darwin.so` or similar (ending with .so)

## Usage

After you have built this, you can simply import the new module in your python code:

```python
import smooth_lines

duration = smooth_lines.Duration(1)
print(f"\nDuration set with value {duration.value}")

time = smooth_lines.Time()
print(f"\nTime set with DEFAULT value {time.value}")

wobble_params = smooth_lines.WobbleSmootherParams(duration, 2.5, 3.5)
print(f"""
Wobble Params set with 
timeout: {wobble_params.timeout.value} , 
speed_floor:\t {wobble_params.speed_floor}, 
speed_ceiling:\t {wobble_params.speed_ceiling}
""")
```

Refer to the `test_binding.py` file for a complete example using this library.

## Common problems
`ModuleNotFoundError: No module named 'smooth_lines'` when trying to `import` the new module in python code.

This usually happens when you have multiple python versions installed and use 1 for building, and another one to run your python scripts.

Make sure you're using the same version both times. You can specify the python executable `path` when running cmake likes this:

```bash
mkdir build
cd build
cmake -DPYTHON_EXECUTABLE=/some/path/to/your/python3 ..
```

To know what versions of python you have installed, in Linux/Mac you can see it with `where python3` shell command.


