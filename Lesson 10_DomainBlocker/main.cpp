#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

using namespace std;

class Domain {
public:
    // МОЖНО ЛУЧШЕ
    // Здесь можно использовать perfect forwarding.
    // Тогда удастся избежать лишних копирований.
    Domain(const string& str) : domain_(str){
        std::reverse(domain_.begin(), domain_.end());
        domain_ += '.';
    }

    bool operator==(const Domain& other) const{
        return domain_ == other.domain_;
    }

    bool IsSubdomain(const Domain& parent) const {
        if (domain_.size() < parent.domain_.size()) {
            return false;
        }
        // КРУТО
        // Эх, а я не догадался применить стандартную функцию. Хорошо сделано!
        return std::equal(parent.domain_.begin(), parent.domain_.end(), domain_.begin());
    }

    const std::string& GetReversed() const {
        return domain_;
    }
private:
    string domain_;
};

class DomainChecker {
public:
    template<typename Iterator>
    DomainChecker(Iterator begin, Iterator end) : blocked_domains_(begin, end){
        //Сортируем домены через реверс
        std::sort(blocked_domains_.begin(),blocked_domains_.end(), [](const Domain& lhs, const Domain& rhs){
            return lhs.GetReversed() < rhs.GetReversed();
        });

        //
        auto it = std::unique(blocked_domains_.begin(), blocked_domains_.end(), [](const Domain& lhs, const Domain& rhs) {
            return rhs.IsSubdomain(lhs);  // Удаляем rhs, если он поддомен lhs
        });

        blocked_domains_.erase(it,blocked_domains_.end());
    }

    bool IsForbidden(const Domain& domain) const {
        auto it = std::upper_bound(blocked_domains_.begin(), blocked_domains_.end(), domain, [](const Domain& lhs, const Domain& rhs) {
            return lhs.GetReversed() < rhs.GetReversed();
        });

        // МОЖНО ЛУЧШЕ
        // Всю конструкцию, включая ретёрно, можно записать в одну строку,
        // используя оператор ||.
        if (it != blocked_domains_.begin()) {
            --it;
            return domain.IsSubdomain(*it);
        }
        return false;
    }

private:
    std::vector<Domain> blocked_domains_;
};


template<typename DomainsQuantity>
std::vector<Domain> ReadDomains(istream& input, DomainsQuantity dq){
    std::vector<Domain> domains;
    domains.reserve(dq);
    string line;
    for (size_t i = 0; i < dq; ++i){
        getline(input, line);
        domains.emplace_back(line);
    }
    return domains;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

//==============================Test Start================================
void TestDomain() {
    // Проверка конструктора и оператора ==
    assert(Domain("gdz.ru") == Domain("gdz.ru"));
    assert(!(Domain("gdz.ru") == Domain("maps.me")));

    // Проверка IsSubdomain
    Domain parent("gdz.ru");
    Domain sub1("math.gdz.ru");
    Domain sub2("alg.m.gdz.ru");
    Domain not_sub1("freegdz.ru");
    Domain not_sub2("ru");

    assert(sub1.IsSubdomain(parent));
    assert(sub2.IsSubdomain(parent));
    assert(!not_sub1.IsSubdomain(parent));
    assert(!not_sub2.IsSubdomain(parent));
}

void TestReadDomains() {
    std::istringstream input("gdz.ru\nmaps.me\ncom\n");
    auto domains = ReadDomains(input, 3);
    assert(domains.size() == 3);
    assert(domains[0] == Domain("gdz.ru"));
    assert(domains[1] == Domain("maps.me"));
    assert(domains[2] == Domain("com"));
}

void TestDomainChecker() {
    std::vector<Domain> forbidden_domains = {
        Domain("gdz.ru"),
        Domain("math.gdz.ru"),  // избыточный
        Domain("maps.me")
    };

    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    // Проверка, что избыточные домены удалены
    assert(checker.IsForbidden(Domain("gdz.ru")));  // запрещён напрямую
    assert(checker.IsForbidden(Domain("math.gdz.ru")));  // запрещён как поддомен
    assert(!checker.IsForbidden(Domain("freegdz.ru")));  // не запрещён
    assert(checker.IsForbidden(Domain("maps.me")));  // запрещён
    assert(!checker.IsForbidden(Domain("me")));  // не запрещён
}

void TestEmptyForbiddenList() {
    std::vector<Domain> forbidden_domains;
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
    assert(!checker.IsForbidden(Domain("gdz.ru")));  // Good, так как список пуст
}

void TestTopLevelDomain() {
    std::vector<Domain> forbidden_domains = {Domain("com")};
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
    assert(checker.IsForbidden(Domain("google.com")));  // Bad
    assert(!checker.IsForbidden(Domain("ru")));  // Good
}

void TestIsSubdomain() {
    Domain parent("gdz.ru");
    Domain sub("math.gdz.ru");
    Domain not_sub("freegdz.ru");
    assert(sub.IsSubdomain(parent));
    assert(!not_sub.IsSubdomain(parent));
}

//==============================Test End================================

int main() {
    // const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    // DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    // const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    // for (const Domain& domain : test_domains) {
    //     cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    // }

    TestDomain();
    TestReadDomains();
    TestDomainChecker();
    TestEmptyForbiddenList();
    TestTopLevelDomain();

    cout << "All tests passed!" << endl;
    return 0;
}
