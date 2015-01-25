#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/sem.h>

#define SEMAFORO_A 0
#define SEMAFORO_B 1

int main(int argc, char *argv[]){
	int semid, i = 10;
	struct sembuf operacion;
	key_t llave;

	llave = ftok(argv[1], 'U');
	if((semid = semget(llave, 2, 0600)) == -1){
		perror("Error en semget");
		exit(-1);
	}

	while(i){

		operacion.sem_flg = 0;
		operacion.sem_op = -1;
		operacion.sem_num = SEMAFORO_B;
		semop(semid, &operacion, 1);

		printf("SOY EL PROGRAMA B: %d\n", i--);

		operacion.sem_op = 1;
		operacion.sem_num = SEMAFORO_A;
		semop(semid, &operacion, 1);

	}

	semctl(semid, 0, IPC_RMID, 0);

	return 0;

}