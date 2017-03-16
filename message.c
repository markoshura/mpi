#include <mpi.h>
#include <stdio.h>
int main(int argc, char **argv)
{ 
int size, rank, i, count;
 double Data[10];  
MPI_Status status;
MPI_Init(&argc, &argv); 

MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

if(rank == 0) { for(i=0;i<10;i++)
Data[i]=i; 

MPI_Send( Data,  5, MPI_DOUBLE, 1, 1,MPI_COMM_WORLD);}
else {
MPI_Recv( Data,10, MPI_DOUBLE, 0, 1,  MPI_COMM_WORLD, &status);
printf("rank = %d Received %d elems", rank, count);
MPI_Finalize(); 
return 0; }
