#include <stdio.h>
#include <stdlib.h>

typedef long long ll;

ll gcd(ll a, ll b) {
    while (b != 0) {
        ll t = b;
        b = a % b;
        a = t;
    }
    return a;
}

ll modExp(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;

    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;

        base = (base * base) % mod;
        exp /= 2;
    }

    return result;
}

void rsa_demo() {

    printf("\n=== RSA DEMO ===\n");

    ll p = 61;
    ll q = 53;

    ll n = p * q;
    ll phi = (p - 1) * (q - 1);

    ll e = 17;

    if (gcd(e, phi) != 1) {
        printf("Invalid e\n");
        return;
    }

    ll d = 0;

    for (ll i = 1; i < phi; i++) {
        if ((e * i) % phi == 1) {
            d = i;
            break;
        }
    }

    printf("Public Key: (e=%lld, n=%lld)\n", e, n);
    printf("Private Key: (d=%lld, n=%lld)\n", d, n);

    ll message = 42;

    ll encrypted = modExp(message, e, n);
    ll decrypted = modExp(encrypted, d, n);

    printf("Original Message: %lld\n", message);
    printf("Encrypted: %lld\n", encrypted);
    printf("Decrypted: %lld\n", decrypted);
}

typedef struct {
    ll x;
    ll y;
    int infinity;
} Point;

ll ec_a = 2;
ll ec_b = 3;
ll ec_p = 97;

ll modInv(ll a, ll p) {
    a %= p;

    for (ll i = 1; i < p; i++) {
        if ((a * i) % p == 1)
            return i;
    }

    return -1;
}

Point pointAdd(Point P, Point Q) {

    Point R;
    R.infinity = 0;

    if (P.infinity)
        return Q;

    if (Q.infinity)
        return P;

    if (P.x == Q.x && P.y != Q.y) {
        R.infinity = 1;
        return R;
    }

    ll m;

    if (P.x == Q.x) {
        ll num = (3 * P.x * P.x + ec_a) % ec_p;
        ll den = modInv(2 * P.y, ec_p);
        m = (num * den) % ec_p;
    } else {
        ll num = (Q.y - P.y + ec_p) % ec_p;
        ll den = modInv((Q.x - P.x + ec_p) % ec_p, ec_p);
        m = (num * den) % ec_p;
    }

    R.x = (m * m - P.x - Q.x) % ec_p;
    R.x = (R.x + ec_p) % ec_p;

    R.y = (m * (P.x - R.x) - P.y) % ec_p;
    R.y = (R.y + ec_p) % ec_p;

    return R;
}

Point scalarMult(Point P, ll k) {

    Point R;
    R.infinity = 1;

    while (k > 0) {

        if (k % 2 == 1)
            R = pointAdd(R, P);

        P = pointAdd(P, P);
        k /= 2;
    }

    return R;
}

void ecc_demo() {

    printf("\n=== ECC DEMO ===\n");

    Point G = {3, 6, 0};

    ll privateKey = 7;

    Point publicKey = scalarMult(G, privateKey);

    printf("Generator Point G = (%lld, %lld)\n", G.x, G.y);
    printf("Private Key = %lld\n", privateKey);
    printf("Public Key = (%lld, %lld)\n", publicKey.x, publicKey.y);

    Point M = {10, 7, 0};

    ll k = 5;

    Point C1 = scalarMult(G, k);
    Point C2 = pointAdd(M, scalarMult(publicKey, k));

    printf("Encrypted:\n");
    printf("C1 = (%lld, %lld)\n", C1.x, C1.y);
    printf("C2 = (%lld, %lld)\n", C2.x, C2.y);

    Point S = scalarMult(C1, privateKey);

    Point negS = {S.x, (ec_p - S.y) % ec_p, 0};

    Point decrypted = pointAdd(C2, negS);

    printf("Decrypted Point = (%lld, %lld)\n",
           decrypted.x,
           decrypted.y);
}

int main() {

    rsa_demo();
    ecc_demo();

    return 0;
}