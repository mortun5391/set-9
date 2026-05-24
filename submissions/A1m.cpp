#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ld = long double;

#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()

struct CmpResult {
    int cmp;
    int lcp;
};

CmpResult lcp_compare(const string& a, const string& b, int known) {
    int i = known;
    while (i < (int)a.size() && i < (int)b.size() && a[i] == b[i]) i++;

    if (i == (int)a.size() || i == (int)b.size()) {
        if (a.size() == b.size()) return {0, i};
        return {a.size() < b.size() ? -1 : 1, i};
    }
    return {a[i] < b[i] ? -1 : 1, i};
}

void merge_lcp(vector<string>& a, vector<string>& tmp, vector<int>& lcp,
               vector<int>& tmp_lcp, int l, int m, int r) {
    int i = l, j = m, pos = l;
    int x = 0, y = 0;

    while (i < m && j < r) {
        if (x > y) {
            tmp[pos] = move(a[i]);
            tmp_lcp[pos++] = x;
            i++;
            if (i < m) x = lcp[i];
        } else if (y > x) {
            tmp[pos] = move(a[j]);
            tmp_lcp[pos++] = y;
            j++;
            if (j < r) y = lcp[j];
        } else {
            auto [cmp, common] = lcp_compare(a[i], a[j], x);
            if (cmp <= 0) {
                tmp[pos] = move(a[i]);
                tmp_lcp[pos++] = x;
                i++;
                y = common;
                if (i < m) x = lcp[i];
            } else {
                tmp[pos] = move(a[j]);
                tmp_lcp[pos++] = y;
                j++;
                x = common;
                if (j < r) y = lcp[j];
            }
        }
    }

    while (i < m) {
        tmp[pos] = move(a[i]);
        tmp_lcp[pos++] = x;
        i++;
        if (i < m) x = lcp[i];
    }
    while (j < r) {
        tmp[pos] = move(a[j]);
        tmp_lcp[pos++] = y;
        j++;
        if (j < r) y = lcp[j];
    }

    tmp_lcp[l] = 0;
    for (int p = l; p < r; p++) {
        a[p] = move(tmp[p]);
        lcp[p] = tmp_lcp[p];
    }
}

void string_merge_sort(vector<string>& a, vector<string>& tmp, vector<int>& lcp,
                       vector<int>& tmp_lcp, int l, int r) {
    if (r - l <= 1) {
        if (r - l == 1) lcp[l] = 0;
        return;
    }

    int m = (l + r) / 2;
    string_merge_sort(a, tmp, lcp, tmp_lcp, l, m);
    string_merge_sort(a, tmp, lcp, tmp_lcp, m, r);
    merge_lcp(a, tmp, lcp, tmp_lcp, l, m, r);
}

void solve() {
    int n;
    cin >> n;

    vector<string> a(n), tmp(n);
    vector<int> lcp(n), tmp_lcp(n);
    for (string& s : a) cin >> s;

    string_merge_sort(a, tmp, lcp, tmp_lcp, 0, n);

    for (const string& s : a) cout << s << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();

    return 0;
}
