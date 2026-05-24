#include <bits/stdc++.h>
using namespace std;

int get_char(const string& s, int d) {
    return d == (int)s.size() ? -1 : (unsigned char)s[d];
}

void string_quick_sort(vector<string>& a, int l, int r, int d) {
    if (l >= r) return;
    int mid = l + (r - l) / 2;
    swap(a[l], a[mid]);
    int pivot = get_char(a[l], d);
    int lt = l, gt = r, i = l + 1;
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

void solve() {
    int n;
    cin >> n;
    vector<string> a(n);
    for (string& s : a) cin >> s;
    string_quick_sort(a, 0, n - 1, 0);
    for (const string& s : a) cout << s << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}
