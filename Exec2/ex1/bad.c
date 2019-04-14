#include <stdio.h>
#include <stdlib.h>

int main() {
    /* P[i] = max timh gia ta prota i edesmata */
    long long n, a, b, c, i, j, temp, *pleasure, *sum, *P;

    /* read N */
    scanf("%lli", &n);

    /* read a, b, c */
    scanf("%lli %lli %lli", &a, &b, &c);

    /* allocate memory */
    pleasure = (long long *) malloc(n * sizeof(long long));
    sum = (long long *) malloc(n * sizeof(long long));
    P = (long long *) malloc(n * sizeof(long long));
    if (!pleasure || !sum || !P) {
        printf("ERROR WITH MALLOC\n");
        exit(1);
    }

    /* read pleasure[i] and compute sum[i] */
    scanf("%lli", &pleasure[0]);
    sum[0] = pleasure[0];
    for (i=1; i<n; i++) {
        scanf("%lli", &pleasure[i]);
        sum[i] = sum[i-1] + pleasure[i];
    }

    /* gia to 1o edesma mono, anagkastika se 1 piato */
    P[0] = a*pleasure[0]*pleasure[0] + b*pleasure[0] + c;
    /* computations, yeahhh */
    for (i=1; i<n; i++) {
        /* give min value the corner case where everything goes to the same plate*/
        P[i] = a*sum[i]*sum[i] + b*sum[i] + c;
        for (j=0; j<i; j++) {
            temp = sum[i] - sum[j];
            if (P[i] < P[j] + a*temp*temp + b*temp + c)
                P[i] = P[j] + a*temp*temp + b*temp + c;
        }
    }

    printf("%lli\n", P[n-1]);
    free(pleasure);
    free(sum);
    free(P);
    return 0;
}
