#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 100
#define min(a, b) (((a) < (b)) ? (a) : (b))

int **graph;             // Matrix to store debts
char people[N][N];       // Names of people
int n;                   // Number of people

// Function to get index of person with minimum net balance
int get_min(int amt[]) {
    int min_ind = 0;
    for (int i = 1; i < n; i++)
        if (amt[i] < amt[min_ind]) min_ind = i;
    return min_ind;
}

// Function to get index of person with maximum net balance
int get_max(int amt[]) {
    int max_ind = 0;
    for (int i = 1; i < n; i++)
        if (amt[i] > amt[max_ind]) max_ind = i;
    return max_ind;
}

// Recursive function to settle debts
void min_cashflow_rec(int amt[]) {
    int mxCredit = get_max(amt), mxDebit = get_min(amt);

    // If all amounts are settled
    if (amt[mxCredit] == 0 && amt[mxDebit] == 0) return;

    int minval = min(-amt[mxDebit], amt[mxCredit]);
    amt[mxCredit] -= minval;
    amt[mxDebit] += minval;

    printf("💰 %-10s pays Rs.%3d to %-10s\n", people[mxDebit], minval, people[mxCredit]);

    min_cashflow_rec(amt);
}

// Function to compute net balances and settlements
void min_cashflow() {
    int amt[N];
    for (int i = 0; i < n; i++) amt[i] = 0;

    // Calculate net balance for each person
    for (int p = 0; p < n; p++)
        for (int i = 0; i < n; i++)
            amt[p] += (graph[i][p] - graph[p][i]);

    // Display net balances
    printf("\n📊 Net Balances:\n");
    printf("------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        if (amt[i] > 0)
            printf("   %-10s will RECEIVE Rs.%d\n", people[i], amt[i]);
        else if (amt[i] < 0)
            printf("   %-10s will PAY     Rs.%d\n", people[i], -amt[i]);
        else
            printf("   %-10s is SETTLED\n", people[i]);
    }
    printf("------------------------------------------------\n");

    // Display settlements
    printf("\n⚖️  Settlements (Minimum Transactions):\n");
    min_cashflow_rec(amt);
}

// Input debts matrix from user
void input_debts() {
    printf("\nEnter debts between people (in Rs, 0 if none):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                graph[i][j] = 0;
                continue;
            }
            int value;
            while (1) {
                printf("   %s has to pay %s: Rs.", people[i], people[j]);
                if (scanf("%d", &value) != 1 || value < 0) {
                    printf("⚠️  Invalid input. Please enter a non-negative number.\n");
                    while (getchar() != '\n'); // clear input buffer
                    continue;
                }
                graph[i][j] = value;
                break;
            }
        }
    }
}

// Input people names
void input_people() {
    for (int i = 0; i < n; i++) {
        printf("Enter name %d: ", i + 1);
        scanf("%s", people[i]);
    }
}

// Free dynamic memory
void free_memory() {
    for (int i = 0; i < n; i++) free(graph[i]);
    free(graph);
}

int main() {
    printf("\n==============================================\n");
    printf("      Welcome to Cash Flow Minimizer\n");
    printf("==============================================\n");
    printf("Goal: Reduce the number of transactions to settle debts efficiently.\n");

    // Input number of people
    while (1) {
        printf("Enter number of people (2-%d): ", N);
        if (scanf("%d", &n) != 1 || n < 2 || n > N) {
            printf("⚠️  Invalid input. Enter a number between 2 and %d.\n", N);
            while (getchar() != '\n'); // clear input buffer
            continue;
        }
        break;
    }

    // Allocate memory for debt matrix
    graph = (int **) malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
        graph[i] = (int *) malloc(n * sizeof(int));

    // Input names and debts
    input_people();
    input_debts();

    // Compute and display minimum cash flow settlements
    min_cashflow();

    // Free memory
    free_memory();

    printf("\n✅ All debts settled!\n");
    return 0;
}
