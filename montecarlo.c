#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char* argv[]){
    int i,id, np,N;
    double x, y;
    int Nincircle;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    N = 1000;
    MPI_Barrier(MPI_COMM_WORLD);

    Nincircle = 0;
    srand(time(0));
    //int local_N = N/np;

    for(i = 0; i<N;i++){
        x = ((double)rand())/((double)RAND_MAX);
        y = ((double)rand())/((double)RAND_MAX);
        if (((x*x) + (y*y)) <= 1) Nincircle++;
    }

    int all_points=0;
    MPI_Allreduce(&Nincircle,&all_points,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
    double result;
    result = (all_points*4)/((double)N*np);
    MPI_Barrier(MPI_COMM_WORLD);


    if (id == 0) {
        printf("Number of Points Used: %d\n",N*np);
        printf("Estimate of Pi: %.4f\n",result);
    }

    MPI_Finalize();
    return 0;
}
