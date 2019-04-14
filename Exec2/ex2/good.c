#include <stdio.h>
#include <stdlib.h>

int main()
{
    long long n, i, j, result, *credit, *lis, *lis1;

    /* read n */
    scanf("%lli", &n);

    /* allocate memory */
    credit = (long long *) malloc(n * sizeof(long long));
    lis = (long long *) malloc(n * sizeof(long long));
    lis1 = (long long *) malloc(n * sizeof(long long));
    if (!credit || !lis || !lis1) {
        printf("ERROR WITH MALLOC\n");
        exit(1);
    }

    /* lis[i] = max subseq from start to i, necessarily have i
     * lis1[i] = max subseq from i to end, necessarily have i
     * i fix them later */

    for (i=0; i<n; i++) {

        /* read social credits */
        scanf("%lli", &credit[i]);

        lis[i] = 1;
        for (j=0; j<i; j++) {
            if (credit[i] > credit[j] && lis[i] < lis[j] + 1)
                lis[i] = lis[j] + 1;
        }
    }

    for (i=n-1; i>=0; i--) {
        lis1[i] = 1;
        for (j=i+1; j<n; j++) {
            if (credit[i] < credit[j] && lis1[i] < lis1[j] + 1)
                lis1[i] = lis1[j] + 1;
        }
    }

    // fix
    for (i=1; i<n; i++) {
        if (lis[i] < lis[i-1]) lis[i] = lis[i-1];
        if (lis1[n-1 - i] < lis1[n-i]) lis1[n-1 - i] = lis1[n-i];
    }


/*    // fix2
    for (i=n-2; i>=0; i--) {
        if (lis1[i] < lis1[i+1]) lis1[i] = lis1[i+1];
    }
*/
    result = lis[n-1];
    for (i=0; i<n-1; i++) {
        if (lis[i] + lis1[i+1] > result)
            result = lis[i] + lis1[i+1];
    }

    printf("%lli\n", result);
    free(credit);
    free(lis);
    free(lis1);
    return 0;
}
