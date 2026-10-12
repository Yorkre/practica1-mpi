#include <iostream>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    // Inicio de la medición de tiempo por proceso
    double t_inicio = MPI_Wtime();

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Mensaje estricto exigido por el guion
    std::cout << "Hola mundo, soy el proceso " << rank 
              << " de un total de " << size 
              << " procesos paralelos." << std::endl;

    // Fin de la medición de tiempo
    double t_fin = MPI_Wtime();
    double t_total = t_fin - t_inicio;

    std::cout << "Proceso " << rank << " ha finalizado en " 
              << t_total << " segundos." << std::endl;

    MPI_Finalize();
    return 0;
}
