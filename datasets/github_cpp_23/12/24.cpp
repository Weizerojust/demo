
class Solution {
public:
    
    static bool cmp_cost(const Connection &a, const Connection &b) {
        return a.cost < b.cost;
    }
    static bool cmp_name(const Connection &a, const Connection &b) {
        if (a.city1 != b.city1) {
            return a.city1 < b.city1;
        }
        return a.city2 < b.city2;
    }
    static bool cmp(const Connection &a, const Connection &b) {
        if (a.cost != b.cost) {
            return a.cost < b.cost;
        }
        if (a.city1 != b.city1) {
            return a.city1 < b.city1;
        }
        return a.city2 < b.city2;
    }
     
    unordered_map<string, string> father;
    int treeNum;
     
    vector<Connection> lowestCost(vector<Connection>& connections) {
        
        treeNum = 0;
        
        
        for (auto &C : connections) {
            string city1 = C.city1;
            string city2 = C.city2;
            if (father.find(city1) == father.end()) {
                father[city1] = city1;
                ++treeNum;
            }
            if (father.find(city2) == father.end()) {
                father[city2] = city2;
                ++treeNum;
            }
        }
        
        
        sort(connections.begin(), connections.end(), cmp);
        
        
        
        int costSum = 0;
        vector<Connection> result;
        
        for (int i = 0; i < connections.size(); ++i) {
            string a = connections[i].city1;
            string b = connections[i].city2;
            string fa = find(a);
            string fb = find(b);
            if (fa == fb) {
                continue;
            }
            Connect(a, b);
            result.push_back(connections[i]);
            if (treeNum == 1) {
                break;
            }
        }
        
        
        if (treeNum > 1) {
            result.clear();
            return result;
        }
        
        sort(result.begin(), result.end(), cmp);
        return result;
        
    }
    
private:
    string find(string &x) {
        if (father[x] == x) {
            return x;
        }
        return father[x] = find(father[x]);
    }
    
    void Connect (string &a, string &b) {
        string fa = find(a);
        string fb = find(b);
        if (fa != fb) {
            father[fa] = fb;
            --treeNum;
        }
    }
};
