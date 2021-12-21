#include <iostream>
#include <vector>
#include <iostream>
#include "matrix.h"
#include "function.h"
#include "jordan.h"
#include <cmath>
#include <chrono>
#include <pthread.h>
#include <semaphore.h>

struct pthread_argument {
    int size;
    Matrix* matrix;
    std::vector<double>* b;
    std::vector<double>* x;
    int* maxElementI;
    int* maxElementJ;
    int pthread_index;
    pthread_mutex_t* mutex;
    sem_t* semaphore;
    int number_of_thread;
    pthread_barrier_t* barr;
};



void* pthread_func(void* pointer) {
    pthread_argument* pa = (pthread_argument*) pointer;
    //pthread_mutex_lock(pa->mutex);
    //pthread_mutex_lock((*pa).mutex); // аналогично
    // ЗАЛОЧЬ ТУТ МЬЮТЕКСч
        // НАДО ПЕРЕПИСАТЬ jordan_solver ТАК ЧТОБ ОНА ОТВЕТ НЕ ВОЗВРАЩАЛА, А ЗАПИСЫВАЛА
        // В ПЕРЕМЕННУЮ КОТОРУЮ ПО УКАЗАТЕЛЮ ПОДАЛИ В КОНЕЦ. ТО ЕСТЬ ДОБАВИТЬ АРГУМЕНТ ФУНКЦИИ
    jordan_solver(pa->size, pa->matrix, pa->b, pa->x, pa->mutex, pa -> semaphore, pa -> pthread_index, pa -> maxElementI, pa -> maxElementJ, pa -> number_of_thread, pa -> barr);
    
    //pthread_mutex_unlock(pa->mutex);
    // разлочь ТУТ МЬЮТЕКС
    return nullptr;
}

int main(int argc, char **argv)
{
    Matrix matrix;
    if (atoi(argv[3]) == 0)
    {
        freopen("input.txt", "r", stdin); // перенаправить стандартный поток вывода в файл
        matrix = Matrix(atoi(argv[1]));
        std::cin >> matrix;
        fclose(stdin);
    }
    if (atoi(argv[3]) != 0)
    {
        matrix = Matrix(atoi(argv[1]));
        int n = matrix.getSize();
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                matrix(i, j) = func(atoi(argv[3]), n, i, j);
            }
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j += 2)
            {
                matrix._b[i] += matrix(i, j);
            }
        }
    }
    print_matrix(matrix, atoi(argv[1]), atoi(argv[2]));
    std::vector<double> x;
    x.resize(atoi(argv[1]));
    // int number_of_pthread -- получить с клавиатуры
    int number_of_pthread = 4;
    //freopen("CON","w",stdout);
    // std::cout << matrix;
    // std::cout << "----------------------------------------------------------------" << std::endl;
    // std::cout << "-----------------------" << endl;
    // matrix.colSwap(1, 2);
    // std::cout << "After columns 1 and 2 swap: " << endl;
    // std::cout << matrix;
    // std::cout << "-----------------------" << endl;
    // std::cout << "After rows 1 and 2 of right_bottom_submatrix swap: " << endl;
    // matrix.rowSwap(1, 2, 2);
    // std::cout << matrix;
    // std::cout << "-----------------------" << endl;
    // for (int i = 0; i < atoi(argv[1]); i++)
    // {
    //     std::cout << "Submatrix max: " << matrix.maxElem(matrix.getSize() - i).first << " " << matrix.maxElem(matrix.getSize() - i).second << endl;
    // }
    //std::cout << "Submatrix max: " << matrix.maxElem(matrix.getSize()).first << " " << matrix.maxElem(matrix.getSize()).second << endl;
    //std::cout << matrix.getSize();
    for (int i = 0; i < argc; i++)
    {
        std::cout << "Command line parameter:" << argv[i] << std::endl;
    }
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    //std::vector<double> solution = jordan_solver(atoi(argv[1]), matrix, matrix._b, x);


    pthread_t Pthreads[number_of_pthread];
    pthread_argument arguments[number_of_pthread];
    pthread_mutex_t Mutex;
    sem_t semaphore;
    pthread_barrier_t barr;
    sem_init(&semaphore, 0, 1);
    pthread_mutex_init(&Mutex, NULL);
    pthread_barrier_init(&barr, NULL, number_of_pthread);
    int maxElementI, maxElementJ;

    for (int i = 0; i < number_of_pthread; ++i) {
        arguments[i] = {atoi(argv[1]), &matrix, &matrix._b, &x, &maxElementI, &maxElementJ, i, &Mutex, &semaphore, number_of_pthread, &barr};
        pthread_create(&(Pthreads[i]), NULL, &pthread_func, &(arguments[i]));
    }

    //std::vector<double> solution = jordan_solver(atoi(argv[1]), matrix, matrix._b, x); НЕ НУЖНАЯ СТРОКА

    for (int i = 0; i < number_of_pthread; ++i) {
        pthread_join(Pthreads[i], NULL);
    }
    pthread_mutex_destroy(&Mutex);
    sem_destroy(&semaphore);


    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
     std::vector<double> exact_solution;
     exact_solution.resize(atoi(argv[1]));
     for (int i = 0; i < atoi(argv[1]); i += 2)
     {
         exact_solution[i] = 1;
     }
     std::vector<double> residual(atoi(argv[1]));
     for (int i = 0; i < atoi(argv[1]); i++)
     {
         residual[i] = matrix._b[i];
     }
     for (int i = 0; i < atoi(argv[1]); i++)
     {
         for (int j = 0; j < atoi(argv[1]); j++)
         {
             residual[i] -= matrix(i, j) * x[j];
         }
     }
     double b_norm = 0;
     double residual_norm = 0;
     for (double X : matrix._b)
     {
         b_norm += X * X;
     }
     b_norm = sqrt(b_norm);
     for (double y : residual)
     {
         residual_norm += y * y;
     }
     residual_norm = sqrt(residual_norm);
     for(int i = 0; i < atoi(argv[2]); i++)
     {
         std::cout << std::scientific << x[i] << " "; 
     }
     std::cout << std::endl;
     std::cout << "Норма невязки = " << std::scientific << residual_norm / b_norm << std::endl;
     std::vector<double> error(atoi(argv[1]));
     for (int i = 0; i < atoi(argv[1]); i++)
     {
         error[i] = x[i] - exact_solution[i];
     }
     double error_norm = 0;
     for (double z : error)
     {
         error_norm += z * z;
     }
     error_norm = sqrt(error_norm);
     std::cout << "Норма погрешности = " << std::scientific << error_norm << std::endl;
     std::cout << "Время решения системы = " << elapsed_seconds.count() << std::endl;
     //std::cout << matrix;
     //print_matrix(matrix, atoi(argv[1]), atoi(argv[1]));
    return 0;
}
