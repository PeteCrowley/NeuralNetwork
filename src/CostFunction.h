#ifndef COST_FUNCTION_H
#define COST_FUNCTION_H

#include <vector>

using namespace std;

/**
 * Class for a cost function
*/
class CostFunction {
public:
    /**
     * Returns the cost of a certain output and expected output
     * @param output: The output of the network
     * @param expectedOutput: The expected output of the network
    */
    double (*cost)(double output, double expectedOutput);

    /**
     * Returns the derivative of the cost function for a certain output and expected output
     * @param output: The output of the network
     * @param expectedOutput: The expected output of the network
    */
    double (*derivative)(double output, double expectedOutput);
    CostFunction();
    CostFunction(double (*cost)(double output, double expectedOutput), double (*derivative)(double output, double expectedOutput));
};

extern CostFunction errorSquared;

#endif // COST_FUNCTION_H