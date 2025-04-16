#include <iostream>
#include <queue>
#include <vector>


// Given a stream of integers and a window size, calculate the moving average
// of all integers in the sliding window.

// Implement the MovingAverage class:
//   * MovingAverage(int size) Initializes the object with the size of the
//     window size.
//   * double next(int val) Returns the moving average of the last size values
//     of the stream.

// Example:
// 1. MovingAverage movingAverage = new MovingAverage(3);
//    movingAverage.next(1); // return 1.0 = 1 / 1
//    movingAverage.next(10); // return 5.5 = (1 + 10) / 2
//    movingAverage.next(3); // return 4.66667 = (1 + 10 + 3) / 3
//    movingAverage.next(5); // return 6.0 = (10 + 3 + 5) / 3

class MovingAverage {
   public:
    MovingAverage(int window_size) {
        // TODO: Implement the constructor
        this->window_size = window_size;
        sum  =0; 
    }

    double next(int val) {
        // TODO: Implement the next function
        if (window.size() == window_size) {
            sum = sum - window.front();
            // this will remove it 
            window.pop();
        }
        window.push(val);
         sum += val;
        double num = static_cast<double>(sum) / window.size();
        return num;
    }

   private:
    // TODO: Define any necessary variables
    int window_size;
    double sum;
    std::queue<int> window;
};

int main() {
    MovingAverage ma(3);
    std::cout << ma.next(1) << std::endl;
    std::cout << ma.next(10) << std::endl;
    std::cout << ma.next(3) << std::endl;
    std::cout << ma.next(5) << std::endl;

    MovingAverage ma2(5);
    std::cout << ma2.next(1) << std::endl;
    std::cout << ma2.next(10) << std::endl;
    std::cout << ma2.next(3) << std::endl;
    std::cout << ma2.next(5) << std::endl;
    std::cout << ma2.next(7) << std::endl;
    std::cout << ma2.next(8) << std::endl;

    return 0;
}