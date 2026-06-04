#include "codexion.h"


typedef struct t_node
{
	int	number_of_coders;
}	trheads;

void *routine_di_prova(void *arg) 
{
	trheads *a = (trheads*)arg;
	int x = a->number_of_coders; 
    printf("Thread: Ho iniziato a lavorare...\n");
    sleep(2);
    printf("Thread: Lavoro finito! e io sono %d\n", x);
    return NULL;
}

int main(int ac, char **av) 
{

    int num = atoi(av[1]);
    pthread_t th[num]; 
    int x = 0;
	

    printf("Main: Assumo tutti thread.\n");
    trheads *a = malloc(sizeof(trheads)*num);
    while(x < num)
	{
		a[x].number_of_coders = x;
		pthread_create(&th[x], NULL, &routine_di_prova, &a[x]);
		x++;
	}

    printf("Main: Aspetto che finisca...\n");
    x = 0;
    while(x < num)
	{
		pthread_join(th[x], NULL);
		x++;
	}
	free(a);
	
    
    printf("Main: Il thread ha finito. Chiudo la cucina!\n");
    return 0;
}