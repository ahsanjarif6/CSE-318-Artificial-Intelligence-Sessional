#include<bits/stdc++.h>
#define pii pair< int , int >
#define ff first 
#define ss second
#define eps 1e-8
using namespace std;
int n,m;
vector< vector< pii > > g;
//map< pii , int > e;
vector< vector< int > > e;


// type = 1 semi-greedy ,  type = 0 greedy
int semi_greedy_maxcut(set< int >&x , set< int >&y , int type){
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<double> distr(0,1);
	double alpha = distr(eng);
	if(type == 0) alpha = 1.0;
	//cout << setprecision(6) << ' ' << alpha << endl;
	
	
	int wmax = INT_MIN;
	int wmin = INT_MAX;
	/*for(map<pii,int>::iterator it = e.begin();it != e.end();it ++){
		wmax = max(wmax , it->ss);
		wmin = min(wmin , it->ss);
	}*/
	for(int i = 0 ; i < n ; i ++ ){
		for(int j = i ; j < n ; j ++ ){
			wmax = max(wmax , e[i][j]);
			wmin = min(wmin , e[i][j]);
		}
	}
	
	
	double mu = (double)wmin + alpha * (double)(wmax - wmin);
	vector< pii > RCLe;
	/*for(map<pii,int>::iterator it = e.begin();it != e.end();it ++){
		if((double)it->ss >= mu){
			RCLe.push_back(it->ff);
		} 
	}*/
	for(int i = 0 ; i < n ; i ++ ){
		for(int j = i ; j < n ; j ++ ){
			if((double) e[i][j] >= mu){
				RCLe.push_back({i,j});
			}
			/*if(fabs((double)e[i][j] - mu) < eps){
				RCLe.push_back({i,j});
			}*/
		}
	}
	
	set< int > remaining;
	for(int i = 0 ; i < n ; i ++ ){
		remaining.insert(i);
	}
	
	int sz = (int)RCLe.size();
	int ix = (rand() % sz);
	pii edge = RCLe[ix];
	x.insert(edge.ff);
	y.insert(edge.ss);
	int iteration = 0;
	//cout << ix << ' ' << "index chosen" << endl;
	while(x.size() + y.size() != n){
		vector< int > phix(n,0) , phiy(n,0);
		wmax = INT_MIN;
		wmin = INT_MAX;
		/*for(int i = 0 ; i < n ; i ++ ){
			if(x.count(i) == 0 && y.count(i) == 0){
				for(set< int >::iterator it = y.begin();it != y.end();it ++){
					int u = *it;
					if(e.find({i,u}) != e.end()){
						phix[i] += e[{i,u}];
					}
				}
				for(set< int >::iterator it = x.begin();it != x.end();it ++){
					int u = *it;
					if(e.find({i,u}) != e.end()){
						phiy[i] += e[{i,u}];
					}
				}
			}
		}*/
		for(set< int >::iterator it = remaining.begin() ; it != remaining.end() ; it ++ ){
			int i = (*it);
			for(set< int >::iterator it = y.begin();it != y.end();it ++){
				int u = *it;
				phix[i] += e[i][u];
			}
			for(set< int >::iterator it = x.begin();it != x.end();it ++){
				int u = *it;
				phiy[i] += e[i][u];
			}
			wmin = min(wmin , min(phix[i] , phiy[i]));
			wmax = max(wmax , max(phix[i] , phiy[i]));
		}
		/*for(int i = 0 ; i < n ; i ++ ){
			if(x.count(i) == 0 && y.count(i) == 0){
				wmin = min(wmin , min(phix[i] , phiy[i]));
				wmax = max(wmax , max(phix[i] , phiy[i]));
			}
		}*/
		
		mu = (double)wmin + alpha * (double)(wmax - wmin);
		vector< int > RCLv;
		/*for(int i = 0 ; i < n ; i ++ ){
			if(x.count(i) == 0 && y.count(i) == 0){
				if((double)max(phix[i],phiy[i]) >= mu){
					RCLv.push_back(i);
				}
			}
		}*/
		for(set< int >::iterator it = remaining.begin() ; it != remaining.end() ; it ++ ){
			int i = (*it);
			if((double)max(phix[i],phiy[i]) >= mu){
				RCLv.push_back(i);
			}
		}
		sz = (int)RCLv.size();
		ix = rand()%sz;
		int v = RCLv[ix];
		if(phix[v] > phiy[v]){
			//cout << iteration ++ << ' ' << v << endl;
			x.insert(v);
		}
		else{
			//cout << iteration ++ << ' ' << v << endl;
			y.insert(v);
		}
		//cout << v << ' ' << " chosen" << endl;
		remaining.erase(v);
		//cout << x.size() << ' ' << y.size() << endl;
	}
	
	int w = 0;
	for(set< int >::iterator it1 = x.begin();it1 != x.end();it1 ++ ){
		for(set< int >::iterator it2 = y.begin();it2 != y.end();it2 ++ ){
			int u = (*it1);
			int v = (*it2);
			w += e[u][v];
		}
	}
	return w;
}

/*int semi_greedy_maxcut(set< int >&x , set< int >&y){
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<double> distr(0,1);
	double alpha = distr(eng);
	//cout << setprecision(6) << ' ' << alpha << endl;
	
	
	int wmax = INT_MIN;
	int wmin = INT_MAX;
	for(map<pii,int>::iterator it = e.begin();it != e.end();it ++){
		wmax = max(wmax , it->ss);
		wmin = min(wmin , it->ss);
	}
	
	
	double mu = (double)wmin + alpha * (double)(wmax - wmin);
	vector< pii > RCLe;
	for(map<pii,int>::iterator it = e.begin();it != e.end();it ++){
		if((double)it->ss >= mu){
			RCLe.push_back(it->ff);
		} 
	}
	
	int sz = (int)RCLe.size();
	int ix = (rand() % sz);
	pii edge = RCLe[ix];
	x.insert(edge.ff);
	y.insert(edge.ss);
	int iteration = 0;
	while(x.size() + y.size() != n){
		vector< int > phix(n,0) , phiy(n,0);
		for(int i = 0 ; i < n ; i ++ ){
			if(x.count(i) == 0 && y.count(i) == 0){
				for(set< int >::iterator it = y.begin();it != y.end();it ++){
					int u = *it;
					if(e.find({i,u}) != e.end()){
						phix[i] += e[{i,u}];
					}
				}
				for(set< int >::iterator it = x.begin();it != x.end();it ++){
					int u = *it;
					if(e.find({i,u}) != e.end()){
						phiy[i] += e[{i,u}];
					}
				}
			}
		}
		wmax = INT_MIN;
		wmin = INT_MAX;
		for(int i = 0 ; i < n ; i ++ ){
			if(x.count(i) == 0 && y.count(i) == 0){
				wmin = min(wmin , min(phix[i] , phiy[i]));
				wmax = max(wmax , max(phix[i] , phiy[i]));
			}
		}
		mu = (double)wmin + alpha * (double)(wmax - wmin);
		vector< int > RCLv;
		for(int i = 0 ; i < n ; i ++ ){
			if(x.count(i) == 0 && y.count(i) == 0){
				if((double)max(phix[i],phiy[i]) >= mu){
					RCLv.push_back(i);
				}
			}
		}
		sz = (int)RCLv.size();
		ix = rand()%sz;
		int v = RCLv[ix];
		if(phix[v] > phiy[v]){
			//cout << iteration ++ << ' ' << v << endl;
			x.insert(v);
		}
		else{
			//cout << iteration ++ << ' ' << v << endl;
			y.insert(v);
		}
		//cout << x.size() << ' ' << y.size() << endl;
	}
	
	int w = 0;
	for(set< int >::iterator it1 = x.begin();it1 != x.end();it1 ++ ){
		for(set< int >::iterator it2 = y.begin();it2 != y.end();it2 ++ ){
			int u = (*it1);
			int v = (*it2);
			if(e.find({u,v}) != e.end()){
				w += e[{u,v}];
			}
		}
	}
	return w;
}*/


int local_search_maxcut(set< int >&s1 , set< int >&s2){
	int iterations = 0;
	while(true){
		iterations ++ ;
		bool change = false;
		for(int i = 0 ; i < n ; i ++ ){
			if(s1.count(i)){
				int phis1 = 0;
				int phis2 = 0;
				for(set< int >::iterator it = s2.begin();it != s2.end();it ++){
					int u = (*it);
					phis1 += e[i][u];
					/*if(e.find({i,u}) != e.end()){
						phis1 += e[{i,u}];
					}*/
				}
				for(set< int >::iterator it = s1.begin();it != s1.end();it ++){
					int u = (*it);
					phis2 += e[i][u];
					/*if(e.find({i,u}) != e.end()){
						phis2 += e[{i,u}];
					}*/
				}
				if(phis2 - phis1 > 0){
					s1.erase(i);
					s2.insert(i);
					change = true;break;
				}
			}
			else{
				int phis1 = 0;
				int phis2 = 0;
				for(set< int >::iterator it = s2.begin();it != s2.end();it ++){
					int u = (*it);
					phis1 += e[i][u];
					/*if(e.find({i,u}) != e.end()){
						phis1 += e[{i,u}];
					}*/
				}
				for(set< int >::iterator it = s1.begin();it != s1.end();it ++){
					int u = (*it);
					phis2 += e[i][u];
					/*if(e.find({i,u}) != e.end()){
						phis2 += e[{i,u}];
					}*/
				}
				if(phis1 - phis2 > 0){
					s2.erase(i);
					s1.insert(i);
					change = true;break;
				}
			}
		}
		if(!change) break;
	}
	
	/*int w = 0;
	for(set< int >::iterator it1 = s1.begin();it1 != s1.end();it1 ++ ){
		for(set< int >::iterator it2 = s2.begin();it2 != s2.end();it2 ++ ){
			int u = (*it1);
			int v = (*it2);
			if(e.find({u,v}) != e.end()){
				w += e[{u,v}];
			}
		}
	}
	return w;*/
	return iterations;
}


int randomized(set< int >&x , set< int >&y){
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 1);
	
	for(int i = 0 ; i < n ; i ++ ){
		int j = dis(gen);
		//cout << "j = " << j << endl;
		if(!j) x.insert(i);
		else y.insert(i);
	}
	
	int w = 0;
	for(set< int >::iterator it1 = x.begin();it1 != x.end();it1 ++ ){
		for(set< int >::iterator it2 = y.begin();it2 != y.end();it2 ++ ){
			int u = (*it1);
			int v = (*it2);
			w += e[u][v];
		}
	}
	return w;
}


int partial_max_cut(set< int >&x , set< int >&y){
	int w = 0;
	for(set< int >::iterator it1 = x.begin();it1 != x.end();it1 ++ ){
		for(set< int >::iterator it2 = y.begin();it2 != y.end();it2 ++ ){
			int u = (*it1);
			int v = (*it2);
			w += e[u][v];
		}
	}
	return w;
}


/*int greedy_heuristic(set< int >&x , set< int >&y){
	
}*/


int grasp(int max_iter , double& avg_iter , double& avg_best){
	int local_iter = 0;
	int best = 0;
	avg_best = 0;
	for(int i = 0 ; i < max_iter ; i ++ ){
		set< int > x , y;
		semi_greedy_maxcut(x,y,1);
		local_iter += local_search_maxcut(x,y);
		int w = partial_max_cut(x,y);
		avg_best += (double)w;
		cout << "iteration: " << i + 1 << ' ' << w << endl;
		if(w > best) best = w;
	}
	avg_iter = local_iter / (double)max_iter;
	avg_best /= (double)max_iter;
	return best;
}

int main(){
	//set< int > s1 , s2;
	//semi_greedy_maxcut(s1,s2);
	ios::sync_with_stdio(0);
    	cin.tie(0);
	freopen("g33.txt","r",stdin);
	cin >> n >> m;
	if(n < 1) exit(0);
	g = vector< vector< pii > > (n);
	e = vector< vector< int > > (n , vector< int > (n , 0));
	for(int i = 0 ; i < m ; i ++ ){
		int u , v , w;
		cin >> u >> v >> w;
		u -- , v -- ;
		g[u].push_back({v,w});
		g[v].push_back({u,w});
		//e[{u,v}] = e[{v,u}] = w;
		e[u][v] += w;
		e[v][u] += w;
	}
	int iter = 10;
	double avg_rand = 0 , avg_greedy = 0 , avg_semi = 0;
	for(int i = 0 ; i < iter ; i ++ ){
		set< int > x , y;
		int w = randomized(x,y);
		cout << "iteration: " << i + 1 << ' ' << w << endl;
		avg_rand += (double)w;
	}
	avg_rand /= (double)iter;
	for(int i = 0 ; i < iter ; i ++ ){
		set< int > x , y;
		int w = semi_greedy_maxcut(x,y,0);
		cout << "iteration: " << i + 1 << ' ' << w << endl;
		avg_greedy += (double)w;
	}
	avg_greedy /= (double)iter;
	for(int i = 0 ; i < iter ; i ++ ){
		set< int > x , y;
		int w = semi_greedy_maxcut(x,y,1);
		cout << "iteration: " << i + 1 << ' ' << w << endl;
		avg_semi += (double)w;
	}
	avg_semi /= (double)iter;
	double avg_iter = 0.0 , avg_best = 0.0;
	cout << n << ' ' << m << endl;
	cout << fixed;
	cout << setprecision(3);
	cout << avg_rand << ' ' << avg_greedy << ' ' << avg_semi << endl;
	int best = grasp(iter , avg_iter , avg_best);
	cout << avg_iter << ' ' << avg_best << ' ' << iter << ' ' << best << endl;
	//set< int > s1 , s2;
	//cout << greedy_heuristic(s1,s2) << endl;
	//s1.clear();s2.clear();
	//cout << randomized(s1,s2) << endl;
	//s1.clear();s2.clear();
	//cout << semi_greedy_maxcut(s1,s2,1) << endl;
	//s1.clear();s2.clear();
	//cout << semi_greedy_maxcut(s1,s2,0) << endl;
	//s1.clear();s2.clear();
	/*for(auto x : s1) cout << x + 1 << ' ' ;
	cout << '\n';	
	for(auto y : s2) cout << y + 1 << ' ' ;
	cout << '\n';
	cout << local_search_maxcut(s1,s2) << '\n';
	for(auto x : s1) cout << x + 1 << ' ' ;
	cout << '\n';	
	for(auto y : s2) cout << y + 1 << ' ' ;*/
	return 0;
}
