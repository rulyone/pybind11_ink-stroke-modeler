import smooth_lines
import math
import time
import sys

from PySide6.QtWidgets import QApplication, QWidget
from PySide6.QtGui import QPainter, QPainterPath
      
# We start creating the StrokeModelerParams 
# (using the same values as in the example provided by the C++ library)
timeout = smooth_lines.Duration(.04)
speed_floor = 1.31
speed_ceiling = 1.44
wobble_smoother_params = smooth_lines.WobbleSmootherParams(timeout, speed_floor, speed_ceiling)

spring_mass_constant = 11.0 / 32400
drag_constant = 72.0
position_modeler_params = smooth_lines.PositionModelerParams(spring_mass_constant, drag_constant)

min_output_rate = 180
end_of_stroke_stopping_distance = 0.001
end_of_stroke_max_iterations = 20
sampling_params = smooth_lines.SamplingParams(min_output_rate, end_of_stroke_stopping_distance, end_of_stroke_max_iterations)

max_input_samples = 20
stylus_state_modeler_params = smooth_lines.StylusStateModelerParams(max_input_samples)

prediction_params = smooth_lines.StrokeEndPredictorParams()

stroke_model_params = smooth_lines.StrokeModelParams(
    wobble_smoother_params,
    position_modeler_params,
    sampling_params,
    stylus_state_modeler_params,
    prediction_params
)

# Now we have the params to create our StrokeModeler
print("Setting up StrokeModeler")
modeler = smooth_lines.StrokeModeler()

print("CALLING Reset without params (it should print an error)")
result = modeler.Reset()
print(f"Reset: {result}")

print("CALLING Reset with params (it should print OK)")
result = modeler.Reset(stroke_model_params)
print(f"Reset: {result}")

print("CALLING Save without params (no output from this)")
modeler.Save()
print("CALLING Restore without params (no output from this)")
modeler.Restore()

# Now we will use the points.txt file to feed our StrokeModeler with data
# The points.txt file has x, y, t values, where:
# x = x axis of the point
# y = y axis of the point
# t = time in seconds (we need to multiply this value by 1000 because the library expects miliseconds)
print("PROCESING INPUTS")
# First our kDown, kMove and kUp events. 
# These represent mouse press, mouse move (while pressing), and mouse release 
# to represent the start, movement and end of a single stroke.
kDown = smooth_lines.EventType.kDown
kMove = smooth_lines.EventType.kMove
kUp = smooth_lines.EventType.kUp

# We won't use pressure, tilt nor orientation for this example.
# But this data can be obtained if using specific tablet models.
pressure = 0
tilt = 0
orientation = math.pi

# We create our ResultVector. 
# This vector gets updated after using the modeler.Update functions
smoothed_stroke = smooth_lines.ResultVector()

print("SMOOTHED STROKE LEN SO FAR...")
print(len(smoothed_stroke))
print("CALLING Update with inputs from points.txt file")

# We read points.txt and start feeding the StrokeModeler
raw_data = []
with open("points.txt", "r") as f:
    first = True
    for line in f:
        row = line.split()
        x = float(row[0])
        y = float(row[1])
        t = float(row[2]) * 1000 # important to use miliseconds
        raw_data.append({"x": x, "y": y, "t": t})
        if first:
            first = False
            event = kDown # first input we will assume is the mouse press event
        else:
            event = kMove # rest of inputs we will assume are mouse move events

        point = smooth_lines.Vec2(float(row[0]), float(row[1]))
        time = smooth_lines.Time(t)

        input = smooth_lines.Input(event, point, time, pressure, tilt, orientation)
        status = modeler.Update(input, smoothed_stroke)
        if status != "OK":
            print("Something is wrong here. Shouldn't happen.")
            print(status)

#TODO: since we haven't added a last kUp event, we can use the Predict function
# We repeat our last input as a kUp event, representing the end of stroke or mouse release event.
raw_data.append({"x": raw_data[-1]["x"], "y": raw_data[-1]["y"], "t": raw_data[-1]["t"]})
endpoint = smooth_lines.Vec2(raw_data[-1]["x"], raw_data[-1]["y"])
endtime = smooth_lines.Time(raw_data[-1]["t"])
endinput = smooth_lines.Input(kUp, endpoint, endtime, pressure, tilt, orientation)
modeler.Update(endinput, smoothed_stroke)

print("SMOOTHED STROKE LEN SO FAR...")
print(len(smoothed_stroke))

# Class to draw on screen using Qt the original raw points.txt as lines, 
# and also the new points processed by the StrokeModeler and saved 
# into the ResultVector (smoothed_stroke)
# We can see that smoothed_stroke has several more points. This is because
# the library interpolates data if certain thresholds are reached.
print(f"raw size: {len(raw_data)}, smoothed size: {len(smoothed_stroke)}")
class DrawingPaths(QWidget):

    def paintEvent(self, event):
        
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)
        pen = painter.pen()
        pen.setColor("red")
        pen.setWidth(10) # we can change the line width here.
        painter.setPen(pen)
        path = QPainterPath()

        #first we draw the raw points as lines
        path.moveTo(raw_data[0]["x"], raw_data[0]["y"])
        for i in range(1, len(raw_data)):
            path.lineTo(raw_data[i]["x"], raw_data[i]["y"])
        painter.drawPath(path)
        path.clear()

        # second we draw the smoothed stroke points as lines, 150 pixels to the right
        first_point = True
        for iterator in smoothed_stroke:
            if first_point:
                first_point = False
                path.moveTo(150 + iterator.position.x, iterator.position.y)
            else:
                path.lineTo(150 + iterator.position.x, iterator.position.y)
        painter.drawPath(path)
        painter.end()
            

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = DrawingPaths()
    ex.show()
    sys.exit(app.exec())

print("CLEARING OUT ResultVector")
smoothed_stroke.clear()
print("SMOOTHED STROKE LEN AFTER CLEAR...")
print(len(smoothed_stroke))
