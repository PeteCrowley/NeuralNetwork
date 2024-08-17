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
    float (*cost)(float output, float expectedOutput);

    /**
     * Returns the derivative of the cost function for a certain output and expected output
     * @param output: The output of the network
     * @param expectedOutput: The expected output of the network
    */
    float (*derivative)(float output, float expectedOutput);
    CostFunction();
    CostFunction(float (*cost)(float output, float expectedOutput), float (*derivative)(float output, float expectedOutput));
};

extern CostFunction errorSquared;

#endif // COST_FUNCTION_H