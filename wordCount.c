#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define PATH "/tmp/text"

char ** tab;

void prepare(int tab_size){

	int i;
	FILE *f;
	char word[32];

	f = fopen(PATH, "r");
	if(f == NULL)
		perror("fopen"), exit(-1);
	

	tab = malloc(sizeof(char *) * tab_size);
	if(tab == NULL)
		perror("malloc"), exit(-1);

	memset(tab, 0, sizeof(char *) * tab_size);

	for(i=0;i<tab_size-1;i++){
	
		fscanf(f, "%31s", word);
		
		tab[i] = malloc( sizeof(char) * strlen(word));

		if(tab[i] == NULL)
			perror("Malloc (for)"), exit(-1);

		strncpy(tab[i], word, 31);
	}
}

int main(void){


	return 0;
}
