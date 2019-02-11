#include<stdio.h>
#include<stdlib.h>
#include<math.h>


struct node 
{
    int capacity;
    int residual_capacity;
    char dest;
    char source;
    int flow;
    struct node *next;
    int backward;

};

struct head_node
{
    char source;
    int visited;
    struct node *list;
    struct node *last;
};


struct QNode 
{
    struct node *exact_node;
    struct head_node *one; 
    struct QNode *next;
    struct QNode *back_track; 
}; 
  

struct Queue 
{ 
    struct QNode *front, *rear,*start; 
}; 

struct QNode* newNode(struct head_node *one,struct QNode *parent,struct node *exact_node) 
{ 
    struct QNode *temp = (struct QNode*)malloc(sizeof(struct QNode)); 
    temp->one = one; 
    temp->exact_node = exact_node;
    // temp->parent = parent;
    temp->back_track = parent;
    temp->next = NULL; 
    return temp;  
} 
  
// A utility function to create an empty queue 
struct Queue *createQueue() 
{ 
    struct Queue *q = (struct Queue*)malloc(sizeof(struct Queue)); 
    q->front = q->rear = NULL;
    q->start = NULL; 
    return q; 
} 
  
// The function to add a key k to q 
void enQueue(struct Queue *q, struct head_node *one,struct QNode *parent,struct node *exact_node) 
{ 
    // Create a new LL node 
    struct QNode *temp = newNode(one,parent,exact_node); 
  
    // If queue is empty, then new node is front and rear both 
    if (q->rear == NULL) 
    { 
       q->front = q->rear = temp; 
       if(q->start == NULL)
       {
         q->start = temp;
       }
       return; 
    } 
  
    // Add the new node at the end of queue and change rear 
    q->rear->next = temp; 
    q->rear = temp; 
} 
  
// Function to remove a key from given queue q 
struct QNode *deQueue(struct Queue *q) 
{ 
    // If queue is empty, return NULL. 
    if (q->front == NULL) 
       return NULL; 
  
    // Store previous front and move front one node ahead 
    struct QNode *temp = q->front; 
    q->front = q->front->next; 
  
    // If front becomes NULL, then change rear also as NULL 
    if (q->front == NULL) 
       q->rear = NULL; 
    return temp; 
} 

struct node *create_node()
{
    struct node *temp = (struct node *)malloc(sizeof(struct node));
    // printf("enter the source : ");
    // scanf("%c",&temp->source);
    printf("enter the destination : ");
    scanf(" %c", &temp->dest);
    printf("enter the capacity  : ");
    scanf("%d",&temp->capacity);
    temp->flow  = 0;
    // temp->visited = 0;
    temp->residual_capacity = temp->capacity;
    temp->backward = 0;
    return temp;

}

void add_back_node(struct head_node *src,char dest, int residual_capacity)
{
    struct node *temp = src->list;
    while(temp!= NULL)
    {
        if(temp->dest == dest)
        {
            temp->residual_capacity += residual_capacity;
            return;
        }
        temp = temp->next;
    }
    struct node *back = (struct node *)malloc(sizeof(struct node));
    back->source = src->source;
    back->dest = dest;
    back->residual_capacity = residual_capacity;
    back->backward = 1;
    if(src->last == NULL)
    {
        src->last = back;
    }
    else
    {
        src->last->next = back;
        src->last = src->last->next;

    }
    
 
    return;
}

struct head_node **init_graph(int num)
{
    
    struct head_node** graph = malloc(num*sizeof(struct head_node*));
    return graph;
}

struct head_node ** build_graph(int num)
{
    // // int num = num;
    // printf("enter the number of nodes : ");
    // scanf("%d",&num);
    struct head_node**graph = init_graph(num);
    int i;
    for(i = 0; i < num; i++)
    {
        graph[i] = (struct head_node *)malloc(sizeof(struct head_node));
        printf("enter the source char repr : ");
        scanf(" %c", &graph[i]->source);
        graph[i]->visited = 0;
        printf("enter number of childes for node : %c",graph[i]->source);
        int childs;
        scanf("%d",&childs);
        struct node *temp =NULL;
        while(childs--)
        {
            if(temp == NULL)
            {
                temp = create_node();
                temp->source = graph[i]->source;
                graph[i]->list = temp;
                graph[i]->last = temp;

            }
            else
            {
                temp->next = create_node();
                temp = temp->next;
                graph[i]->last = temp;
            }
            
            
        }
    }

    return graph;


}

void print_graph(struct head_node **graph,int num)
{
    int i;
    for(i = 0; i < num; i++)
    {
        struct node *temp = graph[i]->list;
        printf("\n node %c  ->",graph[i]->source);
        while(temp!= NULL)
        {
            printf("%c -> ",temp->dest);
            temp = temp->next;

        }
    }
}

struct Queue * bfs(struct head_node **graph,char src,char dest,int num)
{
    struct Queue *queue = createQueue();
    
    // struct node *temp = 
    int i;
    for(i = 0; i < num; i++)
    {
        if(graph[i]->source == src)
        {
            enQueue(queue, graph[i],NULL,NULL);
            graph[i]->visited = 1;
            break;
        }
    }

    struct QNode *temp;
    
    while((temp = deQueue(queue)))
    {
        struct node *one = temp->one->list;
        // int check = 0;

        while(one != NULL && one->residual_capacity > 0)
        {
            for(i=0; i < num; i++)
            {
                if(graph[i]->visited == 0 && graph[i]->source == one->dest )
                {
                    enQueue(queue,graph[i],temp,one);
                    graph[i]->visited = 1;
                    if(graph[i]->source == dest)
                    {
                        return queue;
                    }
                    
                    break;
                }
            }
            one = one->next;
           
        }
    
    }

    return NULL;
}

int min(int a , int b)
{
    if(a < b)
    {
        return a;
    }
    else
    {
        return b;
    }
    
}

void max_flow(struct head_node **graph,char src,char dest,int num)
{
    int max_flow = 0;
    struct Queue *q = bfs(graph,src,dest,num);
    while(q!= NULL)
    {
        int path_flow = 9999;
        struct QNode *temp = q->rear;           
        while(temp->back_track != NULL)
        {
            path_flow = min(path_flow,temp->exact_node->residual_capacity);

            temp = temp->back_track;
            // bfs(graph,src,dest,num);
        }
        temp = q->rear; 
        max_flow += path_flow; 
        while(temp->back_track != NULL)
        {
            // path_flow = min(path_flow,temp->exact_node->residual_capacity);
            temp->exact_node->residual_capacity -= path_flow;

            add_back_node(temp->one,temp->back_track->one->source,path_flow);
            temp = temp->back_track;
            // bfs(graph,src,dest,num);
        }


        // struct QNode *clear = q->start;
        // while(clear != NULL)
        // {
        //     clear->one->visited = 0;
        //     clear = clear->next;
        // }

        int i;
        for(i = 0; i < num; i++)
        {
            graph[i]->visited = 0;
        }
        
        q = bfs(graph,src,dest,num);

    }


    printf("\n  max flow : %d",max_flow);

}


int main()
{
    int num;
    printf("enter the number of nodes");
    scanf("%d",&num);
    struct head_node **graph = build_graph(num);
    print_graph(graph,num);

    max_flow(graph,'s','t',num);


}