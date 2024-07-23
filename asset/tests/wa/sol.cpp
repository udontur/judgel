#include <bits/stdc++.h>
#define ll long long
using namespace std;
ll n,m,a[100001],p[100001],ans=1;
int main(){
    cin >> n >> m;
    for(int i=1;i<=n;i++) cin >> a[i],p[i]=p[i-1]+a[i];
    if(m<=n){
        cout << 1 << endl;
        return 0;
    }
    for(int i=1;i<=n;i++){
        for(int j=i;j<=n;j++){
            ans*=p[j]-p[i-1];
            ans%=m;
        }
    }
    cout << ans << endl;
}