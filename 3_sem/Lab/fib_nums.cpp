#include <iostream>
#include <ctime>
#include <vector>
#include <iomanip>

using ll = long long;

constexpr int mod = 1000000007;

class Matrix {
public:
    constexpr Matrix(int b00 = 1, int b01 = 1, int b10 = 1, int b11 = 0) :
                     a00(b00), a01(b01), a10(b10), a11(b11) {}

    constexpr int operator()(int row, int col) const {
        return (!row && !col) ? a00 : ((!row && col) ? a01 :
                                      ((row && !col) ? a10 : a11));
    }

    constexpr Matrix operator*(const Matrix& x) const {
       return Matrix(mult_elem(a00, a01, x.a00, x.a10), mult_elem(a00, a01, x.a01, x.a11),
                     mult_elem(a10, a11, x.a00, x.a10), mult_elem(a10, a11, x.a01, x.a11));
    }

private:
    int _size = 2;
    int a00;
    int a01;
    int a10;
    int a11;

    constexpr int mult_elem(int a1, int a2, const int& b1, const int& b2) const {
        return ((1ll * a1 * b1) % mod + (1ll * a2 * b2) % mod) % mod;
    }
};

template <class U>
constexpr U bin_powTale(const U& a, const ll pow, const U& ans) {
    return (!pow) ? ans : ((pow & 1) ? bin_powTale(a * a, pow / 2, ans * a) :
                          bin_powTale(a * a, pow / 2, ans));
}

template <class U>
constexpr U bin_pow(U a, ll pow) {
    return bin_powTale(a, pow, U(1, 0, 0, 1));
}

constexpr int binPow_fib(ll n) {
    return (bin_pow(Matrix(), n)(0, 1) );
}

constexpr int naive_fib(ll n) {
    return n == 0 ? 0 :
          (n == 1 ? 1 : (naive_fib(n - 1) + naive_fib(n - 2)) % mod);
}

constexpr int classic_fib(ll n) {
    int a = 0;
    int b = 1;
    for (ll i = 0ll; i < n; i++) {
        int tmp = a;
        a = b;
        b = (b + tmp) % mod;
    }
    return a;
}

int main()
{
    return 0;
}
