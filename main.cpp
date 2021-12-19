#include <iostream>
#include <vector>
#include <iostream>
#include "matrix.h"
#include "function.h"
#include "jordan.h"
#include <cmath>
#include <chrono>
#include <pthread.h>

struct pthread_argument {
    int size;
    Matrix* matrix;
    std::vector<double>* b;
    std::vector<double>* x;
    int* maxElementI;
    int* maxElementJ;
    int pthread_index;
    pthread_mutex_t* mutex;
};

void *pthread_func(void* pointer) {
    pthread_mutex_t mutex;
    pthread_argument* pa = (pthread_argument*) pointer;
    pthread_mutex_lock(pa->mutex);
    // ЗАЛОЧЬ ТУТ МЬЮТЕКСчё
   // pthread_mutex_init(&mutex, NULL);
    if (pa->pthread_index == 0) {
        // НАДО ПЕРЕПИСАТЬ jordan_solver ТАК ЧТОБ ОНА ОТВЕТ НЕ ВОЗВРАЩАЛА, А ЗАПИСЫВАЛА
        // В ПЕРЕМЕННУЮ КОТОРУЮ ПО УКАЗАТЕЛЮ ПОДАЛИ В КОНЕЦ. ТО ЕСТЬ ДОБАВИТЬ АРГУМЕНТ ФУНКЦИИ
        jordan_solver(pa->size, *(pa->matrix), *(pa->b), pa->x, pa -> mutex);
    }
    pthread_mutex_unlock(pa -> mutex);
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
    pthread_t Pthreads[number_of_pthread];
    pthread_argument arguments[number_of_pthread];
    pthread_mutex_t Mutex;
    pthread_mutex_init(&Mutex, NULL);
    int maxElementI, maxElementJ;
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


    for (int i = 0; i < number_of_pthread; ++i) {
        arguments[i] = {atoi(argv[1]), &matrix, &matrix._b, &x, &maxElementI, &maxElementJ, i, &Mutex};
        pthread_create(&Pthreads[i], NULL, &pthread_func, &(arguments[i]));
    }

    //std::vector<double> solution = jordan_solver(atoi(argv[1]), matrix, matrix._b, x);

    for (int i = 0; i < number_of_pthread; ++i) {
        pthread_join(Pthreads[i], NULL);
    }


    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    // std::vector<double> exact_solution;
    // exact_solution.resize(atoi(argv[1]));
    // for (int i = 0; i < atoi(argv[1]); i += 2)
    // {
    //     exact_solution[i] = 1;
    // }
    // std::vector<double> residual(atoi(argv[1]));
    // for (int i = 0; i < atoi(argv[1]); i++)
    // {
    //     residual[i] = matrix._b[i];
    // }
    // for (int i = 0; i < atoi(argv[1]); i++)
    // {
    //     for (int j = 0; j < atoi(argv[1]); j++)
    //     {
    //         residual[i] -= matrix(i, j) * solution[j];
    //     }
    // }
    // double b_norm = 0;
    // double residual_norm = 0;
    // for (double x : matrix._b)
    // {
    //     b_norm += x * x;
    // }
    // b_norm = sqrt(b_norm);
    // for (double y : residual)
    // {
    //     residual_norm += y * y;
    // }
    // residual_norm = sqrt(residual_norm);
    // for(int i = 0; i < atoi(argv[2]); i++)
    // {
    //     std::cout << std::scientific << solution[i] << " "; 
    // }
    // std::cout << std::endl;
    // std::cout << "Норма невязки = " << std::scientific << residual_norm / b_norm << std::endl;
    // std::vector<double> error(atoi(argv[1]));
    // for (int i = 0; i < atoi(argv[1]); i++)
    // {
    //     error[i] = solution[i] - exact_solution[i];
    // }
    // double error_norm = 0;
    // for (double z : error)
    // {
    //     error_norm += z * z;
    // }
    // error_norm = sqrt(error_norm);
    // std::cout << "Норма погрешности = " << std::scientific << error_norm << std::endl;
    // std::cout << "Время решения системы = " << elapsed_seconds.count() << std::endl;
    // //cout << matrix;
    return 0;
}
