/* n = 10 */
#include <stdio.h>
#include <math.h>

/* Function of computing factorial for int n > 0 */
long factorial(int n) {
    if (n == 0 || n == 1)
        return 1;
    long result = 1;
    for (int i = 2; i <= n; i++)
        result *= i;
    return result;
}

int main(void) {
    int n;
    while (1) {
        printf("Enter an integer: ");
        if (scanf("%d", &n) != 1)
            break;
        if (n <= 0) {
            printf("Goodbye!\n");
            break;
        }
    /* Compute Math functions */
    double log_n = log(n);
    double sqrt_n = sqrt(n);
    int n_self = n;
    double n_log_n = n * log(n);
    int n_sqr = n * n;
    int n_cube = n * n * n;
    double two_power_n = pow(2,n);
    long n_factorial = factorial(n);
    double n_power_n = pow(n,n);

    printf("For n = %d:\n", n);
    printf("log(n)     = %.0f\n", log_n);
    printf("sqrt(n)    = %.0f\n", sqrt_n);
    printf("n          = %d\n", n_self);
    printf("n * log(n) = %.0f\n", n_log_n);
    printf("n^2        = %d\n", n_sqr);
    printf("n^3        = %d\n", n_cube);
    printf("2^n        = %.0f\n", two_power_n);
    printf("n!         = %ld\n", n_factorial);
    printf("n^n        = %.0f\n", n_power_n);    
    }
    return 0;
}