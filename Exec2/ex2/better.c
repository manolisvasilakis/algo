/*
 * CHECK HERE
 * https://www.geeksforgeeks.org/longest-monotonically-increasing-subsequence-size-n-log-n/
 */

#include <stdio.h>
#include <stdlib.h>

// Binary search (note boundaries in the caller)
long long CeilIndex(long long *v, long long l, long long r, long long key) {

    long long m;

    while (r - l > 1) {
        m = l + (r - l) / 2;
        if (v[m] >= key) r = m;
        else l = m;
    }

    return r;
}

// Binary search 2 (note boundaries in the caller) | have reversed l, r because v[i] anapoda
long long FloorIndex(long long *v, long long l, long long r, long long key) {

    long long m;

    while (r - l > 1) {
        m = l + (r - l) / 2;
        if (v[m] >= key) l = m;
        else r = m;
    }

    return r;
}



int main() {
    /* len[i] = max subseq 0...i, later add i+1...n */
    long long n, i, j, length, *credit, *tail, *len, *len2;

    /* read n */
    scanf("%lli", &n);

    /* allocate memory */
    credit = (long long *) malloc(n * sizeof(long long));
    tail = (long long *) malloc(n * sizeof(long long));
    len = (long long *) malloc(n * sizeof(long long));
    len2 = (long long *) malloc(n * sizeof(long long));
    if (!credit || !tail || !len || !len2) {
        printf("ERROR WITH MALLOC\n");
        exit(1);
    }

    /* read 1st social credit, rest in for */
    scanf("%lli", &credit[0]);


    // for 0...i
    length = 1; // always points empty slot in tail

    tail[0] = credit[0];
    len[0] = 1;
    for (i=1; i<n; i++) {

        // read rest social credits
        scanf("%lli", &credit[i]);

        // new smallest value
        if (credit[i] < tail[0]) tail[0] = credit[i];

        // credit[i] extends largest subsequence
        else if (credit[i] > tail[length - 1]) tail[length++] = credit[i];

        // credit[i] will become end candidate of an existing
        // subsequence or Throw away larger elements in all
        // LIS, to make room for upcoming grater elements
        // than credit[i] (and also, credit[i] would have already
        // appeared in one of LIS, identify the location
        // and replace it)
        else tail[CeilIndex(tail, -1, length - 1, credit[i])] = credit[i];

        len[i] = length;
    }

    // for i...n | the same, but search for longest decreasing subseq
    length = 1; // always points empty slot in tail

    tail[0] = credit[n-1];
    len2[n-1] = 1;
    for (i=n-2; i>=0; i--) {

        // new largest value
        if (credit[i] > tail[0]) tail[0] = credit[i];

        // credit[i] extends largest subsequence
        else if (credit[i] < tail[length - 1]) tail[length++] = credit[i];

        // credit[i] will become end candidate of an existing
        // subsequence or Throw away smaller elements in all
        // LIS, to make room for upcoming smaller elements
        // than credit[i] (and also, credit[i] would have already
        // appeared in one of LIS, identify the location
        // and replace it)
        else tail[FloorIndex(tail, -1, length - 1, credit[i])] = credit[i];

        len2[i] = length;
    }

    for (i=0; i<n-1; i++) {
        if (len[i] + len2[i+1] > length)
            length = len[i] + len2[i+1];
    }

    printf("%lli\n", length);

    free(credit);
    free(tail);
    free(len);
    free(len2);
    return 0;
}
