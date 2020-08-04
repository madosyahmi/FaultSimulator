void faultsim(int (*signal_line)[6],unsigned char **fault_list,int line_no,unsigned int *pointer_list,unsigned int signal_lines,int *cal_no,int now_cal_i){
    //信号線の故障リストを計算
    int res;
    int i,j;
    int pointer_head;
    
    switch(signal_line[line_no-1][0]){
        case 0: //外部入力
            break;
        case 1: //or
            if(signal_line[line_no-1][1] <= 1){ //入力数
                memcpy(fault_list[line_no-1],fault_list[signal_line[line_no-1][2]-1],sizeof(unsigned char)*signal_lines);
            }else{
                pointer_head = signal_line[line_no-1][2];
                if(signal_line[line_no-1][5] ==0){ //正常出力0
                    for(i=0;i<signal_line[line_no-1][1];i++){
                        if(signal_line[(pointer_list[(pointer_head-1+i)])-1][5] == 0){
                            for(j=0;j<now_cal_i;j++){
                                fault_list[line_no-1][cal_no[j]-1] |= fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1];
                            }
                        }
                    }
                }else{ //出力１
                    for(i=0;i<now_cal_i;i++){
                        //AND計算に備えてすべてを１へ
                        fault_list[line_no-1][cal_no[i]-1] = ~fault_list[line_no-1][cal_no[i]-1];
                    }
                    for(i=0;i<signal_line[line_no-1][1];i++){
                        if(signal_line[(pointer_list[(pointer_head-1+i)])-1][5] == 1){
                            for(j=0;j<now_cal_i;j++){
                                fault_list[line_no-1][cal_no[j]-1] &= fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1];
                            }
                        }else{ //入力が0のとき
                            for(j=0;j<now_cal_i;j++){
                                fault_list[line_no-1][cal_no[j]-1] &= (~fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1]) <<6 >>6;
                            }
                        }
                    }
                }
            }
            break;
        case 2: // AND
            if(signal_line[line_no-1][1] <= 1){ //入力数
                memcpy(fault_list[line_no-1],fault_list[signal_line[line_no-1][2]-1],sizeof(unsigned char)*signal_lines);
            }else{
                pointer_head = signal_line[line_no-1][2];
                if(signal_line[line_no-1][5] ==1){ //正常出力1
                    for(i=0;i<signal_line[line_no-1][1];i++){
                        if(signal_line[(pointer_list[(pointer_head-1+i)])-1][5] == 1){
                            for(j=0;j<now_cal_i;j++){
                                fault_list[line_no-1][cal_no[j]-1] |= fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1];
                            }
                        }
                    }
                }else{ //出力0
                    for(i=0;i<now_cal_i;i++){
                        //AND計算に備えてすべてを１へ
                        fault_list[line_no-1][cal_no[i]-1] = ~fault_list[line_no-1][cal_no[i]-1];
                    }
                    for(i=0;i<signal_line[line_no-1][1];i++){
                        if(signal_line[(pointer_list[(pointer_head-1+i)])-1][5] == 0){
                            for(j=0;j<now_cal_i;j++){
                                fault_list[line_no-1][cal_no[j]-1] &= fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1];
                            }
                        }else{ //入力が1のとき
                            for(j=0;j<now_cal_i;j++){
                                fault_list[line_no-1][cal_no[j]-1] &= (~fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1]) <<6 >>6;
                            }
                        }
                    }
                }
            }
            break;
        case 3: //分岐の枝
            memcpy(fault_list[line_no-1],fault_list[signal_line[line_no-1][2]-1],sizeof(unsigned char)*signal_lines);
            break;
        case 4: //外部出力
            memcpy(fault_list[line_no-1],fault_list[signal_line[line_no-1][2]-1],sizeof(unsigned char)*signal_lines);
            break;
        case 5: //EXOR
            //排他的論理和で入力の故障リストを合成
            if(signal_line[line_no-1][1] <= 1){ //入力数
                printf("Logic cal EXOR Error(%dth Line).\n",line_no);
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
            if(signal_line[line_no-1][1] <= 1){ //入力数
                memcpy(fault_list[line_no-1],fault_list[signal_line[line_no-1][2]-1],sizeof(unsigned char)*signal_lines);
            }else{
                pointer_head = signal_line[line_no-1][2];
                if(signal_line[line_no-1][5] ==0){ //正常出力0
                    for(i=0;i<signal_line[line_no-1][1];i++){
                        if(signal_line[(pointer_list[(pointer_head-1+i)])-1][5] == 1){
                            for(j=0;j<now_cal_i;j++){
                                fault_list[line_no-1][cal_no[j]-1] |= fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1];
                            }
                        }
                    }
                }else{ //出力1
                    for(i=0;i<now_cal_i;i++){
                        //AND計算に備えてすべてを１へ
                        fault_list[line_no-1][cal_no[i]-1] = ~fault_list[line_no-1][cal_no[i]-1];
                    }
                    for(i=0;i<signal_line[line_no-1][1];i++){
                        if(signal_line[(pointer_list[(pointer_head-1+i)])-1][5] == 0){
                            for(j=0;j<now_cal_i;j++){
                                fault_list[line_no-1][cal_no[j]-1] &= fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1];
                            }
                        }else{ //入力が1のとき
                            for(j=0;j<now_cal_i;j++){
                                fault_list[line_no-1][cal_no[j]-1] &= (~fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1]) <<6 >>6;
                            }
                        }
                    }
                }
            }
            break;
        case -2: //NOR
            if(signal_line[line_no-1][1] <= 1){ //入力数
                memcpy(fault_list[line_no-1],fault_list[signal_line[line_no-1][2]-1],sizeof(unsigned char)*signal_lines);
            }else{
                pointer_head = signal_line[line_no-1][2];
                if(signal_line[line_no-1][5] ==1){ //正常出力1
                    for(i=0;i<signal_line[line_no-1][1];i++){
                        if(signal_line[(pointer_list[(pointer_head-1+i)])-1][5] == 0){
                            for(j=0;j<now_cal_i;j++){
                                fault_list[line_no-1][cal_no[j]-1] |= fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1];
                            }
                        }
                    }
                }else{ //出力0
                    for(i=0;i<now_cal_i;i++){
                        //AND計算に備えてすべてを１へ
                        fault_list[line_no-1][cal_no[i]-1] = ~fault_list[line_no-1][cal_no[i]-1];
                    }
                    for(i=0;i<signal_line[line_no-1][1];i++){
                        if(signal_line[(pointer_list[(pointer_head-1+i)])-1][5] == 1){
                            for(j=0;j<now_cal_i;j++){
                                fault_list[line_no-1][cal_no[j]-1] &= fault_list[(pointer_list[(pointer_head-1+i)])-1][cal_no[j]-1];
                            }
                        }else{ //入力が0のとき
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
            printf("Logic cal Error(%dth Line).\n",line_no);
            exit(-1);
            break;
    }
    
    //最後に自分の縮退故障を追加する
    if(signal_line[line_no-1][5] == 0){
        fault_list[line_no-1][line_no-1] = 0b01; //1縮退
    }else if(signal_line[line_no-1][5] == 1){
        fault_list[line_no-1][line_no-1] = 0b10; //0縮退
    }else{
        printf("fault cal Error.\n");
        exit(-1);
    }
    
    return;
}
