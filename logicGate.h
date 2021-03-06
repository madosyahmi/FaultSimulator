char logicsim(int (*signal_line)[6],int line_no,unsigned int *pointer_list){
    //Calculate signal output, return -1 if PI is unknown
    int res;
    int i;
    int pointer_head;
    
    switch(signal_line[line_no-1][0]){
        case 0: //PI
            //nothing do
            return (char)signal_line[line_no-1][5];
            break;
        case 1: //or
            if(signal_line[line_no-1][1] <= 1){ //No.of.PI
                signal_line[line_no-1][5] = signal_line[(signal_line[line_no-1][2])-1][5];
                return (char)signal_line[line_no-1][5];
            }else{
                res = 0;
                pointer_head = signal_line[line_no-1][2];
                for(i=0;i<signal_line[line_no-1][1];i++){
                    if(signal_line[(pointer_list[(pointer_head-1+i)])-1][5] == -1){
                        signal_line[line_no-1][5] = -1;
                        return -1;
                    }else{
                        res += signal_line[(pointer_list[(pointer_head-1+i)])-1][5];
                    }
                }
                if(res != 0){
                    res = 1;
                }
                signal_line[line_no-1][5] = res;
                return (char)res;
            }
            break;
        case 2: // AND
            if(signal_line[line_no-1][1] <= 1){ //No.of.PI
                signal_line[line_no-1][5] = signal_line[(signal_line[line_no-1][2])-1][5];
                return (char)signal_line[line_no-1][5];
            }else{
                res = 1;
                pointer_head = signal_line[line_no-1][2];
                for(i=0;i<signal_line[line_no-1][1];i++){
                    if(signal_line[(pointer_list[(pointer_head-1+i)])-1][5] == -1){
                        signal_line[line_no-1][5] = -1;
                        return -1;
                    }else{
                        res *= signal_line[(pointer_list[(pointer_head-1+i)])-1][5];
                    }
                }
                if(res != 0){
                    res = 1;
                }
                signal_line[line_no-1][5] = res;
                return (char)res;
            }
            break;
        case 3: //Split
            if(signal_line[line_no-1][1] > 1){ //No.of.PI
                printf("ERROR : logicsim @ SPLIT (Line %dth).\n",line_no);
                exit(-1);
            }else{
                signal_line[line_no-1][5] = signal_line[(signal_line[line_no-1][2])-1][5];
                return (char)signal_line[line_no-1][5];
            }
            break;
        case 4: //PO
            if(signal_line[line_no-1][1] > 1){ //No.of.PI
                printf("ERROR : logicsim @ PO (Line %dth).\n",line_no);
                exit(-1);
            }else{
                signal_line[line_no-1][5] = signal_line[(signal_line[line_no-1][2])-1][5];
                return (char)signal_line[line_no-1][5];
            }
            break;
        case 5: //EXOR
            if(signal_line[line_no-1][1] <= 1){ //No.of.PI
                printf("ERROR : logicsim @ EXOR (Line %dth).\n",line_no);
                exit(-1);
            }else{
                res = 0;
                pointer_head = signal_line[line_no-1][2];
                for(i=0;i<signal_line[line_no-1][1];i++){
                    if(signal_line[(pointer_list[(pointer_head-1+i)])-1][5] == -1){
                        signal_line[line_no-1][5] = -1;
                        return -1;
                    }else{
                        if(signal_line[(pointer_list[(pointer_head-1+i)])-1][5] == 1){
                            if(res == 0){
                                res = 1;
                            }else{
                                res = 0;
                            }
                        } //Do nothing if PI is 0
                    }
                }
                signal_line[line_no-1][5] = res;
                return (char)res;
            }
            break;
        case -1: //NAND
            if(signal_line[line_no-1][1] <= 1){ //No.of.PI
                switch(signal_line[(signal_line[line_no-1][2])-1][5]){
                    case -1:
                        res = -1;
                        break;
                    case 0:
                        res = 1;
                        break;
                    case 1:
                        res = 0;
                        break;
                    default:
                        res = -1;
                        printf("ERROR : NAND is undefined at signal %d (Line %dth).\n",signal_line[(signal_line[line_no-1][2])-1][5],line_no);
                        exit(-1);
                        break;
                }
                signal_line[line_no-1][5] = res;
                return (char)signal_line[line_no-1][5];
            }else{
                res = 1;
                pointer_head = signal_line[line_no-1][2];
                for(i=0;i<signal_line[line_no-1][1];i++){
                    if(signal_line[(pointer_list[(pointer_head-1+i)])-1][5] == -1){
                        signal_line[line_no-1][5] = -1;
                        return -1;
                    }else{
                        res *= signal_line[(pointer_list[(pointer_head-1+i)])-1][5];
                    }
                }
                if(res != 0){
                    res = 0;
                }else{
                    res = 1;
                }
                signal_line[line_no-1][5] = res;
                return (char)res;
            }
            break;
        case -2: //NOR
            if(signal_line[line_no-1][1] <= 1){ //No.of.PI
                switch(signal_line[(signal_line[line_no-1][2])-1][5]){
                    case -1:
                        res = -1;
                        break;
                    case 0:
                        res = 1;
                        break;
                    case 1:
                        res = 0;
                        break;
                    default:
                        res = -1;
                        printf("ERROR : NOR is undefined at signal %d (Line %dth).\n",signal_line[(signal_line[line_no-1][2])-1][5],line_no);
                        exit(-1);
                        break;
                }
                signal_line[line_no-1][5] = res;
                return (char)signal_line[line_no-1][5];
            }else{
                res = 0;
                pointer_head = signal_line[line_no-1][2];
                for(i=0;i<signal_line[line_no-1][1];i++){
                    if(signal_line[(pointer_list[(pointer_head-1+i)])-1][5] == -1){
                        signal_line[line_no-1][5] = -1;
                        return -1;
                    }else{
                        res += signal_line[(pointer_list[(pointer_head-1+i)])-1][5];
                    }
                }
                if(res != 0){
                    res = 0;
                }else{
                    res = 1;
                }
                signal_line[line_no-1][5] = res;
                return (char)res;
            }
            break;
        case -3: //NOT
            if(signal_line[line_no-1][1] > 1){ //No.of.PI
                printf("ERROR : logicsim @ NOT (Line %dth).\n",line_no);
                exit(-1);
            }else{
                switch(signal_line[(signal_line[line_no-1][2])-1][5]){
                    case -1:
                        res = -1;
                        break;
                    case 0:
                        res = 1;
                        break;
                    case 1:
                        res = 0;
                        break;
                    default:
                        res = -1;
                        printf("ERROR : NOT is undefined at signal %d (Line %dth).\n",signal_line[(signal_line[line_no-1][2])-1][5],line_no);
                        exit(-1);
                        break;
                }
                signal_line[line_no-1][5] = res;
                return (char)signal_line[line_no-1][5];
            }
            break;
        default:
            printf("ERROR : logicsim erorr (Line %dth).\n",line_no);
            exit(-1);
            break;
    }
}
