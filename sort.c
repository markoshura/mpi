#include <stdlib.h>
#include "stdio.h"
#include "mpi.h"
#include <stdlib.h>
#include <time.h>
//Сортировка пузырьком
//Сначала главный процессор разбивает исходный массив на p равных частей,
// где n кол-во процессов, каждый блок содержит n/p элементов массива.
// Полученные блоки рассылаются по процессам. Далее идет предобработка.
// На этапе предобработки каждый процесс сортирует свой массив методом последовательной сортировки.
// Далее происходит чередование чет-нечетных перестановок на p итерациях.
// На нечетной итерации пары процессов с номерами (0,1)(2,3)(4,5)… обмениваются друг с другом массивами,
// после чего на процессе с меньшим номером остается n/p первых упорядоченных элементов объединенного массива,
// а на процессе с большим номером остается n/p последних упорядоченных элементов объединенного массива.
// На четной итерации происходит все точно так же,
// но обмен производится между процессами с номерами (1,2)(3,4)(5,6)(7,8)… .
// Если в течении 2-х последовательных четной и нечетной итерации не производится никаких изменений,
// то алгоритм может прекратить свою работу заранее.
// После этого части массивов отсылаются на главный процесс и там объединяются, после чего получается отсортированный массив.
int* mergeArrays (int* v1, int n1, int* v2, int n2)
{
    int i, j, k;
    int* result;

    result = (int*)malloc(n1 + n2);
    i = 0;
    j = 0;
    k = 0;

    while (i < n1 && j < n2)
        if (v1[i] < v2[j]) {
            result[k] = v1[i];
            i++;
            k++;
        } else {
            result[k] = v2[j];
            j++;
            k++;
        }

    if (i == n1)
        while (j < n2) {
            result[k] = v2[j];
            j++;
            k++;
        }
    if(j == n2)
        while (i < n1) {
            result[k] = v1[i];
            i++;
            k++;
        }

    return result;
}

void swap (int* v, int i, int j)
{
    int t;
    t = v[i];
    v[i] = v[j];
    v[j] = t;
}

void sort (int* v, int n)
{
    int i, j;

    for (i = n - 2; i >= 0; i--)
        for (j = 0; j <= i; j++)
            if (v[j] > v[j + 1])
                swap (v, j, j + 1);
}



void main (int argc, char ** argv)
{
    int* data;            //Our unsorted array
    int* resultant_array; //Sorted Array
    int* sub;

    int m, n;
    int id, p;
    int r;
    int s;
    int i;
    int z;
    int move;
    MPI_Status status;

    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &id);
    MPI_Comm_size (MPI_COMM_WORLD, &p);

    //Task Of The Master Processor
    if (id == 0) {
        n = 10;
        s = n / p;
        r = n % p;
        data = (int*)malloc(n + s - r);

        srand(time(0));
        printf("SOURCE DATA\n");

        for(i = 0; i < n; i++)
        {
            data[i] = rand()%100+1;
            printf("%d\n", data[i]);
        }


        MPI_Bcast (&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
        resultant_array = (int*)malloc(s); //Allocating result array
        //Sending data array from master to all other slaves
        MPI_Scatter (data, s, MPI_INT, resultant_array, s, MPI_INT, 0, MPI_COMM_WORLD);
        sort (resultant_array, s);
    } else {
        MPI_Bcast (&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
        //Allocating resultant array
        resultant_array = (int*)malloc(s);
        MPI_Scatter (data, s, MPI_INT, resultant_array, s, MPI_INT, 0, MPI_COMM_WORLD);
        //Each slave processor will sort according to the array partitioning n/p
        sort (resultant_array, s); //Sort the array up to index s.
    }

    move = 1;

    //Merging the sub sorted arrays to obtain resultant sorted array
    while (move < p) {
        if (id%(2*move)==0) {
            if (id + move < p) {                     //Receive
                MPI_Recv (&m, 1, MPI_INT, id + move, 0, MPI_COMM_WORLD, &status);
                sub = (int*)malloc(m); //Allocate space for sub array
                MPI_Recv (sub, m, MPI_INT, id + move, 0, MPI_COMM_WORLD, &status);
                //Obtaing resultant array by merging sub sorted array
                resultant_array = mergeArrays (resultant_array, s, sub, m);
                s = s + m;
            }
        } else { //Send datas to neighbour processors
            int near = id - move;
            MPI_Send (&s, 1, MPI_INT, near, 0, MPI_COMM_WORLD);
            MPI_Send (resultant_array, s, MPI_INT, near, 0, MPI_COMM_WORLD);
            break;
        }

        move = move * 2;
    }

    //Final Part, In this part Master CPU outputs the results
    if (id == 0) {
        printf("FINAL RESULTS\n");
        for(i = 0; i < n; i++)
        {
            printf("%d\n",resultant_array[i]);
       }

    }

    MPI_Finalize (); //Finalize MPI environment.
}
