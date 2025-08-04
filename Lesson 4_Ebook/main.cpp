#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

class ReadingSystem {

public:
    ReadingSystem() : user_pages_(MAX_USER, 0), page_counts_(MAX_PAGE, 0), total_users_(0) {}

    void Read(int user, int page) {
        int prev_page = user_pages_[user];
        if (prev_page == 0) {
            total_users_++;
        } else {
            page_counts_[prev_page]--;
        }

        user_pages_[user] = page;
        page_counts_[page]++;
    }

    double Cheer(int user) const {
        int current_page = user_pages_[user];
        if (current_page == 0) {
            return 0.0;
        }

        if (total_users_ == 1) {
            return 1.0;
        }

        int less_pages = 0;
        for (int p = 1; p < current_page; ++p) {
            less_pages += page_counts_[p];
        }

        return static_cast<double>(less_pages) / (total_users_ - 1);
    }

private:
    const int MAX_USER = 1e5 + 1;
    const int MAX_PAGE = 1001;

    vector<int> user_pages_;
    vector<int> page_counts_;
    int total_users_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int Q;
    cin >> Q;

    ReadingSystem system;

    while (Q--) {
        string query;
        cin >> query;

        if (query == "READ") {
            int user, page;
            cin >> user >> page;
            system.Read(user, page);
        } else if (query == "CHEER") {
            int user;
            cin >> user;
            double ratio = system.Cheer(user);
            cout << setprecision(6) << ratio << '\n';
        }
    }

    return 0;
}
