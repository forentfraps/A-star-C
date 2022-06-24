#include <stdio.h>
#include <stdlib.h>

#define W 15
#define H 15
#define X0 0
#define X1 14
#define Y0 0
#define Y1 14

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

// To enable ANSI escape code (for colors) in cmd, use the folдowing command
// REG ADD HKCU\CONSOLE /f /v VirtualTerminalLevel /t REG_DWORD /d 1 

void mwall(int **field, int x, int y){
    if (x < 0 || y < 0 || x == X0 || y == Y0 || y == Y1 || x == X1 || x >= W || y >= H || field[y][x] == 1){
        return;
    }
    field[y][x] = 1;
    return;
}
int*** create_costgrid(int m, int n)
{
    int*** board = calloc(m, sizeof(int**));
    for (int i = 0; i < m; ++i){
        board[i] = calloc(n, sizeof(int*));
        for (int j = 0; j < n; ++j){
            board[i][j] = calloc(3, sizeof(int));
        }
    }
    return board;
}
int** create_field(int m, int n)
{
    int** rows = calloc(m, sizeof(int*));
    for (int i=0; i<m; ++i)
    {
        rows[i] = calloc(n, sizeof(int));
    }
    return rows;
}
void print_field(int **n){
    int index;
    int jdex;
    int ndex;
    float tmp;

    for( index = 0; index < W; index++){
        for( jdex = 0; jdex < H; jdex++){
            int c = n[index][jdex];
            switch (c){
                case 0:
                    printf( "%d  ",n[index][jdex]);
                    break;
                case 1:
                    printf( "%s%d %s ",COLOR_CYAN, n[index][jdex], COLOR_RESET);
                    break;
                case 2:
                    printf( "%s%d %s ",COLOR_YELLOW, n[index][jdex], COLOR_RESET);
                    break;
                case 3:
                    printf( "%s%d %s ",COLOR_GREEN, n[index][jdex], COLOR_RESET);
                    break;
                case 4:
                    printf( "%s%d %s ",COLOR_MAGENTA, n[index][jdex], COLOR_RESET);
                    break;
            }           
        }
        printf("\n");
    }
    
    printf("\n");
    return;
}
int cost(int x0, int y0, int x1,int y1){
    int r = abs(x0 - x1);
    int d = abs(y0 - y1);
    int x = max(r, d);
    int n = min(r, d);
    return n * 14 + (x - n) * 10;
}
int** get_adj(int x0,int y0){
    int** adj = calloc(9, sizeof(int*));
    for (int i = 0; i < 9; ++i){
        adj[i] = calloc(2, sizeof(int));
    }
    for (int i = 0; i < 9; ++i){
            int x1 = i % 3 + x0 - 1;
            int y1 = i / 3 + y0 - 1;
            if ( x1 < 0 || y1 < 0 || x1 > W -1 || y1 > H -1){
            adj[i][0] = -1;
            adj[i][1] = -1;
            }
            else{
                adj[i][0] = x1;
                adj[i][1] = y1;
            }
    }
    adj[4][0] = -1;
    adj[4][1] = -1;
    return adj;
}
int hcost(int x0, int y0){
    return cost(x0, y0, X1, Y1);
}
void collapse(int** field, int*** costgrid, int x, int y){
    int** cords_adj = get_adj(x, y);
    int g0 = costgrid[y][x][0];
    field[y][x] = 3;
    for(int i = 0; i < 9; ++i){
        if (cords_adj[i][0] > -1 && cords_adj[i][1] > -1){
            int x1 = cords_adj[i][0];
            int y1 = cords_adj[i][1];
            int diag = 0;
            if (abs(x1 - x) + abs(y1 - y) == 2){
                diag = 1;
            }
            switch (field[y1][x1]){
                case 1:
                    break;
                case 3:
                    break;
                case 0:
                    costgrid[y1][x1][0] = g0 + 10 +4* diag;
                    costgrid[y1][x1][1] = hcost(x1, y1);
                    costgrid[y1][x1][2] = costgrid[y1][x1][0] + costgrid[y1][x1][1];
                    field[y1][x1] = 2;
                    break;
                case 2:
                    costgrid[y1][x1][0] = min(g0 +10 + 4* diag, costgrid[y1][x1][0]);
                    costgrid[y1][x1][2] = costgrid[y1][x1][0] + costgrid[y1][x1][1];
                    break;
            }
        }
    }
    return;
}
void display_path(int** field, int*** costgrid){
    int x1 = X1;
    int y1 = Y1;
    while(1 == 1){
        int** cords_adj = get_adj(x1, y1);
        int glf = 999999999;
        int glg = 999999999;
        for(int i = 0; i < 9; ++i){
            if(cords_adj[i][0] > -1 && cords_adj[i][1] > -1){
                int x2 = cords_adj[i][0];
                int y2 = cords_adj[i][1];
                if(x2 == X0 && y2 == Y0){
                    field[Y0][X0] = 4;
                    return;
                }
                if(field[y2][x2] == 3 && glf > costgrid[y2][x2][2] && costgrid[y2][x2][0] < glg){
                    glf = costgrid[y2][x2][2];
                    glg = costgrid[y2][x2][0];
                    x1 = x2;
                    y1 = y2;
                }
            }
        }
        field[y1][x1] = 4;
    }
    return;
}
void solve(int** field,int*** costgrid ){
    field[Y0][X0] = 2;
    costgrid[Y0][X0][0] = 0;
    costgrid[Y0][X0][1] = cost(X0, Y0, X1, Y1);
    costgrid[Y0][X0][2] = costgrid[Y0][X0][1];
    collapse(field, costgrid, X0, Y0);
    while (1 == 1){
        int glm = 99999999;
        int xf1 = 0;
        int yf1 = 0;
        int glh = 99999999;
        for(int i = 0; i < H; ++i){
            for (int j = 0; j < W; ++j){
                if (field[i][j] == 2){
                    if (field[Y1][X1] == 3){
                        field[Y1][X1] = 4;
                        return;
                    }
                    int h1 = costgrid[i][j][1];
                    int f1 = costgrid[i][j][2];
                    if ((glm == f1 && h1 < glh) || f1 < glm){
                        glm = f1;
                        glh = h1;
                        xf1 = j;
                        yf1 = i;

                    }
                }
            }
        }
        collapse(field, costgrid, xf1, yf1);
    }
    return;
}
int main(){
    int** field = create_field(W, H);
    int*** costgrid = create_costgrid(W, H);
    mwall(field, 2,2);
    mwall(field, 1,2);
    mwall(field, 1,3);
    mwall(field, 1,4);
    mwall(field, 3,3);
    mwall(field, 4,3);
    mwall(field, 5,3);
    mwall(field, 6,3);
    mwall(field, 6,4);
    mwall(field, 6,5);
    mwall(field, 6,6);
    mwall(field, 6,7);
    mwall(field, 6,8);
    mwall(field, 6,9);
    mwall(field, 7,6);
    mwall(field, 8,6);
    mwall(field, 9,6);
    mwall(field, 10,6);
    mwall(field, 11,6);
    print_field(field);
    solve(field, costgrid);
    display_path(field,costgrid);
    print_field(field);
    int** adj = get_adj(4,4);
    return 0;
}
