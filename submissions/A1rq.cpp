#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ld = long double;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()

const int R = 74;
const int SIGMA = 74;

int get_char(const string& s, int d) {
    if (d == (int)s.size()) return -1;
    static const array<int, 128> rank = [] {
        array<int, 128> result{};
        result.fill(-1);
        const string symbols = "!#%&()*-0123456789:;@ABCDEFGHIJKLMNOPQRSTUVWXYZ^abcdefghijklmnopqrstuvwxyz";
        for (int i = 0; i < (int)symbols.size(); i++) result[(unsigned char)symbols[i]] = i;
        return result;
    }();
    return rank[(unsigned char)s[d]];
}

void string_quick_sort(vector<string>& a, int l, int r, int d) {
    if (l >= r) return;

    int mid = l + (r - l) / 2;
    swap(a[l], a[mid]);
    int pivot = get_char(a[l], d);

    int lt = l, gt = r;
    int i = l + 1;
    while (i <= gt) {
        int cur = get_char(a[i], d);
        if (cur < pivot) swap(a[lt++], a[i++]);
        else if (cur > pivot) swap(a[i], a[gt--]);
        else i++;
    }

    string_quick_sort(a, l, lt - 1, d);
    if (pivot != -1) string_quick_sort(a, lt, gt, d + 1);
    string_quick_sort(a, gt + 1, r, d);
}

void msd_sort(vector<string>& a, vector<string>& tmp, int l, int r, int d) {
    if (l >= r) return;
    if (r - l + 1 < SIGMA) {
        string_quick_sort(a, l, r, d);
        return;
    }

    array<int, R + 2> cnt{};
    for (int i = l; i <= r; i++) cnt[get_char(a[i], d) + 2]++;
    for (int i = 0; i < R + 1; i++) cnt[i + 1] += cnt[i];

    auto bounds = cnt;
    for (int i = l; i <= r; i++) tmp[l + cnt[get_char(a[i], d) + 1]++] = move(a[i]);
    for (int i = l; i <= r; i++) a[i] = move(tmp[i]);

    for (int c = 0; c < R; c++) {
        int from = l + bounds[c + 1];
        int to = l + bounds[c + 2] - 1;
        msd_sort(a, tmp, from, to, d + 1);
    }
}

void solve() {
    int n;
    cin >> n;

    vector<string> a(n), tmp(n);
    for (string& s : a) cin >> s;

    msd_sort(a, tmp, 0, n - 1, 0);

    for (const string& s : a) cout << s << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();

    return 0;
}
