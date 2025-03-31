#include <stdlib.h>
#include <stdio.h>

// Define the struct _retire_info
struct _retire_info {
    int months;             // Number of months it is applicable to
    double contribution;    // Dollars contributed (or spent if negative) per month
    double rate_of_return;  // Rate of return (after inflation)
};

// Create a typedef for the struct
typedef struct _retire_info retire_info;

void printStatement(int month, double balance) {
    printf("Age %3d month %2d you have $%.2lf\n", month / 12, month % 12, balance);
}

double balance_calc(double balance, retire_info stats) {
    balance += balance * stats.rate_of_return;
    balance += stats.contribution;
  
    return balance;
}

void retirement(int startAge, double initial, retire_info working, retire_info retired) {
    // Set balance at initial to start
    double balance = initial;
    int total_months = startAge;

    for (int i = 0; i < working.months; i++) {
        printStatement(total_months, balance);
        balance = balance_calc(balance, working);
        total_months++;
    }

    for (int i = 0; i < retired.months; i++) {
        printStatement(total_months, balance);
        balance = balance_calc(balance, retired);
        total_months++;
    }

    return;

}

int main(void) {
    retire_info working = {489, 1000.0, 0.045 / 12};
    retire_info retired = {384, -4000.0, 0.01 / 12};

    retirement(327, 21345, working, retired);

    return 0;
}