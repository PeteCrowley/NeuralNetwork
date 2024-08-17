#ifndef CSVREADER_H
#define CSVREADER_H


#include <vector>
#include <string>

class CSVReader{
public:
    static std::vector<std::vector<float>> readfloatRows(std::string fileName, int startColumn, int endColumn, int startRow, int endRow);
    static std::vector<float> readfloatColumn(std::string fileName, int columnIndex, int startRow, int endRow);
    static std::vector<std::vector<float>> normalize(std::vector<std::vector<float>> data, float max);
    static std::vector<std::vector<float>> vectorizeOutputs(std::vector<float> outputs, int numClasses);
};

#endif // CSVREADER_H

