#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/sem.h>

#define SEMAFORO_PADRE 0
#define SEMAFORO_HIJO 1
#define SEMAFORO_ESPIRITU_SANTO 2

int main(int argc, char *argv[]){
	int semid, i = 10, pid;
	struct sembuf operacion;
	key_t llave;

	/*Pregunta 7.1*/
	// printf("sembuf.sem_num: %d\n", operacion.sem_num);
	// printf("sembuf.sem_op: %d\n", operacion.sem_op);
	// printf("sembuf.sem_flg: %u\n", operacion.sem_flg);

	llave = ftok(argv[0], 'U');

	if((semid = semget(llave, 3, IPC_CREAT | 0600)) == -1){
		perror("Error en semget");
		exit(-1);
	}
	semctl(semid, SEMAFORO_PADRE, SETVAL, 1);
	semctl(semid, SEMAFORO_HIJO, SETVAL, 0);
	semctl(semid, SEMAFORO_ESPIRITU_SANTO, SETVAL, 0);

	if((pid = fork()) == -1){
		perror("fork");
		exit(-1);
	}
	else if (pid == 0){
		while (i){
			operacion.sem_flg = 0;
			operacion.sem_op = -1;
			operacion.sem_num = SEMAFORO_HIJO;
			printf("sem_num: %d\n", operacion.sem_num);
			printf("sem_op: %d\n", operacion.sem_op);
			semop(semid, &operacion, 1);

			printf("SOY EL PROCESO HIJO: %d\n", i--);

			operacion.sem_op = 1;
			operacion.sem_num = SEMAFORO_PADRE;
			semop(semid, &operacion, 1);
		}

		semctl(semid, 0, IPC_RMID, 0);
	}else{
			while(i){
			operacion.sem_flg = 0;
			operacion.sem_op = -1;
			operacion.sem_num = SEMAFORO_PADRE;

			semop(semid, &operacion, 1);
			printf("SOY EL PROCESO PADRE: %d\n", i--);

			operacion.sem_op = 1;
			operacion.sem_num = SEMAFORO_HIJO;
			semop(semid, &operacion, 1);

			}

			semctl(semid, 0, IPC_RMID, 0);
		}

	return 0;
}