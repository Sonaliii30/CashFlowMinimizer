#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define RESET   "\033[0m"

#define N 100
#define min(a, b) (((a) < (b)) ? (a) : (b))

int graph[N][N];         // Matrix to store debts
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
            printf(GREEN "   %-10s will RECEIVE Rs.%d\n" RESET, people[i], amt[i]);
        else if (amt[i] < 0)
            printf(RED "   %-10s will PAY     Rs.%d\n" RESET, people[i], -amt[i]);
        else
            printf(YELLOW "   %-10s is SETTLED\n" RESET, people[i]);
    }
    printf("------------------------------------------------\n");

    // Display settlements
    printf("\n⚖️ Settlements (Minimum Transactions):\n");
    min_cashflow_rec(amt);
}

// Helper function to get a person's index or add them if new
int get_person_index(const char* name) {
    for (int i = 0; i < n; ++i) {
        if (strcmp(people[i], name) == 0) {
            return i;
        }
    }
    // If person not found, add them
    if (n < N) {
        strcpy(people[n], name);
        return n++;
    }
    return -1; // Error case
}

int main() {
    int num_transactions;
    if (scanf("%d", &num_transactions) != 1) {
        printf("Error: Invalid number of transactions.\n");
        return 1;
    }

    n = 0; // Reset number of people
    // Initialize graph with zeros
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            graph[i][j] = 0;
        }
    }

    for (int i = 0; i < num_transactions; ++i) {
        char payer[N], receiver[N];
        int amount;
        if (scanf("%s %s %d", payer, receiver, &amount) != 3) {
            printf("Error: Invalid transaction format.\n");
            return 1;
        }

        int payer_idx = get_person_index(payer);
        int receiver_idx = get_person_index(receiver);

        if (payer_idx == -1 || receiver_idx == -1) {
            printf("Error: Maximum number of people reached.\n");
            return 1;
        }

        graph[payer_idx][receiver_idx] += amount;
    }

    min_cashflow();

    return 0;
}