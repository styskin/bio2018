#include <iostream>
#include <iomanip> 

using namespace std;

typedef long long int ll;

double max(double a, double b) {
    return a > b ? a : b;
}

double min(double a, double b) {
    return a < b ? a : b;
}

double sum(double a, double b) {
    double r1 = min(a, b/6);
    double r2 = a - r1;
    return 38 * r1 + 2 * r2;
}

double calc(double a, double b, double x) {
    double m1 = x / (a + 6*b);
    double m2 = (x / a);
    if (38 * m1 < 2 * m2) {
        cout << "111" << endl;
        return 2 * m2;
    }
    double mm = 0;
    ll mc = ll(x / (1.* a / 6 + b)); 
    for (ll i = mc; i >= max(0, mc - 1000000); --i) {
        ll ac = ll((x - i * b) / a);
        mm = max(mm, sum(ac, i));
        //cout << "Try " << i  << " " << ", ="  << sum(a, b) << endl;
    }
    return mm;
}

int main() {
    int n;
    cin >> n;
    for (int i =0; i < n; ++i) {
        ll a, b, x;
        cin >> a >> b >> x;
        cout << std::setprecision(20) << calc(a, b, x) << endl;
    }

    return 0;
}
