#include "mpi.h"
#include <stdio.h>


#define N 1000000 // define N 1000000



int main(int argc,char *argv[])
{
   int id;               /* Process rank */
   int p;
   double ans = 0;
   double global_solution = 0;
   int solution;
   MPI_Init (&argc, &argv); 
   MPI_Comm_rank (MPI_COMM_WORLD, &id);
   MPI_Comm_size (MPI_COMM_WORLD, &p);
   double elapsed_t;
   int n,d,i;
   elapsed_t = -MPI_Wtime(); //set elapsed_t
   if(id == 0)
   {
   		n = 5;
		d = 10;
		
		printf("Enter the harmonic progression %d and  %d (digits) \n",n,d);
		//scanf("%d %d",&n,&d);
		//MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
		//MPI_Bcast(&d,1,MPI_INT,0,MPI_COMM_WORLD);
   }
   


   MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
   MPI_Bcast(&d,1,MPI_INT,0,MPI_COMM_WORLD);
   MPI_Barrier(MPI_COMM_WORLD);
   
   double frac;
   
   for(i = id+1; i <= n;i+= p)
   {
   		frac = (double) 1 / (double) i;
   		ans += frac;
   }
   //printf(" processor id : %d  ,and the n = %d, and compute the value %lf \n ",id,n,ans); 
   
   //printf(" processor id : %d  , and compute the value %lf \n ",id,ans);
   MPI_Reduce(&ans,&global_solution,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
   
   MPI_Barrier(MPI_COMM_WORLD);
   
   
   if(id == 0)
   {
   		//printf("value of harmonica with 1 to %d is  %.*lf \n",n,d,global_solution);
   		elapsed_t +=  MPI_Wtime();
   		printf("%f is execution time for %d processor \n",elapsed_t,p);
   }
   
   fflush (stdout);
   MPI_Finalize();
   return 0; // shut down process's mpi
   
   
   
   
   
   
   
   
   
   
   
}
