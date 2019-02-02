
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <migraphx/program.hpp>
#include <migraphx/onnx.hpp>

namespace py = pybind11;

template<class F>
struct skip_half
{
    F f;

    template<class A>
    void operator()(A a) const
    {
        f(a);
    }

    void operator()(migraphx::shape::as<migraphx::half>) const
    {
        throw std::runtime_error("Half not supported in python yet.");
    }  
};

template<class F>
void visit_type(const migraphx::shape& s, F f)
{
    s.visit_type(skip_half<F>{f});
}

template<class T>
py::buffer_info to_buffer_info(T& x)
{
    migraphx::shape s = x.get_shape();
    py::buffer_info b;
    visit_type(s, [&](auto as) {
            b = py::buffer_info(
            x.data(),
            as.size(),
            py::format_descriptor<decltype(as())>::format(),
            s.lens().size(),
            s.lens(),
            s.strides()
        );
    });
    return b;
}

PYBIND11_MODULE(migraphx, m) {
    py::class_<migraphx::shape>(m, "shape")
        .def(py::init<>())
        .def("type", &migraphx::shape::type)
        .def("lens", &migraphx::shape::lens)
        .def("strides", &migraphx::shape::strides)
        .def("elements", &migraphx::shape::elements)
        .def("bytes", &migraphx::shape::bytes)
        .def("type_size", &migraphx::shape::type_size)
        .def("packed", &migraphx::shape::packed)
        .def("transposed", &migraphx::shape::transposed)
        .def("broadcasted", &migraphx::shape::broadcasted)
        .def("standard", &migraphx::shape::standard)
        .def("scalar", &migraphx::shape::scalar);

    py::class_<migraphx::argument>(m, "argument", py::buffer_protocol())
       .def_buffer([](migraphx::argument &x) -> py::buffer_info {
            return to_buffer_info(x);
        });

    py::class_<migraphx::program>(m, "program")
        .def("get_parameter_shapes", &migraphx::program::get_parameter_shapes)
        .def("compile", [](migraphx::program& p, const migraphx::target& t) {
            p.compile(t);
        })
        .def("eval", &migraphx::program::eval);

    m.def("parse_onnx", &migraphx::parse_onnx);

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}

