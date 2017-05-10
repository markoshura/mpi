
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main( int argc, char* argv[] ) {
    const int N = 100;//число отрезков разбивания
    const float a = 0.0; //начало отрезка интегрирования
    const float b = 20.0;//конец отрезка интегрирования
    double x[N], TotalSum, ProcSum = 0.0;
    int ProcRank, ProcNum, k, i1, i2;
    float h;
    MPI_Status Status;

    // Инициализация
    MPI_Init( &argc, &argv ); // Создаёт community из процессов
    MPI_Comm_size( MPI_COMM_WORLD, &ProcNum ); //  Раздаёт всем процессам их число  - в переменную ProcNUm
    MPI_Comm_rank( MPI_COMM_WORLD, &ProcRank );// Раздаёт каждому процессу его номер, начиная с нуля - в переменную ProcRank

    h = (b-a)/N; //шаг разбиения (нужен для применения метода левых прямоугольников)

    if ( ProcRank == 0 ) {
        for( i1 = 0; i1 <N; ++i1 ) {
            x[i1] = (a+(b-a)*i1/100);//считаем интеграл функции x
        }
    }

    // Рассылка данных на все процессы - коллективная операция
    MPI_Bcast( x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD );

    // Вычисление частичной суммы на каждом из процессов
    // на каждом процессе суммируются элементы вектора x от i1 до i2
    k = N / ProcNum;
    i1 = k * ProcRank;
    i2 = k * ( ProcRank + 1 );
    if ( ProcRank == ProcNum-1 ) i2 = N;
    for ( int i = i1; i <i2; i++ ) {
        ProcSum = ProcSum + x[i];
    }
    // Сбор частичных сумм с процессов - коллективная операция
    MPI_Reduce( &ProcSum, &TotalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );

    // Вывод результата
    if ( ProcRank == 0 ) {
        printf("\nTotal Sum = %10.2f",TotalSum*h); }
    //выводит приближенное значение нашего интеграла (xdx = x^2/2 от 0 до 20)

    MPI_Finalize(); 
    return 0;
}
