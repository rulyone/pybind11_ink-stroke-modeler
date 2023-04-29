# Pybind11 bindings for the Ink Stroke Modeler Google Library

From the original C++ project:

*This library smooths raw freehand input and predicts the input's motion to minimize display latency. It turns noisy pointer input from touch/stylus/etc. into the beautiful stroke patterns of brushes/markers/pens/etc.

Be advised that this library was designed to model handwriting, and as such, prioritizes smooth, good-looking curves over precise recreation of the input.*


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
git submodule update --init #to download git submodules dependencies (pybind11 and ink-stroke-modeler)
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

## Visual Example

There's a `test_binding.py` file that showcase how to use this binding in Python.

This visual example requires `pyside6` dependency. But you can use any other library to  show how the lines are improved by this library.

After you have built the module as shown in a previous step, we can execute this visual example using the following commands:

```bash
#we assume this folder was used to build the module
#if not, go to the Build the bindings section of this readme file
cd build 
#we need to have the .so file in the folder where python will be executed
cp ../test_binding.py . 
#also we need the points.txt file
cp ../points.txt .
#make sure we have the pyside dependency
pip3 install pyside6
#run it
python3 test_binding.py
```

After this, you should see the following widget in your screen:

![Example Wide](example2.png?raw=true "Example Wide")

To the left we see the line painted using the raw points.txt, without processing.
To the right we have the smoothed line by the StrokeModeler class.

The change here is subtle, but try drawing different lines with different widths, and add the pressure data received from the pen and you will notice the difference.

You can change in the `test_binding.py` the line that contains the `pen.setWidth(10)` to draw lines thiner or wider.

![Example Thin](example1.png?raw=true "Example Thin")

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


