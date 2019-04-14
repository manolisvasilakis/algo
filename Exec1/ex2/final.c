#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MAX_TIME 2e9

struct particle {
	long long start_time, speed;
	bool search;
};

int main() {
	long long N, L, K, i, k, id_a, id_b;
	struct particle *list_a, *list_b;
	// isos long double
	double low, up, mid;				// position to check
	double min_left, min_right, temp;	// min time for particle a/b to reach position
	
	/* read N, L, K */
	scanf("%lld", &N);
	scanf("%lld", &L);
	scanf("%lld", &K);

	/* allocate memory */
	list_a = (struct particle *) malloc(N * sizeof(struct particle));
	list_b = (struct particle *) malloc(N * sizeof(struct particle));
	if (list_a == NULL || list_b == NULL) {
		printf("ERROR WITH MALLOC\n");
		exit(1);
	}

	/* read input */
	for (i = 0; i < N; i++) {
		scanf("%lld", &(list_a[i].start_time));
		scanf("%lld", &(list_a[i].speed));
		list_a[i].search = true;
	}

	for (i = 0; i < N; i++) {
		scanf("%lld", &(list_b[i].start_time));
		scanf("%lld", &(list_b[i].speed));
		list_b[i].search = true;
	}
	/* done reading */

	/* detect first K collisions */
	for (k = 0; k < K; k++) {
		low = 0;
		up = L;
		while (1) {
			mid = (low + up) / 2;

			/* min time for particles a / b to reach position mid
			 * check if .search == true 
			 * stop if start_time >= current_min (particles sorted by start_time) */
			min_left = MAX_TIME;
			id_a = 0;
			min_right = MAX_TIME;
			id_b = N-1;

			/* a particles */
			for (i = 0; i < N; i++) {
				if (list_a[i].start_time >= min_left) break;
				if (list_a[i].search) {
					temp = list_a[i].start_time + (mid / list_a[i].speed);	// time for list_a[i] to reach position mid
					if (temp < min_left) {
						min_left = temp;
						id_a = i;
					}
				}
			}

			/* b particles */
			for (i = 0; i < N; i++) {
				if (list_b[i].start_time >= min_right) break;
				if (list_b[i].search) {
					temp = list_b[i].start_time - ((mid-L) / list_b[i].speed);	// time for list_b[i] to reach position mid
					if (temp < min_right) {
						min_right = temp;
						id_b = i;
					}
				}

			}
		
			if (fabs(min_left - min_right) < 0.001) break;
			else if (min_left < min_right) low = mid;
			else up = mid;
		}
		printf("%lld %lld\n", id_a+1, id_b+1);
		list_a[id_a].search = false;
		list_b[id_b].search = false;
	}

	/* free allocated memory */
	free(list_a);
	free(list_b);
	return 0;
}
