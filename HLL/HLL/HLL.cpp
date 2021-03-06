#include <iostream> 
#include <vector> 
#include <random> 
#include <unordered_set>
#include <cmath>

using namespace std;

int leastSignificantBit(int number, int k)
{
    for (int i = 0; i < 64 - k; i ++ )
    {
        if (number & (1 << i))
        {
            return i;
        }
    }
    return 64-k;
}

void addHLL(hash<int>& hash, int k, int num, vector<int>& buckets)
{
    size_t hash_val = hash(num);

    int bits = log2(numeric_limits<size_t>::max());
    int bucket_idx = hash_val & int(pow(2, k) - 1);
    
    //cout << "Bucket:" <<  bucket_idx << endl;
    int index = leastSignificantBit(hash_val >> k, k);
    
    //cout << "Index: " << index << endl;
    buckets[bucket_idx] = max(buckets[bucket_idx], index);
    //cout << "Register value: " << buckets[bucket_idx] << endl;
}

float harmonicMean(vector<int>& buckets)
{
    float sum = 0;
    for (int i = 0; i < buckets.size(); i++)
    {
        sum = sum + (float)pow(2, -buckets[i]);
    }

    return (float)buckets.size() / sum;
}

int main()
{

    vector<int> dataset;
    unordered_set<int> unique_dataset;
    vector<int> buckets;

    int count = 1000000;
    int k = 4;
    float a_m = .673;
    hash<int> int_hash;

    //const int seed = 3;
    random_device rnd;
    mt19937 engine{ rnd() };
    //engine.seed(seed);
    uniform_int_distribution<int> dist{ 1, count };

    dataset = vector<int>(count);
    buckets = vector<int>(pow(2, k));
    for (int i = 0; i < count; i++)
    {
        int num = dist(engine);
        dataset[i] = num;
        unique_dataset.emplace(num);
        addHLL(int_hash, k, num, buckets);
    }

    cout << "Actual Cardinality: " << unique_dataset.size() << endl;
    float mean = harmonicMean(buckets);
    int estimate = mean * pow(2, k) * a_m;
    cout << "Estimated Cardinality: " << estimate << endl;
    return 0;
}