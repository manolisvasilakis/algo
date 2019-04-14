#include <stdio.h>
#include <stdlib.h>

// +2 sto X gti 0-X +1 gia orio
long long grid[180][180][102];

int main() {
    int N, M, K, X;
    int i, j, k, temp1, temp2;
    long long extra, result = 0;

    // read input parameters
    scanf("%d %d %d %d", &N, &M, &K, &X);

    // initialise grid
    for(i=0; i<N; i++) {
        for(j=0; j<M; j++) {
            for(k=0; k<=X+1; k++) {
                grid[i][j][k] = 0;
            }
        }
    }

    // starting position
    grid[N-1][M-1][0] = 1;

    // read diabaseis, mono sto grid[i][j][0]
    for(i=0; i<K; i++) {
        scanf("%d %d", &temp1, &temp2);
        grid[temp1 / M][temp1 % M][0] = temp2 * (-1);
    }

    for(k=0; k<=X; k++) {
        for(i=N-1; i>=0; i--) {
            for(j=M-1; j>=0; j--) {
                // check if diabash apo dejia h apo kato prin prostheso || h ean eimai sta akra
                extra = 0;
                if ((i+1 < N) && (grid[i+1][j][0] > 0)) extra += grid[i+1][j][k];  //apo kato den eixa arxh diabashs, ara mporo na ertho
                if ((j+1 < M) && (grid[i][j+1][0] > 0)) extra += grid[i][j+1][k];  //apo dejia den eixa arxh diabashs, ara mporo na ertho
                extra = extra % 1000000103;

                // eimai se arxh diabashs
                if (grid[i][j][0] < 0) {
                    temp1 = (-1) * grid[i][j][0];
                    grid[temp1 / M][temp1 % M][k+1] = (grid[temp1 / M][temp1 % M][k+1] + extra) % 1000000103;
                }
                else {
                    grid[i][j][k] = (grid[i][j][k] + extra) % 1000000103;
                }
            }
        }
    }

    for(k=0; k<=X; k++)
        result = (result + grid[0][0][k]) % 1000000103;

    printf("%lli\n", result);
    return 0;
}
