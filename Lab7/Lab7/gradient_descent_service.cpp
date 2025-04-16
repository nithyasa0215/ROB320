#include "rix/util/log.hpp"
#include "rix/core/common.hpp"
#include "rix/core/node.hpp"
#include "rix/core/service.hpp"

#include "msg/GradientDescentRequest.hpp"
#include "msg/GradientDescentResponse.hpp"

#include <autodiff/forward/dual.hpp>

using namespace rix::core;
using namespace rix::util;

using namespace autodiff;

using GradientDescentRequest = rix::msg::lab8::GradientDescentRequest;
using GradientDescentResponse = rix::msg::lab8::GradientDescentResponse;

dual objective_function(const dual& x) {
    // TODO: Implement a simple quadratic function: x^2 - 6x + 3.
    return (x * x ) - (6 * x) + 3;
}

std::pair<double, bool> gradient_descent_on_objective_function(double initial_guess, double step_scale, 
                                                               double tolerance, int max_iterations) {
    // TODO: Implement a gradient descent algorithm which:
    //      1. Starts at the value initial_guess 
    //      2. Takes steps of gradient * step_scale
    //      3. Terminates if the step is less then tolerance
    //      4. Takes at most max_iterations steps
    //      5. Returns {value, true} if the algorithm converges and {value, false} 
    //         if the algorithm reaches its iteration limit.

    // this is step 1
    dual x = initial_guess;

    for (int i = 0; i < max_iterations; i++) {
        // this is step 2
        // get the gradient value
        double x_val = val(x);
        dual gradient = derivative(objective_function, wrt(x), at(x_val));
        // does the calc
        double step = step_scale * val(gradient);
        // step 3- terminates if below a threshold
        if (std::abs(step) < tolerance) {
            return {val(x), true};
        }
        // does the decrement
        x = x - step;
    }
    return {val(x), false};
}


void gradient_descent_callback(const GradientDescentRequest &request, GradientDescentResponse &response) {
    // TODO: Run gradient descent with the data in the request and store the result in response.

    auto [value, converged] = gradient_descent_on_objective_function(request.initial_guess, request.step_scale, request.tolerance, request.max_iterations);
    response.value = value;
    response.converged = converged;
}

int main() {
    // TODO: Initialize the node with name "gradient_descent", IP address "127.0.0.1", and port RIX_HUB_PORT

    Node node("gradient_descent", "127.0.0.1", RIX_HUB_PORT);
    // TODO: Advertise the service "gradient_descent" with the service_callback function

    Service service(node, "gradient_descent", gradient_descent_callback);
    // TOOD: Check if the service was advertised successfully
    if(!service.advertised()) {
        LOG_ERROR("Failed to advertise service gradient_descent");
        return 1;
    }

    // TODO: Spin the node in the foreground
    node.spin();
    
    return 0;
}