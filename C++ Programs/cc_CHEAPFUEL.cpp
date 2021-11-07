#include<iostream>
using namespace std;

int main() {
    int t, curr_petrol_price, curr_diesel_price, petrol_inc, diesel_inc, months;
    int petrol_out, diesel_out;
    cin>>t;
    while(t) {
        cin>>curr_petrol_price>>curr_diesel_price>>petrol_inc>>diesel_inc>>months;
        petrol_out = curr_petrol_price + (petrol_inc * months);
        diesel_out = curr_diesel_price + (diesel_inc * months);
        if(petrol_out > diesel_out) {
            cout<<"DIESEL"<<endl;
        }
        else if(petrol_out < diesel_out) {
            cout<<"PETROL"<<endl;
        }
        else {
            cout<<"SAME PRICE"<<endl;
        }
        t--;
    }
    return 0;
}
