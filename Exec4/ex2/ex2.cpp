// bfs

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue>
#include <vector>

using namespace std;

int main()
{
    int i, j, loss, src, N;
    int result = 0;
    int counter;
    int v;
    scanf("%d", &N);
    int visited[N];
    queue<int> my_q;

    // Create a graph given in the above diagram, Winner -> loser
    vector<int> graph[N];


    for (i=0; i<N; i++) {
        scanf("%d", &loss);
        for (j=0; j<loss; j++) {
            scanf("%d", &src);
            graph[src-1].push_back(i);
        }
    }

    for (i=0; i<N; i++) {
        counter = 0;
        memset(visited, 0, sizeof(visited));
        visited[i] = 1;
        my_q.push(i);
        while (!my_q.empty()) {
            v = my_q.front();
            my_q.pop();

            for (j = 0; j < graph[v].size(); j++) {
                if (!visited[graph[v][j]]) {
                    counter++;
                    visited[graph[v][j]] = 1;
                    my_q.push(graph[v][j]);
                }
            }
        }
        if (counter == N-1) result++;
    }

    printf("%d\n", result);
    return 0;
}
