#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include "pybind11/stl.h"

#include "absl/status/status.h"

#include "ink-stroke-modeler/ink_stroke_modeler/types.h"
#include "ink-stroke-modeler/ink_stroke_modeler/params.h"
#include "ink-stroke-modeler/ink_stroke_modeler/stroke_modeler.h"

#include <vector>

namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(std::vector<ink::stroke_model::Result>);

/*
    We are only interested in exposing the interfaces that are going to be used by consumers:
    * types.h
    * params.h
    * stroke_modeler.h
*/
PYBIND11_MODULE(smooth_lines, handle) {
    handle.doc() = "smooth_lines is a module with bindings for the ink-stroke-modeler library";
    
    py::class_<ink::stroke_model::Vec2>(handle, "Vec2")
        .def(py::init<float, float>())
        .def(py::init<>())
        .def_readwrite("x", &ink::stroke_model::Vec2::x)
        .def_readwrite("y", &ink::stroke_model::Vec2::y)
        .def("Magnitude", &ink::stroke_model::Vec2::Magnitude);;
    
    py::class_<ink::stroke_model::Duration>(handle, "Duration")
        .def(py::init<double>())
        .def(py::init<>())
        .def_property_readonly("value", &ink::stroke_model::Duration::Value);
    
    py::class_<ink::stroke_model::Time>(handle, "Time")
        .def(py::init<double>())
        .def(py::init<>())
        .def_property_readonly("value", &ink::stroke_model::Time::Value);

    py::class_<ink::stroke_model::WobbleSmootherParams>(handle, "WobbleSmootherParams")
        .def(py::init<ink::stroke_model::Duration, float, float>())
        .def(py::init<>())
        .def_readwrite("timeout", &ink::stroke_model::WobbleSmootherParams::timeout)
        .def_readwrite("speed_floor", &ink::stroke_model::WobbleSmootherParams::speed_floor)
        .def_readwrite("speed_ceiling", &ink::stroke_model::WobbleSmootherParams::speed_ceiling);

    py::class_<ink::stroke_model::PositionModelerParams>(handle, "PositionModelerParams")
        .def(py::init<float, float>())
        .def(py::init<>())
        .def_readwrite("spring_mass_constant", &ink::stroke_model::PositionModelerParams::spring_mass_constant)
        .def_readwrite("drag_constant", &ink::stroke_model::PositionModelerParams::drag_constant);

    py::class_<ink::stroke_model::SamplingParams>(handle, "SamplingParams")
        .def(py::init<double, float, int, int>())
        .def(py::init<double, float, int>())
        .def(py::init<>())
        .def_readwrite("min_output_rate", &ink::stroke_model::SamplingParams::min_output_rate)
        .def_readwrite("end_of_stroke_stopping_distance", &ink::stroke_model::SamplingParams::end_of_stroke_stopping_distance)
        .def_readwrite("end_of_stroke_max_iterations", &ink::stroke_model::SamplingParams::end_of_stroke_max_iterations)
        .def_readwrite("max_outputs_per_call", &ink::stroke_model::SamplingParams::max_outputs_per_call);

    py::class_<ink::stroke_model::StylusStateModelerParams>(handle, "StylusStateModelerParams")
        .def(py::init<int>())
        .def(py::init<>())
        .def_readwrite("max_input_samples", &ink::stroke_model::StylusStateModelerParams::max_input_samples);

    py::class_<ink::stroke_model::StrokeEndPredictorParams>(handle, "StrokeEndPredictorParams")
        .def(py::init<>());

    py::class_<ink::stroke_model::DisabledPredictorParams>(handle, "DisabledPredictorParams")
        .def(py::init<>());

    py::class_<ink::stroke_model::ExperimentalParams>(handle, "ExperimentalParams")
        .def(py::init<>());

    py::class_<ink::stroke_model::KalmanPredictorParams::ConfidenceParams>(handle, "ConfidenceParams")
        .def(py::init<>())
        .def(py::init<int, float, float, float, float, float>())
        .def_readwrite("desired_number_of_samples", &ink::stroke_model::KalmanPredictorParams::ConfidenceParams::desired_number_of_samples)
        .def_readwrite("max_estimation_distance", &ink::stroke_model::KalmanPredictorParams::ConfidenceParams::max_estimation_distance)
        .def_readwrite("min_travel_speed", &ink::stroke_model::KalmanPredictorParams::ConfidenceParams::min_travel_speed)
        .def_readwrite("max_travel_speed", &ink::stroke_model::KalmanPredictorParams::ConfidenceParams::max_travel_speed)
        .def_readwrite("max_linear_deviation", &ink::stroke_model::KalmanPredictorParams::ConfidenceParams::max_linear_deviation)
        .def_readwrite("baseline_linearity_confidence", &ink::stroke_model::KalmanPredictorParams::ConfidenceParams::baseline_linearity_confidence);
    
    py::class_<ink::stroke_model::KalmanPredictorParams>(handle, "KalmanPredictorParams")
        .def(py::init<>())
        .def(py::init<double, double, int, int, float, float, float, ink::stroke_model::Duration, ink::stroke_model::KalmanPredictorParams::ConfidenceParams>())
        .def_readwrite("process_noise", &ink::stroke_model::KalmanPredictorParams::process_noise)
        .def_readwrite("measurement_noise", &ink::stroke_model::KalmanPredictorParams::measurement_noise)
        .def_readwrite("min_stable_iteration", &ink::stroke_model::KalmanPredictorParams::min_stable_iteration)
        .def_readwrite("max_time_samples", &ink::stroke_model::KalmanPredictorParams::max_time_samples)
        .def_readwrite("min_catchup_velocity", &ink::stroke_model::KalmanPredictorParams::min_catchup_velocity)
        .def_readwrite("acceleration_weight", &ink::stroke_model::KalmanPredictorParams::acceleration_weight)
        .def_readwrite("jerk_weight", &ink::stroke_model::KalmanPredictorParams::jerk_weight)
        .def_readwrite("prediction_interval", &ink::stroke_model::KalmanPredictorParams::prediction_interval)
        .def_readwrite("confidence_params", &ink::stroke_model::KalmanPredictorParams::confidence_params);

    py::class_<ink::stroke_model::PredictionParams>(handle, "PredictionParams")
        .def(py::init<ink::stroke_model::StrokeEndPredictorParams>())
        .def(py::init<ink::stroke_model::KalmanPredictorParams>())
        .def(py::init<ink::stroke_model::DisabledPredictorParams>());

    py::class_<ink::stroke_model::StrokeModelParams>(handle, "StrokeModelParams")
        .def(py::init<ink::stroke_model::WobbleSmootherParams, 
                    ink::stroke_model::PositionModelerParams, 
                    ink::stroke_model::SamplingParams, 
                    ink::stroke_model::StylusStateModelerParams,
                    ink::stroke_model::PredictionParams>());

    //stroke_modeler.h

    py::enum_<ink::stroke_model::Input::EventType>(handle, "EventType")
      .value("kDown", ink::stroke_model::Input::EventType::kDown)
      .value("kMove", ink::stroke_model::Input::EventType::kMove)
      .value("kUp", ink::stroke_model::Input::EventType::kUp);

    py::class_<ink::stroke_model::Input>(handle, "Input")
      .def(py::init<>())
      .def(py::init<
        ink::stroke_model::Input::EventType,
        ink::stroke_model::Vec2,
        ink::stroke_model::Time,
        float,
        float,
        float
        >())
      .def_readwrite("event_type", &ink::stroke_model::Input::event_type)
      .def_readwrite("position", &ink::stroke_model::Input::position)
      .def_readwrite("time", &ink::stroke_model::Input::time)
      .def_readwrite("pressure", &ink::stroke_model::Input::pressure)
      .def_readwrite("tilt", &ink::stroke_model::Input::tilt)
      .def_readwrite("orientation", &ink::stroke_model::Input::orientation);

    py::class_<ink::stroke_model::Result>(handle, "Result")
        .def(py::init<>())
        .def(py::init<
            ink::stroke_model::Vec2,
            ink::stroke_model::Vec2,
            ink::stroke_model::Time,
            float,
            float,
            float
            >())
        .def_readwrite("position", &ink::stroke_model::Result::position)
        .def_readwrite("velocity", &ink::stroke_model::Result::velocity)
        .def_readwrite("time", &ink::stroke_model::Result::time)
        .def_readwrite("pressure", &ink::stroke_model::Result::pressure)
        .def_readwrite("tilt", &ink::stroke_model::Result::tilt)
        .def_readwrite("orientation", &ink::stroke_model::Result::orientation);

    //READ THIS: https://stackoverflow.com/questions/70131486/problem-with-passing-stl-containers-using-pybind11
    //READ THIS: https://github.com/pybind/pybind11/blob/b3573ac9615b705d6e5b9bf598a3378edf079f06/include/pybind11/stl_bind.h
    //https://stackoverflow.com/questions/70012280/how-to-pass-a-vector-by-reference-in-pybind11-c
    //https://stackoverflow.com/questions/56811833/pybind11-accessing-on-opaque-vectors-of-opaque-vectors 
    //https://pybind11.readthedocs.io/en/stable/advanced/cast/stl.html#making-opaque-types
    /*
        bind_vector exposes append, clear, extend, insert, pop, and some iterator methods. 
        bind_map exposes keys, values, items, and some iterator methods.
    */
    py::bind_vector<std::vector<ink::stroke_model::Result>>(handle, "ResultVector");

    py::class_<ink::stroke_model::StrokeModeler>(handle, "StrokeModeler")
        .def(py::init<>())
        .def("Reset", [](ink::stroke_model::StrokeModeler &self) -> py::str {
            auto status = self.Reset();
            if(!status.ok()) {
                return py::str(status.message());
            }
            return py::str("OK");
        })
        .def("Reset", [](ink::stroke_model::StrokeModeler &self, ink::stroke_model::StrokeModelParams params) -> py::str {
            auto status = self.Reset(params);
            if(!status.ok()) {
                return py::str(status.message());
            }
            return py::str("OK");
        })
        .def("Update", [](ink::stroke_model::StrokeModeler &self, const ink::stroke_model::Input& input, std::vector<ink::stroke_model::Result>& result) -> py::str {
            auto status = self.Update(input, result);
            if(!status.ok()) {
                return py::str(status.message());
            }
            return py::str("OK");
        })
        .def("Save", &ink::stroke_model::StrokeModeler::Save)
        .def("Restore", &ink::stroke_model::StrokeModeler::Restore);
        
}
