#include <iostream>
#include <iomanip> 

using namespace std;

double max(double a, double b) {
    return a > b ? a : b;
}

double calc(double a, double b, double x) {
    double m1 = x / (a + 6*b);
    double m2 = x / a;
    return max(38 * m1, 2 * m2);
}

int main() {
    int n;
    cin >> n;
    for (int i =0; i < n; ++i) {
        int a, b, x;
        cin >> a >> b >> x;
        cout << std::setprecision(15) << calc(a, b, x) << endl;
    }

    return 0;
}
