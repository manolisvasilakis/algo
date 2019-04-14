#include <stdio.h>
#include <stdlib.h>

int main() {
	long long *A, *bigger, i, k, n;
	unsigned long long *cost, min, left_cost;

	/* read N */
	scanf("%lld", &n);

	/* allocate memory */
	A = (long long*) malloc(n * sizeof(long long));
	bigger = (long long *) malloc(n * sizeof(long long));
	cost = (unsigned long long *) malloc(n * sizeof(unsigned long long));
	if (A == NULL || bigger == NULL || cost == NULL) {
		printf("ERROR with malloc\n");
		exit(1);
	}

	/* read A[i] and find cost from the left and save it in cost[i] */
	cost[0] = 0;
	scanf("%lld", &A[0]);
	/* bigger : pointer to closest to the left bigger element of A | -1 if max */
	bigger[0] = -1;	// is max from left till here
	for (i = 1; i < n; i++) {
		/* read height */
		scanf("%lld", &A[i]);

		/* find closest k from left so A[k]>=A[i] */
		k = i-1;
		while (k >= 0) {
			if (A[k] >= A[i]) break;
			else k = bigger[k];
		}
		if (k < 0) {
			bigger[i] = -1;	// max
			cost[i] = A[i] * i;
		} else if (A[k] == A[i]) {
			bigger[i] = bigger[k];
			cost[i] = cost[k] + (i - k) * A[i];
		} else {
			bigger[i] = k;
			cost[i] = cost[k] + A[k] + (i - (k+1)) * A[i];
		}
	}

	/* min = last position so I can reuse cost[n-1] */
	min = A[n-1] + cost[n-1];

	/* find cost from the right and save it in cost[i] */
	cost[n-1] = 0;
	/* bigger : pointer to closest to the right bigger element of A | -1 if max */
	bigger[n-1] = -1;	// is max from right till here
	for (i = n-2; i >= 0; i--) {

		/* keep cost from left for position i */
		left_cost = cost[i];

		/* find closest k from right so A[k]>=A[i] */
		k = i+1;
		while (k >= 0) {
			if (A[k] >= A[i]) break;
			else k = bigger[k];
		}
		if (k < 0) {
			bigger[i] = -1;	//max
			cost[i] = A[i] * ((n-1) - i);
		} else if (A[k] == A[i]) {
			bigger[i] = bigger[k];
			cost[i] = cost[k] + (k - i) * A[i];
		} else {
			bigger[i] = k;
			cost[i] = cost[k] + A[k] + ((k-1) - i) * A[i];
		}

		/* check if found new min */
		if (min > cost[i] + left_cost + A[i]) min = cost[i] + left_cost + A[i];
	}

	/* free allocated memory */
	free(A);
	free(bigger);
	free(cost);

	printf("%llu\n", min);
	return 0;
}
