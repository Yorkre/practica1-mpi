#include <iostream>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Control de condición previa: se requieren al menos 2 nodos
    if (size < 2) {
        if (rank == 0) {
            std::cerr << "Error: La topología en anillo requiere como mínimo 2 procesos." << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    int numero = 0;

    if (rank == 0) {
        // --- PROCESO 0: INICIADOR DEL ANILLO ---
        std::cout << "Introduce un número entero para iniciar el anillo: " << std::flush;
        std::cin >> numero;

        // VUELTA 1: Primer envío al nodo siguiente
        MPI_Send(&numero, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

        // VUELTA 1: Espera el cierre del anillo desde el último nodo
        MPI_Recv(&numero, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Soy el nodo " << rank << " y he recibido el valor " << numero 
                  << " (Cierre de vuelta 1)" << std::endl;

        // VUELTA 2: Reenvío para iniciar la segunda vuelta
        MPI_Send(&numero, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

    } else if (rank == size - 1) {
        // --- ÚLTIMO PROCESO (N - 1) ---

        // VUELTA 1: Recibe de N-2 y cierra el ciclo enviando al nodo 0
        MPI_Recv(&numero, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Soy el nodo " << rank << " y he recibido el valor " << numero << std::endl;
        MPI_Send(&numero, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        // VUELTA 2: Recibe de N-2 y decreta el fin del anillo
        MPI_Recv(&numero, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Soy el nodo " << rank << " y he recibido el valor " << numero << std::endl;
        std::cout << "Fin del anillo." << std::endl;

    } else {
        // --- PROCESOS INTERMEDIOS (1 <= rank < N - 1) ---
        // Ejecutan idéntico patrón simétrico en ambas vueltas
        for (int vuelta = 1; vuelta <= 2; ++vuelta) {
            MPI_Recv(&numero, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Soy el nodo " << rank << " y he recibido el valor " << numero << std::endl;
            MPI_Send(&numero, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
