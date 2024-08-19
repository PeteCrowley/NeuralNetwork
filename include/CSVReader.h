#ifndef CSVREADER_H
#define CSVREADER_H


#include <vector>
#include <string>

std::vector<std::vector<int>> readIntRows(std::string fileName, int startColumn, int endColumn, int startRow, int endRow);
std::vector<int> readIntColumn(std::string fileName, int columnIndex, int startRow, int endRow);
std::vector<std::vector<float>> normalize(std::vector<std::vector<int>> data, int max);
std::vector<std::vector<float>> vectorizeOutputs(std::vector<int> outputs, int numClasses);


#endif // CSVREADER_H

