#include<bits/stdc++.h>
#define INF 1e18
#define pii pair< int , int >
#define ff first
#define ss second
#define all(x) x.begin() , x.end()
#define CLASSTYPE 4
#define ATTRTYPE 6
#define ITERCNT 20
using namespace std;

int attVal[] = {4,4,4,3,3,3};

map<string , int> buying , maint , doors , persons , lugboot , safety , classMap;

struct car_data{
    int classification = -1;
    int attrs[6] = {0 , };
};

vector<car_data> dataset , training_dataset , testing_dataset;

struct node{
    int attrType = -1;
    int attrVal = -1;
    bool isLeaf = false;
    int leafVal = -1;
    int setAttrs[ATTRTYPE + 1] = {0, };
    int classCnt[CLASSTYPE + 1] = {0, };
    int pluralityParent = -1;
    vector< node > childList;
};

int findSetCnt(node *tmp){
    int res=0;
    for(int i=0;i<ATTRTYPE;i++){
        if(tmp->setAttrs[i]) res++;
    }
    return res;
}

int findTotalCount(node *tmp){
    int res=0;
    for(int i=1;i<=CLASSTYPE;i++){
        res+=tmp->classCnt[i];
    }
    return res;
}

double calcEntropy(node* root){

    double e = 0.0;
    for(int i = 1 ; i <= CLASSTYPE ; i ++ ){
        double p = (double)root->classCnt[i] / (double)findTotalCount(root);
        if(p > 0.0) e += -p * log2(p);
    }

    return e;
}

int chooseAttr(node* root){
    double rEntropy = calcEntropy(root);
    double totChildEntropy , maxInfo = -INF;

    int choosen = -1;

    for(int i = 0 ; i < ATTRTYPE ; i ++ ){
        if(!root->setAttrs[i]){
            totChildEntropy = 0;
            for(int j = 1 ; j <= attVal[i] ; j ++ ){
                root->setAttrs[i] = j;
                totChildEntropy += calcEntropy(root);
            }
            if(rEntropy - totChildEntropy > maxInfo){
                maxInfo = rEntropy - totChildEntropy;
                choosen = i;
            }
            root->setAttrs[i] = 0;
        }
    }

    return choosen;
}

void decisionTree(node* root){ 
    
    for(int i = 0 ; i < training_dataset.size() ; i ++ ){
        bool ok = true;
        for(int j = 0 ; j < 6 ; j ++ ){
            ok = ok & (root->setAttrs[j] == 0 || training_dataset[i].attrs[j] == root->setAttrs[j]);
        }
        if(ok){
            root->classCnt[training_dataset[i].classification] ++ ;
        }
    }

    if(!findTotalCount(root)){
        root->isLeaf = true;
        root->leafVal = root->pluralityParent;
        return;
    }

    for(int i = 1 ; i <= CLASSTYPE ; i ++ ){
        if(root->classCnt[i] == findTotalCount(root)){
            root->isLeaf = true;
            root->leafVal = i;
            return;
        }
    }

    int choosen = chooseAttr(root);

    node child;

    child.isLeaf = root->isLeaf;
    
    for(int i = 0 ; i <= ATTRTYPE ;i++){
        child.setAttrs[i] = root->setAttrs[i];
    }

    child.attrVal = root->attrVal;
    
    child.attrType = choosen;

    vector< pii > v;
    for(int i = 1 ; i <= CLASSTYPE ; i ++ ){
        v.push_back({root->classCnt[i] , i});
    }
    sort(all(v));
    int plurality = v.back().ss;

    child.pluralityParent = plurality;

    child.leafVal = root->leafVal;

    for(int i = 1 ; i <= attVal[choosen] ; i ++ ){
        child.setAttrs[choosen] = child.attrVal = i;
        root->childList.push_back(child);
    }

    for(int i = 0 ; i < root->childList.size() ; i ++ ){
        decisionTree(&root->childList[i]);
    }
}


bool test(node* root , car_data di){
    if(root->isLeaf){
        return (root->leafVal == di.classification);
    }

    for(int i = 0 ; i < root->childList.size() ; i ++ ){
        if(di.attrs[root->childList[i].attrType] == root->childList[i].attrVal){
            return test(&root->childList[i] , di);
        }
    }

    return false;
}

int randomfunc(int j)
{
    return rand() % j;
}

int main(){
    freopen("car evaluation dataset/car.data" , "r" , stdin);
    freopen("out.txt" , "w" , stdout);

    buying["vhigh"] = 1; buying["high"] = 2; buying["med"] = 3; buying["low"] = 4;
    maint["vhigh"] = 1; maint["high"] = 2; maint["med"] = 3; maint["low"] = 4;
    doors["2"] = 1; doors["3"] = 2; doors["4"] = 3; doors["5more"] = 4;
    persons["2"] = 1; persons["4"] = 2; persons["more"] = 3;
    lugboot["small"] = 1; lugboot["med"] = 2; lugboot["big"] = 3;
    safety["low"] = 1; safety["med"] = 2; safety["high"] = 3;
    classMap["unacc"] = 1; classMap["acc"] = 2; classMap["good"] = 3; classMap["vgood"] = 4;
    
    string s;
    while(cin >> s){
        string token;
        int cnt = 0;
        car_data di;

        for(int j = 0 ; j < s.size() ; j ++ ){
            if(s[j] != ','){
                token += s[j];
            }
            else{
                switch (cnt)
                {
                case 0:
                    di.attrs[0] = buying[token];
                    break;
                case 1:
                    di.attrs[1] = maint[token];
                    break;
                case 2:
                    di.attrs[2] = doors[token];
                    break;
                case 3:
                    di.attrs[3] = persons[token];
                    break;
                case 4:
                    di.attrs[4] = lugboot[token];
                    break;
                case 5:
                    di.attrs[5] = safety[token];
                    break;
                }
                token.clear();
                cnt ++ ;
            }
        }
        di.classification = classMap[token];
        dataset.push_back(di);
    }

    srand(time(0));
    for(double r = 0.01 ; r <= 0.99 ; r += 0.01){
        double meanAccuracy  , variance , standardDev ;
        meanAccuracy = variance = standardDev = 0;

        int dcnt = (int)dataset.size();
        vector< double > accuracyList;
        for(int i = 1 ; i <= ITERCNT ; i ++ ){
            training_dataset.clear();
            testing_dataset.clear();

            random_shuffle(dataset.begin(), dataset.end(), randomfunc);
            int ind = 0;
            while(ind < dcnt*r){
                training_dataset.push_back(dataset[ind]);
                ind++;
            }  
            while(ind < dcnt){
                testing_dataset.push_back(dataset[ind]);
                ind++;
            }       
        
            node root;
            decisionTree(&root);

            int ans = 0;
            for(int i = 0 ; i < testing_dataset.size() ; i ++ ){
                ans += test(&root , testing_dataset[i]);
            }

            double accuracy = (double) ans / (double)(testing_dataset.size());

            accuracyList.push_back(accuracy);
            meanAccuracy += accuracy;
        }

        meanAccuracy = meanAccuracy / accuracyList.size();

        for(int i = 0 ; i < accuracyList.size() ; i ++ ){
            double tmp = (meanAccuracy - accuracyList[i]);
            variance += tmp * tmp;
        }

        variance = variance / accuracyList.size();
        standardDev = sqrt(variance);

        cout << fixed << setprecision(2) << r * 100 << ' ' ;
        cout << fixed << setprecision(4) << meanAccuracy << ' ' ;
        cout << fixed << setprecision(6) << standardDev  << endl;
        //cout<<"Mean Accuracy: "<<fixed<<setprecision(4)<<meanAccuracy*100<<"%"<<endl;
        //cout<<"Standard Deviation of Accuracy: "<<fixed<<setprecision(6)<<standardDev*100<<endl;
    }
    return 0;
}