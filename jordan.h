#pragma once
#include <algorithm>
#include <cmath>
#include "matrix.h"
#include <vector>
#include <semaphore.h>
void jordan_solver(int size,
                                  Matrix* matrix_ptr,
                                  std::vector<double>* b_ptr,
                                  std::vector<double> * answer,
                                  pthread_mutex_t* mutex,
                                  sem_t* semaphore,
                                  int pthread_index,
                                  int* max_i,
                                  int* max_j,
                                  int number_of_thread,
                                  pthread_barrier_t* barr)
{
    Matrix &matrix = *matrix_ptr;
    std::vector<double> &b = *b_ptr;
    std::vector<int> swap_memory(size);
    std::vector<double> x(size);
   
    /* lock
    if (== 0) {
        НАЙТИ МАКС ЭЛЕМЕНТ
        *maxElementI = maxi;
        *.. = ...;
    }
    unlock */
    
    for (int i = 0; i < size; i++)
    {
        int row_max, col_max;
        //std::cout <<pthread_index << "pthread_index" << " ";
        // std::cout << "PTHREAD_INDEX" << pthread_index << '\n';
        // std::cout << '1' << '\n';
        //std::cout << matrix;
        sem_wait(semaphore);
        
        std::cout << "PTHREAD_INDEX" << pthread_index << '\n';
        std::cout << '1' << '\n';
        std::cout << matrix;
        sem_post(semaphore);
        if (pthread_index == 0)
        {
            std::pair<int, int> max =  matrix.maxElem(matrix.getSize() - i);
            row_max = max.first;
            col_max = max.second;
            *max_i = row_max;
            *max_j = col_max;
            matrix.colSwap(i, col_max);
            swap_memory.push_back(i);
            swap_memory.push_back(col_max);
            //std::cout << matrix;
            //std::cout << "------------------------------" << std::endl;
            matrix.rowSwap(i, row_max, size - i);
            matrix.bSwap(i, row_max);
            double divisor = matrix(i, i);
            for (int j = 0; j < size; j++)
            {
                matrix(i, j) = matrix(i, j) / divisor;
                //matrix(k, j) -= matrix(i, j) / matrix(i, i) * matrix(k, i);
            }
            matrix._b[i] = matrix._b[i] / divisor;
        } 
        if (pthread_index != 0)
        {
            row_max = *max_i;
            col_max = *max_j;
            //continue;
            //return;
        }
        // sem_wait(semaphore);
        pthread_barrier_wait(barr);
        
        std::cout << "PTHREAD_INDEX" << pthread_index << '\n';
        std::cout << '2' << '\n';
        std::cout << matrix;
        sem_post(semaphore);
        // if (pthread_index != 0)
        // {
        //     row_max = *max_i;
        //     col_max = *max_j;
        //     //continue;
        //     //return;
        // }
        // sem_wait(semaphore);
        // sem_post(semaphore);
        //matrix(matrix.maxElem(matrix.getSize()).first, matrix.maxElem(matrix.getSize()).second);
        // if (pthread_index == 0)
        // {
        //     matrix.colSwap(i, col_max);
        //     swap_memory.push_back(i);
        //     swap_memory.push_back(col_max);
        //     //std::cout << matrix;
        //     //std::cout << "------------------------------" << std::endl;
        //     matrix.rowSwap(i, row_max, size - i);
        //     matrix.bSwap(i, row_max);
        //     double divisor = matrix(i, i);
        //     for (int j = 0; j < size; j++)
        //     {
        //         matrix(i, j) = matrix(i, j) / divisor;
        //         //matrix(k, j) -= matrix(i, j) / matrix(i, i) * matrix(k, i);
        //     }
        //     matrix._b[i] = matrix._b[i] / divisor;
        // }
        // sem_wait(semaphore);
        // sem_post(semaphore);
        // std::cout << "PTHREAD_INDEX" << pthread_index << '\n';
        // std::cout << '3' << '\n';
        //std::swap(exact_solution[i], exact_solution[row_max]);
        // for (int i = 0; i < size; i++)
        // {
        //     std::cout << exact_solution[i] << std::endl;
        // }

        // std::cout << matrix;
        // std::cout << "---------------------------------------------------------------" << std::endl;
        // std::cout << matrix;
        // std::cout << "---------------------------------------------------------------" << std::endl;
        double factor;
        //for (int k = i + 1; k < size; k++)
        for (int k = pthread_index; k < size; k += number_of_thread)
        {
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
            //std::cout << matrix;
        }
        // sem_wait(semaphore);
        // sem_post(semaphore);
        pthread_barrier_wait(barr);
        // std::cout << "PTHREAD_INDEX" << pthread_index << '\n';
        // std::cout << '4' << '\n';
        
        // std::cout << matrix;
        // std::cout << "------------------------------" << std::endl;
    }
    // 
    if (pthread_index == 0)
    {
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
        *answer = x;
    }
    //sem_wait(semaphore);
    // for (int i = 0; i < size; i++)
    // {
    //     std::cout << x[i] << std::endl;
    // }
    //return x;
}