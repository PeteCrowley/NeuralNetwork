#include <iostream>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include "CSVReader.h"
#include <string>
#include <vector>

using namespace std;
/**
 * Reads a CSV file and returns a vector of vectors of floats
 * @param fileName: The name of the file to read
 * @param startColumn: The column to start reading from
 * @param endColumn: The column to stop reading at
 * @return: A vector of vectors of floats
*/
vector<vector<int>> readIntRows(string fileName, int startColumn, int endColumn, int startRow, int endRow){
    ifstream file(fileName);
    vector<vector<int>> rows;
    string line;
    int rowNumber = 0;
    while(getline(file, line) && rowNumber <= endRow){
        if(rowNumber < startRow){
            rowNumber++;
            continue;
        }
        vector<int> row;
        stringstream ss(line);
        string cell;
        int column = 0;
        while(getline(ss, cell, ',')){
            if(column >= startColumn){
                row.push_back(stoi(cell));
                if(column == endColumn){
                    break;
                }
            }
            column++;
        }
        rows.push_back(row);
        rowNumber++;
    }
    return rows;
}

/**
 * Reads a CSV file and returns a vector of ints
 * @param fileName: The name of the file to read
 * @param columnIndex: The column to read from
 * @return: A vector of ints
*/
vector<int> readIntColumn(string fileName, int columnIndex, int startRow, int endRow){
    ifstream file(fileName);
    vector<int> column;
    string line;
    int rowNumber = 0;
    while(getline(file, line) && rowNumber <= endRow){
        if(rowNumber < startRow){
            rowNumber++;
            continue;
        }
        stringstream ss(line);
        string cell;
        int columnNumber = 0;
        while(getline(ss, cell, ',')){
            if(columnNumber == columnIndex){
                column.push_back(stoi(cell));
                break;
            }
            columnNumber++;
        }
        rowNumber++;
    }
    return column;
}

/**
 * Normalizes a vector of vectors of floats
 * @param data: The data to normalize
 * @param max: The maximum value to normalize to
 * @return: The normalized data
*/
vector<vector<float>> normalize(vector<vector<int>> data, int max){
    vector<vector<float>> normalizedData;
    for(int i = 0; i < data.size(); i++){
        vector<float> row;
        for(int j = 0; j < data[i].size(); j++){
            row.push_back(data[i][j] / max);
        }
        normalizedData.push_back(row);
    }
    return normalizedData;
}

/**
 * Vectorizes a vector of ints
 * @param outputs: The outputs to vectorize
 * @param numClasses: The number of classes
 * @return: The vectorized outputs
*/
vector<vector<float>> vectorizeOutputs(vector<int> outputs, int numClasses){
    vector<vector<float>> vectorizedOutputs;
    for(int i = 0; i < outputs.size(); i++){
        vector<float> vectorizedOutput;
        for(int j = 0; j < numClasses; j++){
            if(j == outputs[i]){
                vectorizedOutput.push_back(1);
            }else{
                vectorizedOutput.push_back(0);
            }
        }
        vectorizedOutputs.push_back(vectorizedOutput);
    }
    return vectorizedOutputs;
}