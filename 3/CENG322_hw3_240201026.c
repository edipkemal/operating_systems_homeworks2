#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*
	Edip Kemal SARDOGAN
	240201026
	this code compiled with gnu99
*/
#define BOOK_NUMBER 10
#define STOP_WORD_NUMBER 851
#define MAX_WORD_SIZE 20

typedef struct{
	char word[MAX_WORD_SIZE];
	int frequencies[BOOK_NUMBER];
}STOP_WORD;

typedef struct{
	int first;//first index 
	int last; //last index
}threadParameter;

char* books[]={"book1.txt","book2.txt","book3.txt","book4.txt","book5.txt","book6.txt","book7.txt","book8.txt","book9.txt","book10.txt"};
char stopwords_filename[]="englishstopwords.txt";
STOP_WORD stop_words[STOP_WORD_NUMBER];
int THREAD_NUMBER;
int* find_number_of_substrings(char* keyword);
void* threadJob(void* param); 
void fillStopWords();
void printWordsFrequencies(int randomNumber);
int main(int argc,char* argv[]){
	srand(time(0));
	if(argc!=2){
		fprintf(stderr,"This program needs 2 arguments. Usuage: <out file> <thread number>\n");
		exit(1);
	}

	if(atoi(argv[1])<1){
		fprintf(stderr, "Thread number must be positive\n");
		exit(1);
	}

	THREAD_NUMBER=atoi(argv[1]);

	int i,first,last;
	fillStopWords();

	struct timespec requestStart, requestEnd;
	

	pthread_t tid[THREAD_NUMBER];

	last=-1;//-1 because will be increased in the for loop

	/*this step size will be changed by thread number*/
	int step=STOP_WORD_NUMBER/THREAD_NUMBER+1;//+1 because we need the upper bound integer of the result

	printf("\nFrequency calculation started..\n");
	clock_gettime(CLOCK_REALTIME, &requestStart); //timer started
	for(i=0;i<THREAD_NUMBER;i++){
		first=last+1;//first search index
		last=first+step;//last search index

		threadParameter param;//will give the first and last search indexes to the thread
		param.first=first;
		param.last=last;

		pthread_create(&tid[i],NULL,threadJob,(void*)&param);
	}

	for(i=0;i<THREAD_NUMBER;i++){
		pthread_join(tid[i],NULL);
	}
	clock_gettime(CLOCK_REALTIME, &requestEnd); //timer ended
	printf("Frequency calculation ended..\n");
	int num1 = (rand() % (STOP_WORD_NUMBER-1)) + 1;
	int num2 = (rand() % (STOP_WORD_NUMBER-1)) + 1;

	printWordsFrequencies(num1);
	printWordsFrequencies(num2);

	double spentTime=(requestEnd.tv_sec - requestStart.tv_sec)+(requestEnd.tv_nsec - requestStart.tv_nsec)/10E9;
	printf("\nTotal time spent %lf s, for %d threads\n\n",spentTime,THREAD_NUMBER);

	pthread_exit(NULL);
}

void printWordsFrequencies(int randomNumber){
	printf("\nfrequency of the word “%s” in all books:\n",stop_words[randomNumber].word);
	int i,freq;

	for(i=0;i<BOOK_NUMBER;i++){
		freq=stop_words[randomNumber].frequencies[i];
		printf("%d",freq);
		if(i!=BOOK_NUMBER-1){
			printf("-");
		}else{
			printf("\n");
		}
	}
}

/*checks all words in given range in the all books */
void* threadJob(void *param){
	threadParameter* parameters=(threadParameter*) param;
	int first=parameters->first;
	int last=parameters->last;

	if(last<STOP_WORD_NUMBER){ //this thread works on end of the array so its last index is last index of the array
		last=STOP_WORD_NUMBER-1;
	}
	int i,k;
	for(i=first;i<last+1;i++){
		char word[MAX_WORD_SIZE];
		snprintf(word,MAX_WORD_SIZE,"%s",stop_words[i].word);
		int* frequencies;
		frequencies=find_number_of_substrings(word);

		for(k=0;k<BOOK_NUMBER;k++){
			stop_words[i].frequencies[k]=frequencies[k];
		}
	}
	pthread_exit(NULL);
}


/*finds the frequencies of a word in all books*/
int* find_number_of_substrings(char* keyword){
	int i,k,num,len;
	FILE *file;
	int *frequencies;
	frequencies=(int*)malloc(BOOK_NUMBER*sizeof(int));
	char c,cl;//char c and lowercase of c -> cl
	len=strlen(keyword);
	for (k=0;k<BOOK_NUMBER;k++){
		char *book=books[k];
		num=0;
		file=fopen(book,"r");
		if(file==NULL){
			printf("There is no such file named : %s",book);
			exit(1);
		}
		c= fgetc(file);
		while(1){
			nextword:
			cl=tolower(c);//we have all lower case letters in stopwords file, so uppercase latters must be turned to lowercase
			if (cl==keyword[0]){
				for(i=1;i<len;i++){
					c=fgetc(file);
					cl=tolower(c);//we have all lower case letters in stopwords file, so uppercase latters must be turned to lowercase
					if(cl!=keyword[i]){
						goto nextword; //words did not match
					}
				}
				num++;
			}
			c = fgetc(file);
			if(c==EOF){
				break;
			}
		}
		fclose(file);
		frequencies[k]=num;
	}
	return frequencies;
}

/*reads the stop word file and put them in STOP_WORD struct array*/
void fillStopWords(){
	int j;
	FILE* stop_file;
	char word[MAX_WORD_SIZE];
	stop_file=fopen(stopwords_filename,"r");
	if(stop_file==NULL){
		printf("There is no such file named : %s",stopwords_filename);
		exit(1);
	}
	for(j=0;j<STOP_WORD_NUMBER;j++){
		fgets(word,MAX_WORD_SIZE,stop_file);
		word[strlen(word)-2]='\0'; //cut the word from last 2 characters. -2 since \n and \r characters
		snprintf(stop_words[j].word,MAX_WORD_SIZE,"%s",word);
	}
	fclose(stop_file);
}