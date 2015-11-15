#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

#define D(x) cout << #x << " = " << x << " at " << __LINE__ << endl;
#define LOG(x) cout << x << "\t:" << __LINE__ << endl;

string to_string(int i) {
    stringstream ss;
    ss << i;
    string str = ss.str();
    return str;
}

struct ItemSet {
    set<int> items;

    ItemSet() {
    }

    ItemSet(int it[]) {
        int len = sizeof(it)/sizeof(int);
        for(int i = 0; i < len; i++) {
            items.insert(it[i]);
        }
    }

    void print() const {
        cout << toString();
    }

    string toString() const {
        string out = "(";
        for(auto it : items) {
            if(*(items.cbegin()) != it) {
                out+= " ";
            }
            out += to_string(it);
        }
        out += ")";
        return out;
    }

    bool operator==(const ItemSet &other) const;

    int size() const {
        return items.size();
    }

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

    void clear() {
        this->items.clear();
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

    set<int>::iterator begin()  {
        return items.begin();
    }

    set<int>::iterator end()  {
        return items.end();
    }

    set<int>::const_iterator cbegin() const {
        return items.cbegin();
    }

    set<int>::const_iterator cend() const  {
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

struct AssociationRule {
    ItemSet left, right;
    double confidence;

    AssociationRule(ItemSet left, ItemSet right, double confidence) {
        this->left = left;
        this->right = right;
        this->confidence = confidence;
    }

    string toString() const {
        int confPro = (int)(confidence*100);
        string out = left.toString() + " -> " + right.toString() + " :" + to_string(confPro) + "%";
        return out;
    }
};

struct DB {
    vector<ItemSet> transactions;

    void print() const {
        for(int i = 0; i < transactions.size(); i++) {
            transactions[i].print();
            cout << endl;
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

void read(DB &db) {
    int n;
    cin >> n;

    for(int i = 0; i < n; i++) {
        int k, el;
        ItemSet t;
        cin >> k;
        for(int j = 0; j < k; j++) {
            cin >> el;
            t.items.insert(el);
        }

        db.transactions.push_back(t);
    }
}

unordered_set<ItemSet> getSingleItems(const DB &db) {
    unordered_set<ItemSet> res;

    int itemsArray[1];

    for(int i = 0; i < db.transactions.size(); i++) {
        auto items = db.transactions[i];

        for(auto it = items.cbegin(); it != items.cend(); it++) {
            itemsArray[0] = *it;
            res.insert( ItemSet(itemsArray));
        }
    }
    return res;
}

void print(const unordered_set<ItemSet> &s) {
    for(auto it : s) {
        cout << it.toString() << endl;
    }
    /*
    for(auto it = s.cbegin(); it != s.cend(); it++) {
        cout <<
        it->print();
    }*/
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
        if (c >= SUPPORT * db.size()) {
            res.insert(*it);
        }
    }

    return res;
}


/**
 is - k-element set
 frequent - all frequent (k-1)-element sets
 returns true iff all (k-1) subsets of is are in frequent
*/
bool isFrequentItemSet(const ItemSet &is, const unordered_set<ItemSet> &frequent) {
    int numSubsetsInFrequent = 0;

    for(auto it = frequent.cbegin(); it != frequent.cend(); it++) {
        if(is.isSubset(*it)) {
            numSubsetsInFrequent++;
        }
    }

    return numSubsetsInFrequent == is.size();
}

unordered_set<ItemSet> getNextRankCandidates (
                            const unordered_set<ItemSet> &currRank,
                            const unordered_set<ItemSet> &singleElements) {

    unordered_set<ItemSet> nextRank;
    //LOG("1");
    ItemSet nextRankSet;

    for (auto it = currRank.cbegin(); it != currRank.cend(); it++) {
    //LOG("2")
        for(auto itSingle = singleElements.cbegin(); itSingle != singleElements.cend(); itSingle++) {
            if(!it->isSubset(*itSingle)) {
                nextRankSet = it->setUnion(*itSingle);
                //LOG("t")
                //nextRankSet.print();
                //itSingle->print();
                if(isFrequentItemSet(nextRankSet, currRank)) {
                    nextRank.insert(nextRankSet);
                }
            }
        }
    }

    return nextRank;
}


vector< unordered_set<ItemSet> > getFrequentItemsetsByRank (const DB &db,const double SUPPORT) {

    auto singleItems = getSingleItems(db);

    vector< unordered_set<ItemSet> > ranks;


    auto rank1 = filterCandidates(singleItems, db, SUPPORT);
    ranks.push_back(rank1);

    while(true) {
        auto currRank = getNextRankCandidates(ranks.back(), singleItems);
        currRank = filterCandidates(currRank, db, SUPPORT);
        if(currRank.size() == 0) {
            break;
        }
        ranks.push_back(currRank);
    }

    return ranks;
}

vector<AssociationRule> generateAssociationRules(
                            const vector< unordered_set<ItemSet> > &frequentRank,
                            const DB &db,
                            const double MIN_CONFIDENCE) {

    vector<AssociationRule> rules;
    ItemSet left, right;
    int leftCount, allCount;
    double currConfidence;

    for(auto rankIt = frequentRank.cbegin(); rankIt != frequentRank.cend(); rankIt++) {
        for(auto it = rankIt->cbegin(); it != rankIt->cend(); it++) {


            for (int currSubsetMask = (1 << it->size())-2; currSubsetMask > 0; currSubsetMask--) {
                int idx = 0;

                left.clear();
                right.clear();

                for(auto elIt = it->cbegin();
                        elIt != it->cend();
                        idx++, elIt++) {

                    if((currSubsetMask >> idx) & 1) {
                        left.items.insert(*elIt);
                    } else {
                        right.items.insert(*elIt);
                    }
                }

                leftCount = getCount(left, db);
                allCount = getCount(*it, db); // TODO: move out of for

                currConfidence = (double)allCount / (double)leftCount;
                if(currConfidence >= MIN_CONFIDENCE) {
                    rules.push_back(AssociationRule(left, right, currConfidence));
                }
            }
        }
    }

    return rules;
}

bool associationRuleCompare (const AssociationRule &l, const AssociationRule &r) {
    return l.confidence > r.confidence;
}

void apriori() {
    const double SUPPORT = 2.0 / 9.0;
    const double MIN_CONFIDENCE = .3;

    DB db;
    read(db);

    cout << "Database is: \n";
    db.print();
    cout << endl;

    vector< unordered_set<ItemSet> > frequentRank = getFrequentItemsetsByRank(db, SUPPORT);

    cout << "Frequent tuples are: \n";
    for(auto it : frequentRank) {
        print(it);
    }
    cout << endl;

    vector<AssociationRule> rules = generateAssociationRules(frequentRank, db, MIN_CONFIDENCE);

    sort(rules.begin(), rules.end(), associationRuleCompare);

    cout << "Association rules are: \n";
    for(auto rule : rules) {
        cout << rule.toString() << endl;
    }
    cout << endl;

}

int main () {

    apriori();

    return 0;
}
