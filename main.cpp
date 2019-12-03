#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using namespace std::chrono;

const int N = 16;
const int M = (1<<N);
const int INF = 1e9;
long long sum_of_tours=0;
long long factorial[20];

int n;
int grid[N][N];

int BruteforceTSP()
{
    int u = 0;
    vector<int> vertex;

    for (int i = 0; i < n; i++)
        if (i != u)
            vertex.push_back(i);

    int min_path = INF;

    do {
        int current_pathweight = 0;

        int k = u;

        for (int i = 0; i < vertex.size(); i++) {
            current_pathweight += grid[k][vertex[i]];
            k = vertex[i];
        }
        current_pathweight += grid[k][u];

        sum_of_tours += current_pathweight;

        min_path = min(min_path, current_pathweight);

    } while (next_permutation(vertex.begin(), vertex.end()));
    return min_path;
}

int greedyTSPsolve(const int& start, int u, int cnt)
{
    static bool visited[N];
    if(cnt==n)
    {
        memset(visited,0,sizeof visited);
        return 0;
    }

    visited[u] = 1;

    int mn_weight = INF, mn_idx = start;

    for(int v=0;v<n;v++)
        if(grid[u][v] < mn_weight && visited[v] == 0)
            mn_idx = v, mn_weight = grid[u][v];

    return greedyTSPsolve(start, mn_idx, ++cnt) + grid[u][mn_idx];
}


int greedyTSP()
{
    int start = 0;
    int greedy_ans = greedyTSPsolve(start,0,0);

    for(int i=1;i<n;i++)
    {
        start = i;
        greedy_ans = min(greedy_ans,greedyTSPsolve(start,i,0));
    }

    return greedy_ans;
}

int dp[100000][N+1];
int VISITED_ALL = (1<<n) -1;

int DP_TSP_solve(int mask, int pos)
{
	if(mask==((1<<n) -1)){
		return grid[pos][0];
	}
	if(dp[mask][pos]!=-1){
	   return dp[mask][pos];
	}

	//Now from current node, we will try to go to every other node and take the min ans
	int ans = INF;

	//Visit all the unvisited cities and take the best route
	for(int city=0;city<n;city++){
		if((mask&(1<<city))==0){

			int newAns = grid[pos][city] + DP_TSP_solve(mask|(1<<city), city);
			ans = min(ans, newAns);
		}
	}
	return dp[mask][pos] = ans;
}

int DP_TSP()
{
    memset(dp,-1,sizeof dp);
    return DP_TSP_solve(1,0);
}

void RunSolution()
{
	//Intialize the needed vairables to benchmark
	high_resolution_clock::time_point start;
	high_resolution_clock::time_point end;
	duration<double> TimeElapsed;

    cout.precision(7);

	/* Benchmarking greedyTSP */

	start = high_resolution_clock::now();
    int greedyAns = greedyTSP();
	end = high_resolution_clock::now();
	TimeElapsed = duration_cast<duration<double>>(end - start);
    cout << "The greedy approach to the TSP graph is: " << greedyAns
    << " and it took " << fixed << TimeElapsed.count() << " seconds" << endl;

    /* Benchmarking BruteforceTSP */

    start = high_resolution_clock::now();
    int BruteforceAns = BruteforceTSP();
	end = high_resolution_clock::now();
	TimeElapsed = duration_cast<duration<double>>(end - start);
    cout << "The bruteforce approach to the TSP graph is: " << BruteforceAns
    << " and it took " << fixed << TimeElapsed.count() << " seconds" << endl;

    /* Benchmarking DynamicProgramming Solution*/

    start = high_resolution_clock::now();
    int DPans = DP_TSP();
	end = high_resolution_clock::now();
	TimeElapsed = duration_cast<duration<double>>(end - start);
    cout << "The dynamic programming approach to the TSP graph is: " << DPans
    << " and it took " << fixed << TimeElapsed.count() << " seconds" << endl;

    cout.precision(1);
    return;
}

int main()
{
    freopen("input.in", "r", stdin);

    factorial[1] = 1;
    for(int i=2;i<20;i++)
        factorial[i] = factorial[i-1] * i;
    
    int case_no = 1;

    while(cin >> n)
    {
        cout << "Case #" << case_no++ << ": " << endl;
        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                cin >> grid[i][j];

        RunSolution();

        cout << "The average tour of the TSP graph is: " << (double)sum_of_tours/factorial[n-1] << endl << endl;
    }

}
