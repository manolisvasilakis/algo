/* Improvements :
   1. Stack instead of queue, O(1) insert
   2. Not waiting for stack to empty, finish as soon as found all nodes
      and change stack lower bound
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stack>
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
    stack<int> my_q;
    int size = 0, flag = 0;

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
        flag = 0;
        counter = 0;
        memset(visited, 0, sizeof(visited));
        visited[i] = 1;
        my_q.push(i);
        while (my_q.size() > size) {
            v = my_q.top();
            my_q.pop();

            for (j = 0; j < graph[v].size(); j++) {
                if (!visited[graph[v][j]]) {
                    counter++;
                    if (counter == N-1) {
                        result++;
                        size = my_q.size();
                        flag = 1;
                        break;
                    }
                    visited[graph[v][j]] = 1;
                    my_q.push(graph[v][j]);
                }
            }
            if (flag) break;
        }
    }

    printf("%d\n", result);
    return 0;
}
