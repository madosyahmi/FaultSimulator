void faultsim(int (*signal_line)[6],unsigned char **fault_list,int line_no,unsigned int *pointer_list,unsigned int signal_lines,int *cal_no,int now_cal_i){
    //Calculate fault list signals
    int res;
    int i,j;
    int pointer_head;
    
    switch(signal_line[line_no-1][0]){
        case 0: //PI
            break;
        case 1: //OR
            if(signal_line[line_no-1][1] <= 1){ //Number of PI
                memcpy(fault_list[line_no-1],fault_list[signal_line[line_no-1][2]-1],sizeof(unsigned char)*signal_lines);
            }else{
                pointer_head = signal_line[line_no-1][2];
                if(signal_line[line_no-1][5] ==0){
                    for(i=0;i<signal_line[line_no-1][1];i++){
                        if(signal_line[(pointer_list[(pointer_head-1+i)])-1][5] == 0){
                            for(j=0;j<now_cal_i;j++){
                                fault_list[line_no-1][cal_no[j]-1] |= fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1];
                            }
                        }
                    }
                }else{ //PO
                    for(i=0;i<now_cal_i;i++){
                        //AND - all 1
                        fault_list[line_no-1][cal_no[i]-1] = ~fault_list[line_no-1][cal_no[i]-1];
                    }
                    for(i=0;i<signal_line[line_no-1][1];i++){
                        if(signal_line[(pointer_list[(pointer_head-1+i)])-1][5] == 1){
                            for(j=0;j<now_cal_i;j++){
                                fault_list[line_no-1][cal_no[j]-1] &= fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1];
                            }
                        }else{ //if PI = 0
                            for(j=0;j<now_cal_i;j++){
                                fault_list[line_no-1][cal_no[j]-1] &= (~fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1]) <<6 >>6;
                            }
                        }
                    }
                }
            }
            break;
        case 2: // AND
            if(signal_line[line_no-1][1] <= 1){
                memcpy(fault_list[line_no-1],fault_list[signal_line[line_no-1][2]-1],sizeof(unsigned char)*signal_lines);
            }else{
                pointer_head = signal_line[line_no-1][2];
                if(signal_line[line_no-1][5] ==1){
                    for(i=0;i<signal_line[line_no-1][1];i++){
                        if(signal_line[(pointer_list[(pointer_head-1+i)])-1][5] == 1){
                            for(j=0;j<now_cal_i;j++){
                                fault_list[line_no-1][cal_no[j]-1] |= fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1];
                            }
                        }
                    }
                }else{
                    for(i=0;i<now_cal_i;i++){
                        ///AND - all 1
                        fault_list[line_no-1][cal_no[i]-1] = ~fault_list[line_no-1][cal_no[i]-1];
                    }
                    for(i=0;i<signal_line[line_no-1][1];i++){
                        if(signal_line[(pointer_list[(pointer_head-1+i)])-1][5] == 0){
                            for(j=0;j<now_cal_i;j++){
                                fault_list[line_no-1][cal_no[j]-1] &= fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1];
                            }
                        }else{ //if PI = 1
                            for(j=0;j<now_cal_i;j++){
                                fault_list[line_no-1][cal_no[j]-1] &= (~fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1]) <<6 >>6;
                            }
                        }
                    }
                }
            }
            break;
        case 3: //SPLIT
            memcpy(fault_list[line_no-1],fault_list[signal_line[line_no-1][2]-1],sizeof(unsigned char)*signal_lines);
            break;
        case 4: //PO
            memcpy(fault_list[line_no-1],fault_list[signal_line[line_no-1][2]-1],sizeof(unsigned char)*signal_lines);
            break;
        case 5: //EXOR
            if(signal_line[line_no-1][1] <= 1){
                printf("ERROR : logicsim @ EXOR (Line %dth).\n",line_no);
                exit(-1);
            }else{
                res = 0;
                pointer_head = signal_line[line_no-1][2];
                memcpy(fault_list[line_no-1],fault_list[(pointer_list[(pointer_head-1)])-1],sizeof(unsigned char)*signal_lines);
                for(i=1;i<signal_line[line_no-1][1];i++){
                    for(j=0;j<now_cal_i;j++){
                        fault_list[line_no-1][cal_no[j]-1] ^= fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1];
                    }
                }
            }
            break;
        case -1: //NAND
            if(signal_line[line_no-1][1] <= 1){
                memcpy(fault_list[line_no-1],fault_list[signal_line[line_no-1][2]-1],sizeof(unsigned char)*signal_lines);
            }else{
                pointer_head = signal_line[line_no-1][2];
                if(signal_line[line_no-1][5] ==0){
                    for(i=0;i<signal_line[line_no-1][1];i++){
                        if(signal_line[(pointer_list[(pointer_head-1+i)])-1][5] == 1){
                            for(j=0;j<now_cal_i;j++){
                                fault_list[line_no-1][cal_no[j]-1] |= fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1];
                            }
                        }
                    }
                }else{ 
                    for(i=0;i<now_cal_i;i++){
                        //AND - all 1
                        fault_list[line_no-1][cal_no[i]-1] = ~fault_list[line_no-1][cal_no[i]-1];
                    }
                    for(i=0;i<signal_line[line_no-1][1];i++){
                        if(signal_line[(pointer_list[(pointer_head-1+i)])-1][5] == 0){
                            for(j=0;j<now_cal_i;j++){
                                fault_list[line_no-1][cal_no[j]-1] &= fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1];
                            }
                        }else{ //if PI=1
                            for(j=0;j<now_cal_i;j++){
                                fault_list[line_no-1][cal_no[j]-1] &= (~fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1]) <<6 >>6;
                            }
                        }
                    }
                }
            }
            break;
        case -2: //NOR
            if(signal_line[line_no-1][1] <= 1){
                memcpy(fault_list[line_no-1],fault_list[signal_line[line_no-1][2]-1],sizeof(unsigned char)*signal_lines);
            }else{
                pointer_head = signal_line[line_no-1][2];
                if(signal_line[line_no-1][5] ==1){
                    for(i=0;i<signal_line[line_no-1][1];i++){
                        if(signal_line[(pointer_list[(pointer_head-1+i)])-1][5] == 0){
                            for(j=0;j<now_cal_i;j++){
                                fault_list[line_no-1][cal_no[j]-1] |= fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1];
                            }
                        }
                    }
                }else{
                    for(i=0;i<now_cal_i;i++){
                        ////AND - all 1
                        fault_list[line_no-1][cal_no[i]-1] = ~fault_list[line_no-1][cal_no[i]-1];
                    }
                    for(i=0;i<signal_line[line_no-1][1];i++){
                        if(signal_line[(pointer_list[(pointer_head-1+i)])-1][5] == 1){
                            for(j=0;j<now_cal_i;j++){
                                fault_list[line_no-1][cal_no[j]-1] &= fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1];
                            }
                        }else{ //if PI = 0
                            for(j=0;j<now_cal_i;j++){
                                fault_list[line_no-1][cal_no[j]-1] &= (~fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1]) <<6 >>6;
                            }
                        }
                    }
                }
            }
            break;
        case -3: //NOT
            memcpy(fault_list[line_no-1],fault_list[signal_line[line_no-1][2]-1],sizeof(unsigned char)*signal_lines);
            break;
        default:
            printf("ERROR : logicsim (Line %dth).",line_no);
            exit(-1);
            break;
    }
    
    if(signal_line[line_no-1][5] == 0){
        fault_list[line_no-1][line_no-1] = 0b01; //SA1
    }else if(signal_line[line_no-1][5] == 1){
        fault_list[line_no-1][line_no-1] = 0b10; //SA0
    }else{
        printf("ERROR : faultsim\n");
        exit(-1);
    }
    
    return;
}
