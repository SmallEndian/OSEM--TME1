#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define printf_r(...)					\
	do{						\
		pthread_mutex_lock(&output_lock);	\
		printf(__VA_ARGS__);			\
		pthread_mutex_unlock(&output_lock);	\
	}while(0)

// DEFINE "./text" pour linux ou "/bin/TEXT" pour tsar
#define PATH "/bin/TEXT"
//#define PATH "./text"
static pthread_mutex_t output_lock = PTHREAD_MUTEX_INITIALIZER;
static int total_words = 5000;
static int word_buffer_size;
static char ** word_buffer;
static int count_array[32];
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct arg_struct {
	int worker_id;
	int index_base;
	int index_range;
};

void *thread_func(void *arg)
{
	int pid;
	int tid;
	struct arg_struct *argz;
	int i;
	int count_local[32];
	//recupere les identifiants du thread
	pid = getpid();
	tid = pthread_self();	
	//recupere les parametres
	argz = (struct arg_struct *) arg;
	
	memset(count_local, 0, 32 * sizeof(int)); //RAZ tableau
	//
	//affichage de debut de thread
	printf_r("----------------------\n");
	printf_r("Thread created    pid %d, tid %d   parameters: worker_id %d   index_base %d   index_range %d \n",
			pid,
			tid,
			(int) argz->worker_id,
			(int) argz->index_base,
			(int) argz->index_range);
	printf_r("----------------------\n");


	// comptage
	for(i= argz->index_base; i< (argz->index_base + argz->index_range); i++)
	{	
	pthread_mutex_lock(&mutex);
		if ( strlen(word_buffer[i]) <= 0)
			count_array[0]++;
		else if ( strlen(word_buffer[i]) >= 33)
			count_array[31]++;
		else
			count_array[strlen(word_buffer[i])-1]++;
		//printf_r("worker n %d scanning %d eme mot..\n",argz->worker_id,i);
		// unlock
	pthread_mutex_unlock(&mutex);
	}
		// lock
		// ecriture
	/*
	 *	Les mutex c'est cher. Évitons les!
	 */
/*
	pthread_mutex_lock(&mutex);
	for(i = 0;i <32; i++){
		count_array[i] += count_local[i];
	}
	pthread_mutex_unlock(&mutex);
	*/

	free(arg);
	return NULL;
}



void prepare(){

	int i;
	FILE *f;
	char word[32];

	f = fopen(PATH, "r");
	if(f == NULL)
		perror("fopen"), exit(-1);


	word_buffer = malloc(sizeof(char *) * word_buffer_size);
	if(word_buffer == NULL)
		perror("malloc"), exit(-1);


	printf("Malloc done!\n");


	memset(word_buffer, 0, sizeof(char *) * word_buffer_size);

	for(i=0;i<word_buffer_size;i++){

		fscanf(f, "%31s", word);

		//word_buffer[i] = malloc( sizeof(char) * strlen(word));
		word_buffer[i] = malloc( 32 );

		if(word_buffer[i] == NULL)
			perror("Malloc (for)"), exit(-1);

		strncpy(word_buffer[i], word, 31);
	}
}

void cleanup(){

	int i;
	for(i=0;i<word_buffer_size; i++)
		free(word_buffer[i]);
	free(word_buffer);

}


void test(){

	int i = 0;
	for(i=0;i<word_buffer_size;i++){
		printf(" %s ", word_buffer[i]);
	}
	printf("DONE!\n");

}

int main(int argc, char *argv[])
{

	pthread_t *tids_tbl;
	int workers_nb;
	struct arg_struct*  state;
	int i,err;
	clock_t start;	
	int taken;
	// allocation du tableau contenant les IDs des threads, un thread pour chaque core
	workers_nb = sysconf(_SC_NPROCESSORS_ONLN);
	workers_nb = (workers_nb < 1) ? 1 : workers_nb;
	tids_tbl = malloc(sizeof(pthread_t) * workers_nb);
	
	//initialisation du tableau de comptage	
	memset(count_array, 0, sizeof(int) * 32);
	
	// calcul du nombre de mots consideres dans le fichier texte relation de proportion avec le nombres de coeurs actifs
	word_buffer_size = total_words;// * workers_nb/256;
	printf("Nombre de mots consideres: %d           Nombre de workers : %d\n", word_buffer_size, workers_nb);
	
	// lecture du fichier texte et stockage des mots dans word_buffer
 	prepare();

	// creation des threads workers
	printf("Creation des threads...\n");	
	if(tids_tbl == NULL)
	{
		printf("Main: failed to allocate tids_tbl\n");
		exit(-1);
	}


	start = clock();
	
	for(i = 0; i < workers_nb; i++)
	{	
		// allocation des arguments. 
		struct arg_struct *args = malloc(sizeof(struct arg_struct));
		args->worker_id = i;
		args->index_base =i * (word_buffer_size / workers_nb);
		args->index_range = word_buffer_size/workers_nb;
		
		err = pthread_create(&tids_tbl[i], NULL, thread_func,args); 
		
		if(err)
		{
			printf_r("Main: failed to create thread #%d, err %d\n", i, err);
			break;
		}
	}

	// calcul du temps d'allocation et de creation des threads
        taken = (int)(clock() - start);
	printf_r("[Temps de creations des threads : %d cycles]\n",taken);
        start = clock();	

	workers_nb = i;	
	// attente de terminaisons des threads
	for(i = 0; i < workers_nb; i++)
	{
		err = pthread_join(tids_tbl[i], (void**) &state);
		
		if(err)
			printf_r("Main: failed to join thread #%d, err %d\n", i, err);
		else
			printf_r("Main: thread #%d has finished\n ",i);//%d\n", i, state);

		// desaloue la structure passee en argument ..?
		//free(state);
	}

	// test
	err = 0;
	for(i=0;i<32;i++)
		err += count_array[i];
	if(err != word_buffer_size)
		printf("#####################Tout les mots n'ont pas ete scannés!\n");

	
	// calcul de temps de traitement
	taken = (int)(clock()- start);
	printf_r("[Temps de traitement : %d cycles]\n",taken);

	// affichage des resultats	
	for(i=0;i<32;i++)
		printf("il y a %d mots avec %d caracteres\n", count_array[i], i+1);	
	
	// desaloue le tableau des threads
	free(tids_tbl);
	// desaloue le tableau des mots 
	cleanup();

	//Demande de fermeture du simulateur
	printf("TheEndIsTheMeanToAnEnd\n");

	return 0;
}
