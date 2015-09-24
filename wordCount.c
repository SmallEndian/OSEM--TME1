#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define PATH "/tmp/text"

static char ** word_buffer;
static int word_buffer_size;


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

		//printf("About to small malloc, strlen = %d\n", strlen(word));

		//word_buffer[i] = malloc( sizeof(char) * strlen(word));
		word_buffer[i] = malloc( 32 );

		//printf("Did small malloc\n");

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
	for(i=0;i<100;i++){
		printf(" %s ", word_buffer[i]);
	}
	printf("DONE!\n");

}

int main(void){

	word_buffer_size = 1000;

	printf("Prepare:\n");
	prepare();

	printf("test\n");
	test();

	printf("cleanup\n");
	cleanup();



	return 0;
}
