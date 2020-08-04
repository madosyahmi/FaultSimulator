#define SIZE 50000 //Size of queue, max at SIZE-1

struct queue{
    unsigned int list[SIZE];
    unsigned int head; //Save the loc of data's head (Read DONE)
    unsigned int tail; //Save the next loc of data (Not Read)
};

int enqueue(struct queue *qp,int data){
    if(((qp->head)-1 == (qp->tail)) || ((qp->head)==0 && (qp->tail)==(SIZE-1)) ){
        //Queue FULL
        return -1;
    }
    qp->list[qp->tail] = data;
    qp->tail = (((qp->tail) +1)%SIZE);
    return 0;
}

int dequeue(struct queue *qp){
    if((qp->head)==(qp->tail)){
        //Queue EMPTY
        return -1;
    }
    (qp->head)++;
    if((qp->head) >= SIZE){
        qp->head = 0;
        return (int)qp->list[(SIZE-1)];
    }else{
        return (int)qp->list[((qp->head) - 1)];
    }
}

void callqueue(struct queue *qp){
    int i;
    if((qp->head)==(qp->tail)){
        printf("Empty\n");
        return;
    }
    else{
        i=0;
        do{
            printf("%d ",qp->list[((qp->head)+i)%SIZE]);
            i++;
        }while(qp->list[qp->tail] != qp->list[((qp->head)+i)%SIZE]);
        printf("\n");
        return;
    }
}
