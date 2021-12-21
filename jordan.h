#pragma once
#include <algorithm>
#include <cmath>
#include "matrix.h"
#include <vector>
void jordan_solver(int size,
                                  Matrix matrix,
                                  std::vector<double> b,
                                  std::vector<double> * answer,
                                  pthread_mutex_t* mutex)
{
    std::vector<int> swap_memory(size);
    std::vector<double> x(size);
    for (int i = 0; i < size; i++)
    {
        int row_max = matrix.maxElem(matrix.getSize() - i).first;
        int col_max = matrix.maxElem(matrix.getSize() - i).second;
        //matrix(matrix.maxElem(matrix.getSize()).first, matrix.maxElem(matrix.getSize()).second);
        matrix.colSwap(i, col_max);
        swap_memory.push_back(i);
        swap_memory.push_back(col_max);
        //std::cout << matrix;
        //std::cout << "------------------------------" << std::endl;
        matrix.rowSwap(i, row_max, size - i);
        matrix.bSwap(i, row_max);
        //std::swap(exact_solution[i], exact_solution[row_max]);
        // for (int i = 0; i < size; i++)
        // {
        //     std::cout << exact_solution[i] << std::endl;
        // }

        // std::cout << matrix;
        // std::cout << "---------------------------------------------------------------" << std::endl;
        double divisor = matrix(i, i);
        for (int j = 0; j < size; j++)
        {
            matrix(i, j) = matrix(i, j) / divisor;
            //matrix(k, j) -= matrix(i, j) / matrix(i, i) * matrix(k, i);
        }
        matrix._b[i] = matrix._b[i] / divisor;
        // std::cout << matrix;
        // std::cout << "---------------------------------------------------------------" << std::endl;
        double factor;
        //for (int k = i + 1; k < size; k++)
        for (int k = 0; k < size; k++)
            if (k != i)
            {
                {
                    factor = matrix(k, i);
                    for (int j = 0; j < size; j++)
                    {
                        matrix(k, j) -= matrix(i, j) * factor;
                        //matrix(k, j) -= matrix(i, j) / matrix(i, i) * matrix(k, i);
                    }
                    matrix._b[k] -= matrix._b[i] * factor;
                    // std::cout << matrix;
                    // std::cout << "---------------------------------------------------------------" << std::endl;
                }
            }
        // std::cout << matrix;
        // std::cout << "------------------------------" << std::endl;
    }
    // 
    for (int i = 0; i < size; i++)
    {
        x[i] = matrix._b[i];
    }
    // for (int i = 0; i < size; i++)
    // {
    //     std::cout << x[i] << std::endl;
    // }
    for (int i = swap_memory.size() - 1; i > 0; i -= 2)
    {
        std::swap(x[swap_memory[i]], x[swap_memory[i - 1]]);
    }
    // for (int i = 0; i < size; i++)
    // {
    //     std::cout << x[i] << std::endl;
    // }
    *answer = x;
    //return x;
}