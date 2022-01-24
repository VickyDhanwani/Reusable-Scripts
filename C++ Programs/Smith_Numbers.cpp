#include<bits/stdc++.h>
using namespace std;


vector <int> primes;
  
void populatePrimes(int MAX) {
    
    bool marked[MAX/2 + 100] = {0};
  
    for (int i=1; i<=(sqrt(MAX)-1)/2; i++) {
        for (int j=(i*(i+1))<<1; j<=MAX/2; j=j+2*i+1)
            marked[j] = true;
    }
    primes.push_back(2);
  
    
    for (int i=1; i<=MAX/2; i++)
        if (marked[i] == false)
            primes.push_back(2*i + 1);
}

int SmithNumbers(int n) {
    int original_number = n;
  
    int pDigitSum = 0;
    for (int i = 0; primes[i] <= n/2; i++)
    {
        while (n % primes[i] == 0)
        {
            
            int p = primes[i];
            n = n/p;
            while (p > 0)
            {
                pDigitSum += (p % 10);
                p = p/10;
            }
        }
    }
  
    
    if (n != 1 && n != original_number)
    {
        while (n > 0)
        {
            pDigitSum = pDigitSum + n%10;
            n = n/10;
        }
    }
  
    int sumDigits = 0;
    while (original_number > 0)
    {
        sumDigits = sumDigits + original_number % 10;
        original_number = original_number/10;
    }
  
    
    if(pDigitSum == sumDigits) {
        return pDigitSum;
    }
    return -1;
}

int main() {
    int n;
    cin>>n;
    populatePrimes(n);
    cout<<SmithNumbers(n);
    return 0;
}
