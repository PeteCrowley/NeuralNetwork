#include <iostream>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include "CSVReader.h"

using namespace std;

/**
 * Reads a CSV file and returns a vector of vectors of floats
 * @param fileName: The name of the file to read
 * @param startColumn: The column to start reading from
 * @param endColumn: The column to stop reading at
 * @return: A vector of vectors of floats
*/
vector<vector<float>> CSVReader::readfloatRows(string fileName, int startColumn, int endColumn, int startRow, int endRow){
    ifstream file(fileName);
    vector<vector<float>> rows;
    string line;
    int rowNumber = 0;
    while(getline(file, line) && rowNumber <= endRow){
        if(rowNumber < startRow){
            rowNumber++;
            continue;
        }
        vector<float> row;
        stringstream ss(line);
        string cell;
        int column = 0;
        while(getline(ss, cell, ',')){
            if(column >= startColumn){
                row.push_back(stod(cell));
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
vector<float> CSVReader::readfloatColumn(string fileName, int columnIndex, int startRow, int endRow){
    ifstream file(fileName);
    vector<float> column;
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
                column.push_back(stof(cell));
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
vector<vector<float>> CSVReader::normalize(vector<vector<float>> data, float max){
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
vector<vector<float>> CSVReader::vectorizeOutputs(vector<float> outputs, int numClasses){
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