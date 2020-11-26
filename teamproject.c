#define DEBUG_STATE 1//1=ON,0=OFF

#define ARRIVAL_MAX_TIME 20
#define NUMBER_OF_ARTIFACTS 20


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct item { 
	int type; 
	int arrival_time; 
	int waiting_time; 
	int processed_time; 
	char id[50]; 
	struct item* next; 
} Artifact;

typedef struct { 
	int condition; 
	Artifact* pocessed_item; 
} Machine;

typedef struct {
	int count;
	Artifact* head;
	Artifact* rear;
	Machine* machine;
}Line;

typedef struct {
	int count;
	Line** line_array;//array of line
}Line_set;

typedef struct {
	Artifact* order_list;
	Line_set* type1;
	Line_set* type2;
	Line_set* type3;
}Factory;

void data_gen(char* input);
Line * line_select(line_set*);
int main (){
    LineSet* type =(LineSet*) malloc(sizeof(LineSet));
    type->line_array = (Line**)malloc(10*sizeof(Line*));
    type->count = 0;
    int i = 0;
    for(i = 0;i < 10;i++){
        type->line_array[i] = (Line*)malloc(sizeof(Line));
        type->line_array[i]->count = 3-i;
    }
    Line* temp = line_select(type);
    printf("%d",temp->count);
    //test
}
void data_gen(char* input){//User should input the pointer of the string, there is an example under the code.
    if(DEBUG_STATE){
	    printf("data_gen()==>START!\n");
    }
    int i;
    int count=NUMBER_OF_ARTIFACTS;
    //you can change the the file name for here.
    strcpy(input,"Test_data.txt");
    FILE* Test_data;
    //initial the random function for the purpose of to not to get the same number every time.
    srand(time(NULL));
    Test_data=fopen(input,"w");
    //generate some testing data, you can control the counts of testing by the variable "count"
    for(i=0;i<count;i++){ 
        //(random)From Type1 to Type3
        fprintf(Test_data,"Type%d ",(rand()%3)+1);             
        //(random)From 0 to (count-1)
        fprintf(Test_data,"ID:%6d  ",i);         
        //(random)From 0 to (ARRIVAL_MAX_TIME-1)
        fprintf(Test_data,"Arrival_time:%6d",rand()%ARRIVAL_MAX_TIME); 
        fprintf(Test_data,"\n");
    }
    //insert "END" at the end of FILE, if it make it difficult for you to read the testing data, you can delete this line as will, ha ha.
    fprintf(Test_data,"END\n");                             
    fclose(Test_data);
    if(DEBUG_STATE){
	    printf("data_gen()==>END!\n");
    }
    //Example
    /*
    int i;
    char FileName[20],temp[99];
    FILE *INPUT;

    data_gen(FileName);
    printf("%s",FileName);
    INPUT=fopen(FileName,"r");
    printf("\n");
    for(i=0;i<10;i++)
    {
        fscanf(INPUT,"%s",temp);
        printf("%5s",temp);//Type0(random)
        fscanf(INPUT,"%s",temp);
        printf("%6s",temp);//ID:
        fscanf(INPUT,"%s",temp);
        printf("%6s",temp);//6245(random)
        fscanf(INPUT,"%s",temp);
        printf("%15s",temp);//Arrival_time:
        fscanf(INPUT,"%s",temp);
        printf("%6s\n",temp);//254(random)
    }
    fclose(INPUT);
    */
}

/*Select the line that has the least artifacts on it*/
/*Parameters: the pointer of the type of LineSet 
Return: the pointer of the line selected*/
Line * line_select(line_set * type){
    if(DEBUG_STATE){
	    printf("line_set()==>START!\n");
    }
    int i;
    if(type->count<=0){
        printf("ERROR, THE COUNT IN THE LINESET <=0\n");
        return NULL;
    }//foolproof
    int min = type->line_array[0]->count;//set the first one as the minimum
    Line* temp = type->line_array[0];
    int size = type->count;
    /*finding the minimum by greedy algorithm*/
    for(i = 1;i < size; i++){
        if(min> type->line_array[i]->count){
            min = type->line_array[i]->count;
            temp = type->line_array[i];
        }
    }
    if(DEBUG_STATE){
	    printf("data_gen()==>END!\n");
    }
    return temp;//return the line's pointer
}
