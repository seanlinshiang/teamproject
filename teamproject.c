#define DEBUG_STATE 1//1=ON,0=OFF

#define ARRIVAL_MAX_TIME 20
#define NUMBER_OF_ARTIFACTS 20

#define TYPE1_Processed_Time 1//processed time for type1
#define TYPE2_Processed_Time 2//processed time for type2
#define TYPE3_Processed_Time 3//processed time for type3

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
	Artifact* processed_item; 
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
Artifact* getdata_from_file(FILE*, int);
void insert_to_orderlist(Artifact**, Artifact*);
void create_orderlist(char[], Factory*);
Line * line_select(Line_set*);
void simulation(Factory*, int);
Artifact * dequeue(Line *, int);
void machine_refresh(Factory*, int);
void machine_refresh_type(Line_set*,int);
void enqueue(Line *,Artifact *);
int sum_up(LineSet *);

int main (){
    char filename [20];
    Factory* factory;
    /* IMPORTANT!! ONLY UNCOMMENT BELOW WHEN YOUR FUNCTION IS READY */
    /*PreSet*/
    
    data_gen(filename);
    //factory = fact_spec();
    //create_ord();
	
    /*Simulation*/
	
    //simulation(factory, ask_time());
    //sum_up(factory->type1);
    //sum_up(factory->type2);
    //sum_up(factory->type3);
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
    //Initialize the random function for the purpose of to not to get the same number every time.
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

Factory* fact_spec(){
    if(DEBUG_STATE)printf("fact_spec()==>START!\n");

    //Read the factory setting data from file
    FILE *Fact_spec_file;
    int i;
    int Type_count[3];
    char temp[10];
    char filename[30];
    //if(DEBUG_STATE)printf("fact_spec()read file==>START!\n");
    //You can change the filename from here
    strcpy(filename,"Factspec.txt");
    Fact_spec_file=fopen(filename,"r");
    fscanf(Fact_spec_file,"%s %d",temp,&Type_count[0]);
    fscanf(Fact_spec_file,"%s %d",temp,&Type_count[1]);
    fscanf(Fact_spec_file,"%s %d",temp,&Type_count[2]);
    fclose(Fact_spec_file);
    //if(DEBUG_STATE)printf("fact_spec()read file==>END!\n");

    //Initialize each data in the "NEW_factory"
    Factory* New_factory;

    New_factory=(Factory*)malloc(sizeof(Factory));

    New_factory->order_list=NULL;
    //Type1
    New_factory->type1=(Line_set*)malloc(sizeof(Line_set));
    New_factory->type1->count=Type_count[0];
    New_factory->type1->line_array=(Line**)malloc(Type_count[0]*sizeof(Line*));
    for(i=0;i<Type_count[0];i++){
        New_factory->type1->line_array[i]=(Line*)malloc(sizeof(Line));
        //Initialize
        New_factory->type1->line_array[i]->count=0;
        New_factory->type1->line_array[i]->head=NULL;
        New_factory->type1->line_array[i]->machine=(Machine*)malloc(sizeof(Machine));
        New_factory->type1->line_array[i]->machine->condition=0;
        New_factory->type1->line_array[i]->machine->processed_item=NULL;
        New_factory->type1->line_array[i]->rear=NULL;
    }


    //Type2
    New_factory->type2=(Line_set*)malloc(sizeof(Line_set));
    New_factory->type2->count=Type_count[1];
    New_factory->type2->line_array=(Line**)malloc(Type_count[1]*sizeof(Line*));
    for(i=0;i<Type_count[1];i++){
        New_factory->type2->line_array[i]=(Line*)malloc(sizeof(Line));
        //Initialize
        New_factory->type2->line_array[i]->count=0;
        New_factory->type2->line_array[i]->head=NULL;
        New_factory->type2->line_array[i]->machine=(Machine*)malloc(sizeof(Machine));
        New_factory->type2->line_array[i]->machine->condition=0;
        New_factory->type2->line_array[i]->machine->processed_item=NULL;
        New_factory->type2->line_array[i]->rear=NULL;
    }


    //Type3
    New_factory->type3=(Line_set*)malloc(sizeof(Line_set));
    New_factory->type3->count=Type_count[2];
    New_factory->type3->line_array=(Line**)malloc(Type_count[2]*sizeof(Line*));
    for(i=0;i<Type_count[2];i++){
        New_factory->type3->line_array[i]=(Line*)malloc(sizeof(Line));
        //Initialize
        New_factory->type3->line_array[i]->count=0;
        New_factory->type3->line_array[i]->head=NULL;
        New_factory->type3->line_array[i]->machine=(Machine*)malloc(sizeof(Machine));
        New_factory->type3->line_array[i]->machine->condition=0;
        New_factory->type3->line_array[i]->machine->processed_item=NULL;
        New_factory->type3->line_array[i]->rear=NULL;
    }


    if(DEBUG_STATE)printf("fact_spec()==>END!\n");
    return New_factory;
}


Artifact* getdata_from_file(FILE* input, int type) {
	Artifact* new_item = (Artifact*)malloc(sizeof(Artifact));
	char id_title[20];
	char arrival_time_title[20];
	new_item->type = type;
	fscanf(input, "%s%s", id_title, new_item->id);
	new_item->waiting_time = 0;
	switch (type) {
	case 1:
		new_item->processed_time = TYPE1_Processed_Time;
		break;
	case 2:
		new_item->processed_time = TYPE2_Processed_Time;
		break;
	case 3:
		new_item->processed_time = TYPE3_Processed_Time;
		break;
	default:
		break;
	}
	fscanf(input, "%s%d", arrival_time_title, &(new_item->arrival_time));
	new_item->next = NULL;
	return new_item;
}

void insert_to_orderlist(Artifact** node_of_orderlist, Artifact* new_item) {
	if (DEBUG_STATE) {
		printf("insert to orderlist START\n");
	}

	if (*node_of_orderlist == NULL) {
		*node_of_orderlist = new_item;
	}
	else if (new_item->arrival_time <= (*node_of_orderlist)->arrival_time) {
		new_item->next = *node_of_orderlist;
		*node_of_orderlist = new_item;
	}
	else {
		Artifact* tem = (Artifact*)(*node_of_orderlist);
		while (1) {
			if (tem->next == NULL) {
				break;
			}
			else if (new_item->arrival_time > tem->next->arrival_time) {
				tem = tem->next;
			}
			else {
				break;
			}
		}
		if (tem->next == NULL) {
			new_item->next = NULL;
			tem->next = new_item;
		}
		else {
			new_item->next = tem->next;
			tem->next = new_item;
		}
	}

	if (DEBUG_STATE) {
		printf("insert to orderlist END!\n");
	}
}

void create_orderlist(char filename[50], Factory* factory) {
	if (DEBUG_STATE) {
		printf("create order list START\n");
	}

	FILE* input = fopen(filename, "r");//open the file where the data of artifact is
	int type;
	char type_title[20];

	/*before reach end of file, read data from file and insert them in ascending order according to arrival time. */
	while (fscanf(input, "%s%d", type_title, &type) != EOF) {
		//insert_to_orderlist is used to put artifact to the right position in orderlist
		//getdata_from_file is used to read data from file then put data into a artifact
		insert_to_orderlist(&(factory->order_list), getdata_from_file(input, type));
	}
	fclose(input);

	if (DEBUG_STATE) {
		printf("create order list END\n");
	}
}

/*Select the line that has the least artifacts on it*/
/*Parameters: the pointer of the type of LineSet 
Return: the pointer of the line selected*/
Line * line_select(Line_set * type){
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

void simulation(Factory* factory, int Max_time) {
	if (DEBUG_STATE) {
		printf("Simulation START\n");
	}

	Artifact* tem = (Artifact*)factory->order_list;
	/*t is the time unit*/
	for (int t = 0; t < Max_time; t++) {
		//while there is still artifact isn't been insert enqueue into the line
		if (tem != NULL) {
			//when artifact's arrival_time == t, enqueue artifact into the correct line. 
			while (tem->arrival_time == t) {
				Artifact * temnext = tem->next;
				switch (tem->type)
				{
				//Type 1
				case 1:
					enqueue(line_select(factory->type1), tem);
					break;
				//Type 2
				case 2:
					enqueue(line_select(factory->type2), tem);
					break;
				//Type 3
				case 3:
					enqueue(line_select(factory->type3), tem);
					break;
				default:
					break;
				}
				tem = temnext;
			}
		}
	//Refresh the machine
        machine_refresh(factory, t);
	}

	if (DEBUG_STATE) {
		printf("Simulation END\n");
	}
}


Artifact * dequeue(Line *line, int t)
{
    if (DEBUG_STATE) {
		printf("dequeue START\n");
	}
    if(line->count==0){
	    return NULL;
    }
    line->rear->wait = t - line->rear->arrival_time;
    Artifact * temp;          //memory line->head
    Artifact * pre;       //previous memory
    temp = line->head;
    if(temp == line->rear){
	 line->rear = NULL;
    }else{
	while(1)
    	{
        	if(temp!= line->rear)
        	{
            		pre  = temp;
            		temp = temp->next;
        	}
		if(temp == line->rear)
        	{
            		line->rear = pre;
            		break;
        	}
    	}
    }	    

    line->count--;
    if (DEBUG_STATE) {
        printf("dequeue END\n");
    }
    return temp;
    
}

/* This function refreshes the machine on every single line*/
/*Parameters: the pointer of the whole factory and the current time
Return: void*/
void machine_refresh(Factory* factory, int t){
    if(DEBUG_STATE){
	    printf("machine_refresh()==>START!\n");
    }
    /* go through each type of Line_set*/
    machine_refresh_type(factory->type1,t);
    machine_refresh_type(factory->type2,t);
    machine_refresh_type(factory->type3,t);
    if(DEBUG_STATE){
	    printf("machine_refresh()==>END!\n");
    }
}

void machine_refresh_type(Line_set* type,int t){
    int i;
    /* go through each Line of the Line_set type*/
    for(i=0;i<type->count;i++){
        Line * temp_line = type->line_array[i];
        /*if the machine is not processing any item, then deque the item from the line*/
        if(temp_line->machine->condition==0){
            Artifact * temp_art = dequeue(temp_line,t);
            /*if the item exist, the machine will go processed the item
            and the condition will then be set to 1*/
            if(temp_art!=NULL){
                temp_line->machine->processed_item = temp_art;
                temp_line->machine->condition = 1;
            }
        }else{
            /*The processed time of the processing item decreased by 1*/ 
            temp_line->machine->processed_item->processed_time--;
            /*Then if the processed_time is equal to 0, which implies the item is finished 
            processing, the condition of the machine will be set to 0*/ 
            if(temp_line->machine->processed_item->processed_time==0){
                temp_line->machine->condition = 0;
            }
        }
    }
}

void enqueue(Line * line,Artifact * artifact)
{
    if (DEBUG_STATE) {
		printf("enqueue START\n");
	}
    //artifact = malloc(sizeof(Artifact));
    //line = malloc(sizeof(Line));
    if(line->count == 0)
    {
	artifact->next = line->head;
        line->head = artifact;
        line->rear = artifact;
    }
    else
    {
        artifact->next = line->head;
        line->head     = artifact;
    }
    line->count++;
     if (DEBUG_STATE) {
		printf("enqueue END\n");
	}
}

int sum_up(LineSet * lineset){
    if (DEBUG_STATE) {
		printf("sumup START\n");
	}
    int sum;
    Line * temp;
    temp = line;
    while(1)
    {
        sum += lineset->head->wait;
        if(temp->head->next != NULL)
        {
            temp->head = line->temp->next;
        }
    }

     if (DEBUG_STATE) {
		printf("sumup END\n");
	}
    return sum;
}
