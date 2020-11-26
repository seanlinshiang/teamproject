#include <stdio.h>
#include <stdlib.h>

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

/*Select the line that has the least artifacts on it*/
/*Parameters: the pointer of the type of LineSet 
Return: the pointer of the line selected*/
Line * line_select(LineSet * type){
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
    return temp;//return the line's pointer
}

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
