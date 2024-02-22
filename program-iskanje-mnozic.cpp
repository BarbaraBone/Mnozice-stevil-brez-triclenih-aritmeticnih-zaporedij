
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <ctime>
#include <sstream>
#include <string>

using namespace std;

map<int, int> MEJE;

bool brez_3AZ(vector<int> &zap) {
    int n = zap.size();
    for(int i = 0; i < n; ++i) {
        for(int j = i + 1; j < n; ++j) {
            int razlika = zap[j] - zap[i];
            int naslednje_stevilo = zap[j] + razlika;
            if (find(zap.begin(), zap.end(), naslednje_stevilo) != zap.end()) {
                return false;
            }
        }
    }
    return true;
}

bool algoritem(int n, int k, map<int, int> &meje) {
    if (n < k) {
        throw invalid_argument("Neprimerna izbira n in k.");
    }
    if (n == k) {
        return false;
    }
    vector<int> zap = {1, 2};
    while (zap[0] == 1 && !(zap.size() == k && brez_3AZ(zap))) {
        int l = zap.size();
        if (zap.back() > n) {
            return false;
        }
        if (brez_3AZ(zap)) {
            zap.push_back(zap.back() + 1);
        } else {
            while (l > 1 && meje[n - zap[l - 1]] < k - l + 1) {
                --l;
            }
            zap.resize(l - 1);
            zap.push_back(zap[l - 1] + 1);
        }
    }
    if (zap[0] != 1) {
        return false;
    }
    if (zap.size() == k && brez_3AZ(zap)) {
        // print the zap in stdout
        for (int i = 0; i < k; ++i) {
            // if the number is last no comma
            if (i == k - 1) {
                cout << zap[i];
                break;
            }
            cout << zap[i] << ",";
        }
        cout << "\n";
        return true;
    }
    return false;
}

string format_time(int seconds) {
    int minutes = seconds / 60;
    int hours = minutes / 60;
    seconds %= 60;
    minutes %= 60;
    ostringstream str;
    str << hours << ":" << minutes << ":" << seconds;
    return str.str();
}

int main() {
    ifstream ifs("izracun.csv");
    string line;
    while(getline(ifs, line)) {
        stringstream ss(line);
        string token;
        getline(ss, token, ',');
        int n = stoi(token);
        getline(ss, token, ',');
        int meja = stoi(token);
        MEJE[n] = meja;
    }
    ifs.close();
    int n = max_element(MEJE.begin(), MEJE.end())->first;
    cout << "Shranjene imam ze velikosti podmnozic brez 3AZ do vkljucno stevila " << n << ".\n";
    // we would like to continiue from where we left off
    while(1) {
    n += 1;
        cout << "Racunam natancno mejo za " << n << ".\n";
        time_t start_time = time(0);

        if (MEJE[n - 3] + 2 == MEJE[n - 2] + 1 && MEJE[n - 2] + 1 == MEJE[n - 1]) {
            MEJE[n] = MEJE[n - 1];
        } else {
            int k = MEJE[n - 1];
            if (algoritem(n, k + 1, MEJE)) {
                MEJE[n] = k + 1;
            } else {
                MEJE[n] = MEJE[n - 1];
            }
        }

        int elapsed_time = difftime(time(0), start_time);
        cout << "Izracun za " << n << " je trajal " << format_time(elapsed_time) << ".\n";

        ofstream ofs("izracun.csv", ofstream::app);
        ofs << n << "," << MEJE[n] << "\n";
        ofs.close();
    }

    return 0;
}
