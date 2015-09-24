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

	memset(word_buffer, 0, sizeof(char *) * word_buffer_size);

	for(i=0;i<word_buffer_size;i++){

		fscanf(f, "%31s", word);

		word_buffer[i] = malloc( sizeof(char) * strlen(word));

		if(word_buffer[i] == NULL)
			perror("Malloc (for)"), exit(-1);

		strncpy(word_buffer[i], word, 31);
	}
}

void cleanup(int word_buffer_size){

	int i;
	for(i=0;i<word_buffer_size; i++)
		free(word_buffer[i]);
	free(word_buffer);

}

int main(void){


	return 0;
}
