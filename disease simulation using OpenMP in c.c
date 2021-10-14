#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

struct Graph
{
    int id1, id2;
    struct Graph *next;
};

struct info_list
{
    int id, infected, alive, immunity, daysLeftToRecover, died_date, day_infected;
    struct info_list *next;
};

struct Graph *graph = NULL;
struct info_list *head = NULL;

struct info_list *create_infected_node(int id, int day)
{
    struct info_list *new_node = malloc(sizeof(struct info_list));
    new_node->id = id;
    new_node->next = NULL;
    new_node->alive = 1;
    new_node->daysLeftToRecover = 10;
    new_node->immunity = 0;
    new_node->infected = 1;
    new_node->died_date = 0;
    new_node->day_infected = day;
    return new_node;
}

struct info_list *search_infected(int id)
{
    struct info_list *temp = head;
    
        while (temp)
        {
            if (temp->id == id)
                return temp;
            temp = temp->next;
        }
       
    
    return NULL;
}

void insert_infected(int id, int day)
{
    if (!head)
    {
        head = create_infected_node(id, day);
    }
    else
    {
        if (!search_infected(id))
        {
            struct info_list *new_node = create_infected_node(id, day);
            new_node->next = head;
            head = new_node;
        }
    }
}

struct Graph *create_graph_node(int id1, int id2)
{
    struct Graph *new_node = malloc(sizeof(struct Graph));
    new_node->next = NULL;
    new_node->id1 = id1;
    new_node->id2 = id2;
    return new_node;
}

int search_in_graph(int id1, int id2)
{
    struct Graph *tmp = graph;
    while (tmp)
    {
        if ((tmp->id1 == id1 || tmp->id1 == id2) && (tmp->id2 == id1 || tmp->id2 == id2))
            return 1;
        tmp = tmp->next;
    }
    return 0;
}

void insert_graph(int id1, int id2)
{
    if (!graph)
    {
        graph = create_graph_node(id1, id2);
    }
    else
    {
        if (!search_in_graph(id1, id2))
        {
            struct Graph *new_node = create_graph_node(id1, id2);
            new_node->next = graph;
            graph = new_node;
        }
    }
}

void print(int day)
{
    int new_cases = 0, total_cases = 0, recovered = 0, active = 0, new_deaths = 0, total_deaths = 0;
    struct info_list *tmp = head;
    while (tmp)
    {
        if (tmp->day_infected == day && tmp->alive)
            new_cases++;
        if (tmp->immunity)
            recovered++;
        if (tmp->alive)
            active++;
        else
        {
            total_deaths++;
            if (tmp->died_date == day)
                new_deaths++;
        }
        total_cases++;
        tmp = tmp->next;
    }
    printf("%d,\t %d,\t\t %d,\t\t %d,\t %d,\t  %d, \t\t%d\n", day, new_cases, total_cases, recovered, active, new_deaths, total_deaths);
}

int coin_flip(float max)
{

    float res=((float)(rand())/(float)((RAND_MAX)) * 100);
    if (res <= max)
        return 1;
    return 0;
}
void phase1(struct Graph *el, int day)
{
   
    struct info_list *res = search_infected(el->id1);
    if (res && res->alive)
    {
        if (coin_flip(50))
            insert_infected(el->id2, day);
    }
}

void phase2(struct Graph *el, int day)
{
    struct info_list *res1 = search_infected(el->id1);
    if (!res1)
    {
        struct info_list *res2 = search_infected(el->id2);
        if (res2 && res2->alive && res2->daysLeftToRecover < 10) //an einai infected k zwntanos kai den einai i prwth mera p ein infected
        {
            if (coin_flip(50))
                insert_infected(el->id1, day);
        }
    }
}

void death_or_immunity(struct info_list *temp,int day)
{
    if (coin_flip(0.34) && temp->alive && temp->day_infected < day && !temp->immunity)
    {
        temp->alive = 0;
        temp->died_date = day;
    }
    
    if (temp->daysLeftToRecover == 0 && temp->alive)
        temp->immunity = 1;
    if (temp->alive && temp->day_infected < day)
        temp->daysLeftToRecover--;
}

void readfile(char const *const name)
{

    char const *const fileName = name;
    FILE *file = fopen(fileName, "r");
    FILE *file1 = fopen(fileName, "r");

    if (!file)
    {
        printf("\n Unable to open : %s ", fileName);
        exit(-1);
    }

    char line[500];
    char y;
    int x, first;
    int count;
    int flag = 0;
    while (((count = fscanf(file, "%d", &x)) != EOF) && (fscanf(file1, "%c", &y)) != EOF)
    {

        if (count == 1) //an einai int tha gurisei 1
        {
            flag++;
            if (flag == 2)
            {
                insert_graph(first, x);
                flag = 0;
            }
            else
                first = x;
        }
        else // an de gurisei 1 einai char
        {
            if (y == '#') // an o char einai o # kane consume mexri na vreis new line
            {
                do
                {
                    fgetc(file);
                    y = fgetc(file1);
                } while (y != '\n');
            }
            else // alliws kane consume ena aplo char
                fgetc(file);
        }
    }

    fclose(file);
}
void read_infected(char const *const name)
{

    char const *const fileName = name;
    FILE *file = fopen(fileName, "r");
    FILE *file1 = fopen(fileName, "r");

    if (!file)
    {
        printf("\n Unable to open : %s ", fileName);
        exit(-1);
    }

    char line[500];
    char y;
    int x;
    int count;

    while (((count = fscanf(file, "%d", &x)) != EOF) && (fscanf(file1, "%c", &y)) != EOF)
    {

        if (count == 1) //an einai int tha gurisei 1
        {

            insert_infected(x, 1);
        }
        else // an de gurisei 1 einai char
        {
            if (y == '#') // an o char einai o # kane consume mexri na vreis new line
            {
                do
                {
                    fgetc(file);
                    y = fgetc(file1);
                } while (y != '\n');
            }
            else // alliws kane consume ena aplo char
                fgetc(file);
        }
    }

    fclose(file);
}
struct Graph *temp = NULL;
struct info_list* tmp=NULL;
int counter=0;
void calculate(int days)
{
    #pragma omp parallel  shared(days) 
        #pragma omp single
        {
           for(counter=1;counter<days;counter++){
                for (temp = graph; temp != NULL; temp = temp->next)
                {                
                    #pragma omp task firstprivate(temp,counter)
                    phase1(temp, counter+1);   
                }
                #pragma omp taskwait
                for (temp = graph; temp != NULL; temp = temp->next)
                {
                    #pragma omp task firstprivate(temp,counter)
                    phase2(temp, counter+1);
                }
                #pragma omp taskwait
            
                for (tmp = head; tmp != NULL; tmp = tmp->next)
                {
                    #pragma omp task firstprivate(tmp,counter)
                    death_or_immunity(tmp,counter+1);
                }
                #pragma omp taskwait
                #pragma omp task firstprivate(counter)      
                print(counter+1); 
           }
        }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    if (!argv[1] || !argv[2] || !argv[3])
    {
        printf("Correct format of args is:'Graph.txt infected.txt days'\n");
        exit(-1);
    }
    
    char const *const filename = argv[1];
    char const *const filename1 = argv[2];
    //double start =omp_get_wtime();
    time_t start = time(NULL);
    int days = atoi(argv[3]);
    readfile(filename);
    read_infected(filename1);
    int i = 0;
    printf("Day,    NewCases,   TotalCases,   Recovered,   Active,   NewDeaths,   TotalDeaths\n");
    print(1);
    calculate(days);
    //double time = omp_get_wtime()-start;
    time_t end = time(NULL);


    float time=difftime(end, start);

  
    printf("Time elapsed in seconds %f\n",time);
   

    return 0;
}
