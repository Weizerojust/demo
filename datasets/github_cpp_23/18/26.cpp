







#include <iostream>

using namespace std;


int *price;
int *max_price;
int *s;

int cut_rod(int n){
    
    int q;
    max_price[0]=0;

    for(int j=1;j<=n;j++){
        q=-1;
        for(int i=1;i<=j;i++){
            
            if(q<price[i]+max_price[j-i]){
                q=price[i]+max_price[j-i];
                s[j]=i;
            }
        }
        max_price[j]=q;
    }
    return max_price[n];
}
int get_count(int array[],int k){
    int n=0;
    while(k>0){
        k=k-s[k];
        n++;
    }
    return n;
}
int main() {
    
    int k,i=1;
    int count=0;
    
    cin>>k;
    
    
    max_price=new int[k+1];
    price=new int[k+1];
    s=new int[k+1];
    
    for(int i=0;i<=k;i++){
        max_price[i]=-1;
    }
    
    
    while(i<=k){
        cin>>price[i];
        i++;
    }
    
    int output=cut_rod(k);
    cout<<output<<endl;
    
    count=get_count(s,k);
    
    cout<<count<<endl;
    
    while(k>0){
        cout<<s[k]<<" ";
        k=k-s[k];
    }
        
    
    return 0;
}
