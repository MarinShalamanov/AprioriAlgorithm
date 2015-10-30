#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
using namespace std;

#define D(x) cout << #x << " = " << x << " at " << __LINE__ << endl;
#define LOG(x) cout << x << "\t:" << __LINE__ << endl;

struct ItemSet {
    unordered_set<int> items;

    ItemSet() {
    }

    ItemSet(int it[]) {
        int len = sizeof(it)/sizeof(int);
        for(int i = 0; i < len; i++) {
            items.insert(it[i]);
        }
    }

    void print() const {
        for(auto it = items.cbegin(); it != items.cend(); it++) {
            cout << *it << " ";
        }
        cout << endl;
    }

    bool operator==(const ItemSet &other) const;

    ItemSet setUnion(const ItemSet &other) const {
        ItemSet res;

        for(auto it = this->cbegin(); it != this->cend(); it++) {
            res.items.insert(*it);
        }
        for(auto it = other.cbegin(); it != other.cend(); it++) {
            res.items.insert(*it);
        }

        return res;
    }

    bool isSubset(const ItemSet &other) const {
        for(auto it = other.cbegin(); it != other.cend(); it++) {
            if(!contains(*it)) {
                return false;
            }
        }

        return true;
    }

    bool contains(int value) const {
        return items.find(value) != items.end();
    }

    unordered_set<int>::iterator begin()  {
        return items.begin();
    }

    unordered_set<int>::iterator end()  {
        return items.end();
    }

    unordered_set<int>::const_iterator cbegin() const {
        return items.cbegin();
    }

    unordered_set<int>::const_iterator cend() const  {
        return items.cend();
    }
};

namespace std {
    template <>
    struct hash<ItemSet> {
        size_t operator()(const ItemSet& k) const {
            long long res = 0;
            for(auto it = k.cbegin(); it != k.cend(); it++) {
                res += *it;
            }

            return hash<long long>()(res);
        }
    };
}

bool ItemSet::operator==(const ItemSet &other) const {
     if(this->items.size() != other.items.size()) {
        return false;
     }

     for(auto it1 = this->cbegin(), it2 = other.cbegin();
            it1 != this->cend();
            it1++, it2++) {

                if(*it1 != *it2) {
                    return false;
                }
     }

     return true;
}

struct DB {
    vector<ItemSet> transactions;

    void print() const {
        for(int i = 0; i < transactions.size(); i++) {
            transactions[i].print();
        }
    }

    int size() const {
        return transactions.size();
    }

    vector<ItemSet>::iterator begin()  {
        return transactions.begin();
    }

    vector<ItemSet>::iterator end()  {
        return transactions.end();
    }

    vector<ItemSet>::const_iterator cbegin() const {
        return transactions.cbegin();
    }

    vector<ItemSet>::const_iterator cend() const  {
        return transactions.cend();
    }
};

DB* read() {
    int n;
    cin >> n;
    DB* res = new DB();
    for(int i = 0; i < n; i++) {
        int k, el;
        ItemSet t;
        cin >> k;
        for(int j = 0; j < k; j++) {
            cin >> el;
            t.items.insert(el);
        }

        res->transactions.push_back(t);
    }

    return res;
}

unordered_set<ItemSet> getSingleItems(DB* db) {
    unordered_set<ItemSet> res;

    int itemsArray[1];

    for(int i = 0; i < db->transactions.size(); i++) {
        auto items = db->transactions[i];

        for(auto it = items.cbegin(); it != items.cend(); it++) {
            itemsArray[0] = *it;
            res.insert( ItemSet(itemsArray));
        }
    }
    return res;
}

void print(unordered_set<ItemSet> s) {
    for(auto it = s.cbegin(); it != s.cend(); it++) {
        it->print();
    }
}

int getCount(const ItemSet &s, const DB &db) {
    int num = 0;

    for(auto it = db.cbegin(); it != db.cend(); it++) {
        if(it->isSubset(s)) {
            num++;
        }
    }

    return num;
}

unordered_set<ItemSet> filterCandidates(
                            const unordered_set<ItemSet> &candidates,
                            const DB &db,
                            const double SUPPORT) {
    unordered_set<ItemSet> res;

    int c;
    for(auto it = candidates.cbegin(); it != candidates.cend(); it++) {
        c = getCount(*it, db);
        D(c)
        if (c >= SUPPORT * db.size()) {
            res.insert(*it);
        }
    }

    return res;
}

unordered_set<ItemSet> getNextRankCandidates(
                            const unordered_set<ItemSet> &currRank,
                            const unordered_set<ItemSet> &singleElements) {

    unordered_set<ItemSet> nextRank;
    for (auto it = currRank.cbegin(); it != currRank.cend(); it++) {

    }
}

void apriori() {
    const double SUPPORT = 5.0 / 9.0;

    DB* db = read();

    auto singleItems = getSingleItems(db);
    print(singleItems);

    singleItems = filterCandidates(singleItems, *db, SUPPORT);
    //LOG("filtered")
    //print(singleItems);
    //db->print();


}

int main () {
    apriori();
    return 0;
}
