#include "rix/util/log.hpp"
#include "rix/core/common.hpp"
#include "rix/core/node.hpp"
#include "rix/core/service_client.hpp"

#include "msg/GradientDescentRequest.hpp"
#include "msg/GradientDescentResponse.hpp"

using namespace rix::core;
using namespace rix::util;

using GradientDescentRequest = rix::msg::lab8::GradientDescentRequest;
using GradientDescentResponse = rix::msg::lab8::GradientDescentResponse;

int main() {
    // TODO: Initialize the node with name "srv_client", IP address 127.0.0.1", and port RIX_HUB_PORT
    Node node("srv_client", "127.0.0.1", RIX_HUB_PORT);
    // TODO: Create a service client for the service "gradient_descent"
    ServiceClient<GradientDescentRequest, GradientDescentResponse> client(node, "gradient_descent");
    // TODO: Create a GradientDescentRequest message to request gradient descent to be performed. Try a request with:
    //      1. Initial guess 8.
    //      2. Step scale 1e-2.
    //      3. Tolerance 1e-6.
    //      4. Iteration limit 10000.
    GradientDescentRequest request;
    request.initial_guess = 8.0;
    request.step_scale = 1e-2;
    request.tolerance = (1e-6);
    request.max_iterations = (10000);

    // TODO: Create a GradientDescentResponse message to store the response
    GradientDescentResponse response;
    
    // TODO: Call the service with the request and response messages
    client.call(request, response);

    // TODO: Output the value and convergence in the format "Value: {value}, Converged: {converged}"
    String value = response.value;
    if(response.converged()) {
        value = "true";
    } else {
        value = "false";
    }
    std::cout << "Value: " << response.value << ", Converged: " << value << std::endl;
    return 0;
}