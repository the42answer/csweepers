#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
//4D Minesweeper? ...
//Make it so!
int *board = 0;
int *mask  = 0;

int w_size = 0;
int x_size = 0;
int y_size = 0;
int z_size = 0;

int mines  = 0;
/*Sample board

#========= 01 ============= 02 ========#
|      00 01 02 03     00 01 02 03     |
|  04  #  #  #  #  04  #  #  #  #  04  |
|  03  #  #  #  #  03  #  #  #  #  03  |
02 02  #  #  2  #  02  #  #  #  #  02 02
|  01  #  #  #  #  01  #  #  #  #  01  |
|  00  #  #  #  #  00  #  #  #  #  00  |
|      00 01 02 03     00 01 02 03     |
|  04  #  #  #  #  04  #  #  #  #  04  |
|  03  #  1  1  #  03  #  #  #  #  03  |
01 02  1        1  02  #  #  1  #  02 01
|  01  #  1  1  #  01  #  #  #  #  01  |
|  00  #  #  #  #  00  #  #  #  #  00  |
|      00 01 02 03     00 01 02 03     |
#========= 01 ============= 02 ========#

*/
int *getboundedint(int lower, int upper){
    int ret;
    do{
        scanf("%d", &ret);
    }while( !(lower <= ret && ret <= upper));
    return ret;
}

int main(void){

    int *getboardpointer(int *someboard, int w, int x, int y, int z){
        if (0 <= w && w < w_size && 0 <= x && x < x_size && 0 <= y && y < y_size && 0 <= z && z < z_size){
            return (someboard + ((sizeof(int)) * ( w + x*w_size + y*w_size*x_size + z*w_size*x_size*y_size) ));
        } else {
            return 0;
        }
    }

    int isproximity(int w, int x, int y, int z){
        if (getboardpointer(board, w, x, y, z) != 0){
            if (*getboardpointer(board, w, x, y, z) == 9){
                return 1;
            }
        }
        return 0;
    }

    int proximity(int w, int x, int y, int z){
        int ret = 0;
        ret += isproximity(w-1, x, y, z);
        ret += isproximity(w+1, x, y, z);
        ret += isproximity(w, x-1, y, z);
        ret += isproximity(w, x+1, y, z);
        ret += isproximity(w, x, y-1, z);
        ret += isproximity(w, x, y+1, z);
        ret += isproximity(w, x, y, z-1);
        ret += isproximity(w, x, y, z+1);
        return ret;
    }

    int isvisadj(int w, int x, int y, int z){
        if (getboardpointer(board, w, x, y, z) != 0){
            if (*getboardpointer(board, w, x, y, z) == 0 && *getboardpointer(mask, w, x, y, z) == 1){
                return 1;
            }
        }
        return 0;
    }

    int visadj (int w, int x, int y, int z){
        if (
            isvisadj(w-1, x, y, z) ||
            isvisadj(w+1, x, y, z) ||
            isvisadj(w, x-1, y, z) ||
            isvisadj(w, x+1, y, z) ||
            isvisadj(w, x, y-1, z) ||
            isvisadj(w, x, y+1, z) ||
            isvisadj(w, x, y, z-1) ||
            isvisadj(w, x, y, z+1)
        ){
            return 1;
        }
        return 0;
    }

    char printsquare(int w, int x, int y, int z){
        char ret;
        if (*getboardpointer(mask, w, x, y, z) == 1){
            if (*getboardpointer(board, w, x, y, z) == 0){
                sprintf(&ret, " ");
            }
            else{
                sprintf(&ret, "%d", *getboardpointer(board, w, x, y, z));
            }
        }
        else{
            sprintf(&ret, "#");
        }
        return ret;
    }

    char revealsquare(int w, int x, int y, int z){
        char ret;
        if (*getboardpointer(mask, w, x, y, z) == 9){
            sprintf(&ret, "@");
        }
        else{
            sprintf(&ret, "%d", *getboardpointer(board, w, x, y, z));
        }
        return ret;
    }

    void printborderrow(void){
        printf("#===");
        for(int w_loop = 0; w_loop < w_size; w_loop++){
            for(int y_loop = 0; y_loop < (3*y_size) /2 +y_size%2; y_loop++){
                printf("=");
            }
            printf(" %02d ", w_loop);
            for(int y_loop = 0; y_loop < (3*y_size) /2; y_loop++){
                printf("=");
            }
        }
        printf("===#\n");
    }

    void printnumberrow(void){
        printf("|      ");
        for(int w_loop = 0; w_loop < w_size; w_loop++){
            for(int y_loop = 0; y_loop < y_size; y_loop++){
                printf("%02d ", y_loop);
            }
            printf("    ");
        }
        printf("|\n");
    }

    void printuserboard(void){
        printborderrow();
        for (int x_loop = x_size-1; x_loop >= 0; x_loop--){
            printnumberrow();
            for (int z_loop = z_size-1; z_loop >= 0; z_loop--){
                if(z_loop == z_size/2){
                    printf("%02d ", x_loop);
                }else{
                    printf("|  ");
                }
                for (int w_loop = 0; w_loop < w_size; w_loop++){
                    printf("%02d", z_loop);
                    printf("  ");
                    for (int y_loop = 0; y_loop < y_size; y_loop++){
                        printf("%c  ", printsquare(w_loop, x_loop, y_loop, z_loop));
                    }
                }
                printf("%02d", z_loop);
                if(z_loop == z_size/2){
                    printf(" %02d\n", x_loop);
                }else{
                    printf("  |\n");
                }
            }
        }
        printnumberrow();
        printborderrow();
    }

    void sweep(void){
        int isdone;
        do{
            isdone = 1;
            for (int w_loop = 0; w_loop < w_size; w_loop++){
                for (int x_loop = 0; x_loop < x_size; x_loop++){
                    for (int y_loop = 0; y_loop < y_size; y_loop++){
                        for (int z_loop = 0; z_loop < z_size; z_loop++){
                            if (visadj(w_loop, x_loop, y_loop, z_loop) && *getboardpointer(mask, w_loop, x_loop, y_loop, z_loop) == 0){
                                isdone = 0;
                                *getboardpointer(mask, w_loop, x_loop, y_loop, z_loop) = 1;
                            }
                        }
                    }
                }
            }
        }while (isdone == 0);
    }

    int toremove(void){
        int count = 0;
        for (int w_loop = 0; w_loop < w_size; w_loop++){
            for (int x_loop = 0; x_loop < x_size; x_loop++){
                for (int y_loop = 0; y_loop < y_size; y_loop++){
                    for (int z_loop = 0; z_loop < z_size; z_loop++){
                        if (*getboardpointer(mask, w_loop, x_loop, y_loop, z_loop) == 0){
                            count++;
                        }
                    }
                }
            }
        }
        return count - mines;
    }

    //===========================================================================================================

    start:
    printf ("Enter the W size of the board\n[1-20]: ");
    w_size = getboundedint(1, 20);
    printf ("Enter the X size of the board\n[1-20]: ");
    x_size = getboundedint(1, 20);
    printf ("Enter the Y size of the board\n[1-20]: ");
    y_size = getboundedint(1, 20);
    printf ("Enter the Z size of the board\n[1-20]: ");
    z_size = getboundedint(1, 20);
    printf ("Enter the number of mines\n[0-%d] :", w_size * x_size * y_size * z_size -1);
    mines = getboundedint(0, w_size * x_size * y_size * z_size -1);

    board = calloc(sizeof(int), w_size * x_size * y_size * z_size * 20);
    if (board == 0){
        printf("Insufficient memory to hold the board.\nPlease enter a smaller board size and upgrade your computer.\n");
        goto start; //If the pointer is NULL, then there is nothing to free.
    }
    mask = (board + w_size * x_size * y_size * z_size * sizeof(int));
    *getboardpointer(mask, w_size-1, x_size-1, y_size-1, z_size-1) = 0;//Immediately throws the segfault if the allocation is bad.

    printf("Pass1====================================\n");

    srand(time(0));
    for (int loop = 0; loop < mines; ){
        int mine_w = rand() % w_size;
        int mine_x = rand() % x_size;
        int mine_y = rand() % y_size;
        int mine_z = rand() % z_size;
        int *boardpointer = getboardpointer(board, mine_w, mine_x, mine_y, mine_z);
        if (*boardpointer == 0){
            *boardpointer = 9;
            loop++;
        }
    }

    printf("Pass2====================================\n");

    for (int w_loop = 0; w_loop < w_size; w_loop++){
        for(int x_loop = 0; x_loop < x_size; x_loop++){
            for (int y_loop = 0; y_loop < y_size; y_loop++){
                for (int z_loop = 0; z_loop < z_size; z_loop++){
                    int *boardpointer = getboardpointer(board, w_loop, x_loop, y_loop, z_loop);
                    if (*boardpointer == 0){
                        *getboardpointer(board, w_loop, x_loop, y_loop, z_loop) = proximity(w_loop, x_loop, y_loop, z_loop);
                    }
                }
            }
        }
    }

    printf("Pass3====================================\n");

    printuserboard();

    while(1){
        int w_move;
        int x_move;
        int y_move;
        int z_move;
        printf("Enter W position\n[0-%d]: ", w_size - 1);
        w_move = getboundedint(0, w_size - 1);
        printf("Enter X position\n[0-%d]: ", x_size - 1);
        x_move = getboundedint(0, x_size - 1);
        printf("Enter Y position\n[0-%d]: ", y_size - 1);
        y_move = getboundedint(0, y_size - 1);
        printf("Enter Z position\n[0-%d]: ", z_size - 1);
        z_move = getboundedint(0, z_size - 1);
        if (*getboardpointer(mask, w_move, x_move, y_move, z_move) != 0){
            printf("You have already revealed this square!\n");
            continue;
        }
        if (*getboardpointer(board, w_move, x_move, y_move, z_move) == 9){
            printf("KaBOOM!!!\n");
            //Reveal all squares on databoard.
            printuserboard();
            printf("KaBOOM!!!\n");
            break;
        }
        *getboardpointer(mask, w_move, x_move, y_move, z_move) = 1;
        sweep();
        printuserboard();
        if (toremove() == 0){
            printf("YOU WIN\n");
            break;
        }
    }

    printf("Press return to restart or ctrl+c to quit.\n");
    free(board);
    getchar();
    goto start;
}

