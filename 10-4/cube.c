
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

#define N 695012
#define MIN(a,b)  ((a)<(b)?(a):(b))

int main (int argc, char *argv[]) {
    int     id;        /* Process rank */
    int     p;         /* Number of processes */
    double  time;
    unsigned short xs[3], ys[3], zs[3];
    float x[N], y[N], z[N];
    int small, size, remainder, start, last;
    float count, total, volume;
    int i;
    
    int check_point (float, float, float);
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    
    MPI_Barrier (MPI_COMM_WORLD);
    time = -MPI_Wtime();
    
    xs[0] = 6;
    xs[1] = 2;
    xs[2] = 4;
    
    ys[0] = 1;
    ys[1] = 6;
    ys[2] = 7;
    
    zs[0] = 4;
    zs[1] = 7;
    zs[2] = 8;
    
    for (i = 0; i < N; i++) {
        x[i] = erand48(xs) * 2.0;
        y[i] = erand48(ys) * 2.0;
        z[i] = erand48(zs) * 2.0;
    }
    
    // 分配工作
    small = N / p;
    size = N / p;
    remainder = N % p;
    
    if (id < remainder) {
        size++;
    }
    
    start = id * small + MIN(id, remainder);
    last = start + size - 1;
    
    // parallel N個點判斷是否有在範圍內
    for (i = start; i <= last; i++) {
        count += check_point(x[i], y[i], z[i]);
    }
    
    MPI_Reduce (&count, &total, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    time += MPI_Wtime();
    
    if (!id) {
        volume = (total * 8) / N;
        
        printf ("The volume of the portion of the cube is %6.5f\n", volume);
        printf ("%d processes: %6.6f seconds\n", p, time);
    }
    
    MPI_Finalize();
    
}

int check_point (float x, float y, float z) {
    float ans;
    
    ans = sin(acos((x + y + z) / (sqrt(3) * sqrt(pow(x,2) + pow(y,2) + pow(z,2))))) * sqrt(pow(x,2) + pow(y,2) + pow(z,2));
    
//    printf("%f\n", ans);
    
    if (ans > 0.15) {
        return 1;
    } else {
        return 0;
    }
}
