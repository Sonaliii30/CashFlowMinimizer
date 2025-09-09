#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 100
#define min(i, j) (((i) < (j)) ? (i) : (j))

int **graph;
char people[N][N];
int n;

int get_min(int a[]) {
    int min_ind = 0;
    for (int i = 1; i < n; i++) {
        if (a[i] < a[min_ind]) min_ind = i;
    }
    return min_ind;
}

int get_max(int a[]) {
    int max_ind = 0;
    for (int i = 1; i < n; i++) {
        if (a[i] > a[max_ind]) max_ind = i;
    }
    return max_ind;
}

void min_cashflow_rec(int amt[]) {
    int mxCredit = get_max(amt), mxDebit = get_min(amt);

    if (amt[mxCredit] == 0 && amt[mxDebit] == 0) return;

    int minval = min(-amt[mxDebit], amt[mxCredit]);
    amt[mxCredit] -= minval;
    amt[mxDebit] += minval;

    printf("💰 %s will pay Rs.%d to %s\n", people[mxDebit], minval, people[mxCredit]);

    min_cashflow_rec(amt);
}

void min_cashflow() {
    int amt[N];
    for (int i = 0; i < n; i++) amt[i] = 0;

    // Calculate net balance
    for (int p = 0; p < n; p++) {
        for (int i = 0; i < n; i++) {
            amt[p] += (graph[i][p] - graph[p][i]);
        }
    }

    printf("\n📊 Net balances:\n");
    for (int i = 0; i < n; i++) {
        printf("   %-10s : %d\n", people[i], amt[i]);
    }

    printf("\n⚖️  Settlements (Minimum Cash Flow):\n");
    min_cashflow_rec(amt);
}

int main() {
    printf("\n==============================================\n");
    printf("   💸 Welcome to Cash Flow Minimizer Project 💸\n");
    printf("==============================================\n");
    printf("👉 Goal: Reduce the number of transactions to settle debts.\n");
    printf("👉 This is the FINAL PROJECT version.\n\n");

    printf("Enter number of people: ");
    scanf("%d", &n);

    graph = (int **) malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        graph[i] = (int *) malloc(n * sizeof(int));
    }

    for (int i = 0; i < n; i++) {
        printf("Enter name %d: ", i + 1);
        scanf("%s", people[i]);
    }

    printf("\nEnter debts (in Rs):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                graph[i][j] = 0;
                continue;
            }
            printf("   %s has to pay %s: Rs.", people[i], people[j]);
            scanf("%d", &graph[i][j]);
        }
    }

    min_cashflow();

    for (int i = 0; i < n; i++) free(graph[i]);
    free(graph);

    return 0;
}
