#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

int *board = 0;
int *data  = 0;
int x_size = 0;
int y_size = 0;
int mines  = 0;

int getboundedint(int lower, int upper){
    int ret;
    do{
        scanf("%d", &ret);
    }while( !(lower <= ret && ret <= upper));
    return ret;
}

int main(void){

    int *getboardpointer(int *someboard, int x, int y){
        if (0 <= x && x < x_size && 0 <= y && y < y_size){
            return (someboard + ((sizeof(int)) * (x + (x_size * y) ) ) );
        } else {
            return 0;
        }
    }

    int isproximity(int x, int y){
        if (getboardpointer(board, x, y) != 0){
            if (*getboardpointer(board, x, y) == 9){
                return 1;
            }
        }
        return 0;
    }

    int proximity(int x, int y){
        int ret = 0;
        ret += isproximity(x-1, y-1);
        ret += isproximity(x-1, y  );
        ret += isproximity(x-1, y+1);
        ret += isproximity(x  , y-1);
        ret += isproximity(x  , y+1);
        ret += isproximity(x+1, y-1);
        ret += isproximity(x+1, y  );
        ret += isproximity(x+1, y+1);
        return ret;
    }

    int isvisadj(int x, int y){
        if (getboardpointer(board, x, y) != 0){
            if (*getboardpointer(board, x, y) == 0 && *getboardpointer(data, x, y) == 1){
                return 1;
            }
        }
        return 0;
    }

    int visadj (int x, int y){
        if (
            isvisadj(x-1, y-1) ||
            isvisadj(x-1, y  ) ||
            isvisadj(x-1, y+1) ||
            isvisadj(x  , y-1) ||
            isvisadj(x  , y+1) ||
            isvisadj(x+1, y-1) ||
            isvisadj(x+1, y  ) ||
            isvisadj(x+1, y+1)
        ){
            return 1;
        }
        return 0;
    }

    void printsquare(int x, int y){
        if (*getboardpointer(data, x, y) == 1){
            if (*getboardpointer(board, x, y) == 0){
                printf(" ");
            }
            else{
                printf("%d", *getboardpointer(board, x, y));
            }
        }
        else{
            printf("#");
        }
    }

    void printhorizbound(){
		printf("# ");
        for(int loop = 0; loop < x_size; loop++){
            printf("%02d ", loop);
        }
        printf(" #\n");
	}

    void printboard(int *someboard){
        printhorizbound();
        for (int y_loop = y_size - 1; y_loop >= 0; y_loop--){
            printf("%02d", y_loop);
            for (int x_loop = 0; x_loop < x_size; x_loop++){
                printf(" %d ", *getboardpointer(someboard, x_loop, y_loop));
            }
            printf("%02d\n", y_loop);
        }
        printhorizbound();
    }

    void printuserboard(void){
        printhorizbound();
        for (int y_loop = y_size - 1; y_loop >= 0; y_loop--){
            printf("%02d", y_loop);
            for (int x_loop = 0; x_loop < x_size; x_loop++){
                printf(" ");
                printsquare(x_loop, y_loop);
                printf(" ");
            }
            printf("%02d\n", y_loop);
        }
        printhorizbound();
    }

    void sweep(void){
        int isdone;
        do{
            isdone = 1;
            for (int x_loop = 0; x_loop < x_size; x_loop++){
                for (int y_loop = 0; y_loop < y_size; y_loop++){
                    if (visadj(x_loop, y_loop) && *getboardpointer(data, x_loop, y_loop) == 0){
                        isdone = 0;
                        *getboardpointer(data, x_loop, y_loop) = 1;
                    }
                }
            }
        }while (isdone == 0);
    }

    int toremove(void){
        int count = 0;
        for (int x_loop = 0; x_loop < x_size; x_loop++){
            for (int y_loop = 0; y_loop < y_size; y_loop++){
                if (*getboardpointer(data, x_loop, y_loop) == 0){
                    count++;
                }
            }
        }
        return count - mines;
    }

    start:
    printf ("Enter the X size of the board\n[1-70]: ");
    x_size = getboundedint(1, 70);
    printf ("Enter the Y size of the board\n[1-70]: ");
    y_size = getboundedint(1, 70);
    printf ("Enter the number of mines\n[0-%d] :", x_size * y_size -1);
    mines = getboundedint(0, x_size * y_size -1);

    board = calloc(sizeof(int), x_size * y_size * 20);//The databoard is also allocated in this memory, immediately following board
    //Holds the mines (9) and the proximity (0 - 8);
    if (board == 0){
        printf("Insufficient memory to hold the board.\nPlease enter a smaller board size and upgrade your computer.\n");
        goto start; //If the pointer is NULL, then there is nothing to free.
    }

    data = (board + x_size * y_size * sizeof(int));
    *getboardpointer(data, x_size-1, y_size-1) = 0;
    getchar();
    //A 'boolean' mask for the board: anyhting behind a 0 is not visible to the player.
    srand(time(0));
    for (int loop = 0; loop < mines; ){
        int mine_x = rand() % x_size;
        int mine_y = rand() % y_size;
        int *boardpointer = getboardpointer(board, mine_x, mine_y);
        if (*boardpointer == 0){
            *boardpointer = 9;
            loop++;
        }
    }

    for(int x_loop = 0; x_loop < x_size; x_loop++){
        for (int y_loop = 0; y_loop < y_size; y_loop++){
            int *boardpointer = getboardpointer(board, x_loop, y_loop);
            if (*boardpointer == 0){
                *getboardpointer(board, x_loop, y_loop) = proximity(x_loop, y_loop);
            }
        }
    }

    printuserboard();

    while(1){
        int x_move;
        int y_move;
        printf("Enter X position\n[0-%d]: ", x_size - 1);
        x_move = getboundedint(0, x_size - 1);
        printf("Enter Y position\n[0-%d]: ", y_size - 1);
        y_move = getboundedint(0, y_size - 1);
        if (*getboardpointer(data, x_move, y_move) != 0){
            printf("You have already revealed this square!\n");
            continue;
        }
        if (*getboardpointer(board, x_move, y_move) == 9){
            printf("KaBOOM!!!\n");
            printboard(board);
            printf("KaBOOM!!!\n");
            break;
        }
        *getboardpointer(data, x_move, y_move) = 1;
        sweep();
        printuserboard();
        if (toremove() == 0){
            printf("YOU WIN\n");
            break;
        }
    }

    printf("Press return to restart or enter q to quit.\n");
    free(board);
    getchar();
    goto start;
}
