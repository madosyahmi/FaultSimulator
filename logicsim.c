#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include "logicGate.h"
#include "queue.h"
#include "faultCal.h"

//Call included function
int enqueue(struct queue *qp,int data);
int dequeue(struct queue *qp);
void callqueue(struct queue *qp);
char logicsim(int (*signal_line)[6],int line_no,unsigned int *pointer_list);
void faultsim(int (*signal_line)[6],unsigned char **fault_list,int line_no,unsigned int *pointer_list,unsigned int signal_lines,int *cal_no,int now_cal_i);
void errorCall(int errorN);

int main (int argc, char *argv[]){
    int i,j,now_no;
    char filename[511];
    char checkdirtype[25];
    FILE *fp;
    int input_rd,input_rd2;
    
    int (*signal_line)[6];
    unsigned int *PI_list;
    unsigned int *PO_list;
    unsigned int signal_lines;
    unsigned int PI_lists;
    unsigned int PO_lists;
    unsigned int *pointer_list;
    unsigned int pointer_lists;
    
    unsigned char **test_ptn;
    unsigned int test_ptns;
    unsigned int ptn_i;
    
    unsigned char **fault_list;
    unsigned char *sum_fault;
    
    unsigned char *all_fault;
    unsigned int faultcounters;
    unsigned int faults_det;
    
    int *cal_no;
    
    struct queue que;
    que.head = 0;
    que.tail = 0;
    
    //Read Circuit Table
    if(argc < 2){
        printf("ERROR: Please enter file name.\n");
        exit(-1);
    }
    
    strcpy(checkdirtype,argv[1]);
    if(checkdirtype[0] == 'e'){
        strcpy(filename,"./iscas85/Table/");
    }else if(checkdirtype[0] == 'c'){
        strcpy(filename,"./iscas89_cs/Table/");
    }
    strcat(filename,argv[1]);
    strcat(filename,".tbl");
    
    if((fp = fopen(filename,"r")) == NULL){
        errorCall(1);
    }
    
    //Read signal lines
    if(fscanf(fp,"%d",&signal_lines) != 1){
        errorCall(1);
    }
    
    signal_line = malloc(signal_lines*6*sizeof(int));
    
    if(signal_line == NULL){
        printf("ERROR: malloc error @ signalline.\n");
        exit(-1);
    }
    
    for(i=0;i<signal_lines;i++){
        for(j=0;j<5;j++){
            if(fscanf(fp,"%d",&signal_line[i][j]) != 1){
                errorCall(1);
            }
        }
    }
    
    //Read pointer list
    if(fscanf(fp,"%d",&pointer_lists) != 1){
        errorCall(1);
    }
    
    pointer_list = malloc(pointer_lists*sizeof(unsigned int));
    
    if(pointer_list == NULL){
        printf("ERROR: malloc error @ pointerlist.\n");
        exit(-1);
    }
    
    for(i=0;i<pointer_lists;i++){
        if(fscanf(fp,"%d",&pointer_list[i]) != 1){
            errorCall(1);
        }
    }
    
    //Read PI list
    if(fscanf(fp,"%d",&PI_lists) != 1){
        errorCall(1);
    }
    
    PI_list = malloc(PI_lists*sizeof(unsigned int));
    
    if(PI_list == NULL){
        printf("ERROR: malloc error @ PIlist.\n");
        exit(-1);
    }
    
    for(i=0;i<PI_lists;i++){
        if(fscanf(fp,"%d",&PI_list[i]) != 1){
            errorCall(1);
        }
    }
    
    //Read PO list
    if(fscanf(fp,"%d",&PO_lists) != 1){
        errorCall(1);
    }
    
    PO_list = malloc(PO_lists*sizeof(unsigned int));
    
    if(PO_list == NULL){
        printf("ERROR: malloc error @ POlist.\n");
        exit(-1);
    }
    
    for(i=0;i<PO_lists;i++){
        if(fscanf(fp,"%d",&PO_list[i]) != 1){
            errorCall(1);
        }
    }
    
    fclose(fp);
    
    //read test pattern
    strcpy(checkdirtype,argv[1]);
    if(checkdirtype[0] == 'e'){
        strcpy(filename,"./iscas85/Pattern/");
    }else if(checkdirtype[0] == 'c'){
        strcpy(filename,"./iscas89_cs/Pattern/");
    }
    strcat(filename,argv[1]);
    strcat(filename,".pat");
    
    if((fp = fopen(filename,"r")) == NULL){
        errorCall(2);
    }
    
    if(fscanf(fp,"%d",&test_ptns) != 1){
        errorCall(2);
    }
    
    if((test_ptn = malloc(test_ptns*sizeof(unsigned char *)))==NULL){
        printf("ERROR: malloc error @ testptn\n");
        exit(-1);
    }
    for(i=0;i<test_ptns;i++){
        if((test_ptn[i] = malloc(PI_lists*sizeof(unsigned char))) == NULL){
            printf("ERROR: malloc error @ testptn\n");
            exit(-1);
        }
    }
    
    for(i=0;i<test_ptns;i++){
        for(j=0;j<PI_lists;j++){
            if(fscanf(fp,"%d",&input_rd) != 1){
                printf("ERROR: pat file cannot be read.\n");
                exit(-1);
            }else{
                test_ptn[i][j] = (unsigned char)input_rd;
            }
        }
    }
    
    fclose(fp);
    
    //initialize upon saving the order of calculation
    cal_no = malloc((signal_lines+1)*sizeof(int));
    
    if(cal_no == NULL){
        printf("ERROR: malloc error @ calno\n");
        exit(-1);
    }
    for(i=0;i<=signal_lines;i++){
        cal_no[i] = -1;
    }
    
    //save fault array list
    if((fault_list = malloc(signal_lines*sizeof(unsigned char *)))==NULL){
        printf("ERROR: malloc error @ faultlist\n");
        exit(-1);
    }
    for(i=0;i<signal_lines;i++){
        if((fault_list[i] = malloc(signal_lines*sizeof(unsigned char))) == NULL){
            printf("ERROR: malloc error @ faultlist\n");
            exit(-1);
        }
    }
    if((sum_fault = malloc(signal_lines*sizeof(unsigned char)))==NULL){
        printf("ERROR: malloc error @ faultlist\n");
        exit(-1);
    }
    memset(sum_fault,0b00,sizeof(unsigned char)*signal_lines); //initialize @ 0
    
    //order set
    //output set to unknown
    for(i=0;i<signal_lines;i++){
        signal_line[i][5] = -1;
    }
    
    now_no =0;
    //hypothesize all test data
    for(i=0;i<PI_lists;i++){
        if(signal_line[(PI_list[i]-1)][0] != 0){
            printf("ERROR:  error @ PI\n");
            exit(-1);
        }else{
            signal_line[(PI_list[i]-1)][5] = 1;
            cal_no[now_no] = PI_list[i];
            now_no++;
        }
    }
    
    //calculate first signal line
    for(i=1;i<=signal_lines;i++){
        if(signal_line[(i-1)][5] == -1){ //skip if signal line is set
            if(logicsim(signal_line,i,pointer_list) == -1){
                if(enqueue(&que,i) == -1){
                    printf("ERROR: error @ queuelist\n");
                    exit(-1);
                }
            }else{ //remember order if can be calculated
                cal_no[now_no] = i;
                now_no++;
            }
        }
    }
    
    //re-calc while take out queue
    while((i = dequeue(&que)) != -1){
        if(logicsim(signal_line,i,pointer_list) == -1){
            if(enqueue(&que,i) == -1){
                printf("ERROR: queue is full\n");
                exit(-1);
            }
        }else{ //remember order if can be calculated
            cal_no[now_no] = i;
            now_no++;
        }
    }
    //extraction of output
    for(ptn_i=0;ptn_i<test_ptns;ptn_i++){
        //output signal & fault list to unknown
        for(i=0;i<signal_lines;i++){
            signal_line[i][5] = -1;
            memset(fault_list[i],0b00,sizeof(unsigned char)*signal_lines); //fault list all to 0
        }
        
        //set input for test pattern
        for(i=0;i<PI_lists;i++){
            if(signal_line[(PI_list[i]-1)][0] != 0){
                printf("ERROR:  error @ PI\n");
                exit(-1);
            }else{
                signal_line[(PI_list[i]-1)][5] = test_ptn[ptn_i][i];
            }
        }
        
        i = 0;
        while(cal_no[i] != -1){
            logicsim(signal_line,cal_no[i],pointer_list);
            faultsim(signal_line,fault_list,cal_no[i],pointer_list,signal_lines,cal_no,i);
            i++;
        }
        
        //output is appended to sum_fault by OR
        for(i=0;i<PO_lists;i++){
            for(j=0;j<signal_lines;j++){
                sum_fault[j] = sum_fault[j] | fault_list[(signal_line[(PO_list[i]-1)][4])-1][j];
            }
        }
    }
    
    //read faultset
    strcpy(checkdirtype,argv[1]);
    if(checkdirtype[0] == 'e'){
        strcpy(filename,"./iscas85/Faultset/");
    }else if(checkdirtype[0] == 'c'){
        strcpy(filename,"./iscas89_cs/Faultset/");
    }
    strcat(filename,argv[1]);
    strcat(filename,"f.rep");
    
    if((fp = fopen(filename,"r")) == NULL){
        errorCall(3);
    }
    
    //read faultset
    if(fscanf(fp,"%d",&faultcounters) != 1){
        errorCall(3);
    }
    
    if((all_fault = malloc(signal_lines*sizeof(unsigned char)))==NULL){
        printf("ERROR: malloc error @ allfault\n");
        exit(-1);
    }
    memset(all_fault,0b00,sizeof(unsigned char)*signal_lines);
    
    for(i=0;i<faultcounters;i++){
        if(fscanf(fp,"%d",&input_rd) != 1){
            errorCall(3);
        }
        if(fscanf(fp,"%d",&input_rd2) != 1){
            errorCall(3);
        }
        if(input_rd2 == 0){
            all_fault[input_rd-1] |= 0b10;
        }else if(input_rd2 == 1){
            all_fault[input_rd-1] |= 0b01;
        }else{
            errorCall(3);
        }
    }
    
    //calc total fault found
    faults_det = 0;
    
    for(i=0;i<signal_lines;i++){
        switch(all_fault[i]&sum_fault[i]){
            case 0b01:
                faults_det ++;
                break;
            case 0b10:
                faults_det ++;
                break;
            case 0b11:
                faults_det += 2;
                break;
            case 0b00:
                break;
            default:
                printf("ERROR: error @ fault result\n");
                exit(-1);
                break;
        }
    }
    
    printf("Total Fault：%d \nTotal Fault Detected：%d　\nFault detection rate：%.2f　",faultcounters,faults_det,(double)faults_det*100/faultcounters);
    
    printf("\nTime taken：%.2f\n",(double)clock()/CLOCKS_PER_SEC);
    
    free(signal_line);
    free(pointer_list);
    free(PI_list);
    free(PO_list);
    for(i=0;i<test_ptns;i++){
        free(test_ptn[i]);
    }
    free(test_ptn);
    for(i=0;i<signal_lines;i++){
        free(fault_list[i]);
    }
    free(fault_list);
    free(sum_fault);
    free(all_fault);
    
    return 0;
}


void errorCall(int errorN){
    if (errorN == 1){
        printf("ERROR: tbl file cannot be read.\n");
        exit(-1);
    }
    else if(errorN == 2){
        printf("ERROR: pat file cannot be read.\n");
        exit(-1);
    }
    else if(errorN == 3){
        printf("ERROR: faultset file cannot be read.\n");
        exit(-1);
    }
}
