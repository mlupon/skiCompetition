#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <unordered_set>
using namespace std;
//struct to store individual lifter info
class lifterNode {
public:
	int start;
	int end;
	int time;
};
//struct to store individual slope info
class slopeNode {
public:
	int start;
	int end;
	double length;
};
//struct to store individual racer info
class racerNode {
public:
	string name;
	int speed;
	int racePoint;
};
//struct to store edge information in the graph data structure. time for lifter and length attribute for slope
class edgeNode {
public:
	int end;
	double time;
	double length;
};
//struct to store final result of BFS.
class resultNode {
public:
	string racer;
	double time;
	int point;
};

class skiCompetition {
private:
	vector<racerNode> _racers;
	vector<lifterNode> _lifters;
	vector<slopeNode> _slopes;
	unordered_map<int, int> _heights;
	unordered_map<int, vector<edgeNode>> _graph;
	void buildGraph() {
		for (lifterNode& lnode : _lifters) {
			_graph[lnode.start].push_back({ lnode.end, (double)lnode.time, 0 });
		}
		for (slopeNode& snode : _slopes) {
			_graph[snode.start].push_back({ snode.end, 0, snode.length });
		}
	}
public:
	//constructor builds the graph with all the given inputs
	skiCompetition(vector<racerNode> racers, vector<lifterNode> lifters, vector<slopeNode> slopes, unordered_map<int, int> heights)
		:_racers(racers),
		_lifters(lifters),
		_slopes(slopes),
		_heights(heights) {
		buildGraph();
	}
	//main function to run the competition
	pair<string, double> runCompetition() {
		auto lambda = [&](resultNode& left, resultNode& right) {
			return left.time > right.time && _heights[left.point] >= _heights[right.point]; };
		priority_queue<resultNode, vector<resultNode>, decltype(lambda)> pq(lambda);
		for (racerNode rn : _racers) {
			resultNode n = bfs(rn);
			cout << n.racer << " " << n.time  << " " << n.point << endl;
			pq.push(n);
		}
		if (!pq.empty()) {
			resultNode fr = pq.top();
			return { fr.racer, fr.time };
		}
		return {};
	}
	//BFS function for each racer to find the min height point they can reach.
	resultNode bfs(racerNode& racerNode) {
		queue<resultNode> q;
		resultNode minNode{ racerNode.name, INT_MAX, racerNode.racePoint };
		q.push(minNode);
		unordered_set<int> visited;
		while (!q.empty()) {
			resultNode currNode = q.front();
			q.pop();
			if (_heights[currNode.point] <= _heights[minNode.point]) {
				minNode = currNode;
			}
			visited.insert(currNode.point);
			for (edgeNode& e : _graph[currNode.point]) {
				if (visited.find(e.end) == visited.end()) {
					double time = currNode.time == INT_MAX ? 0 : currNode.time;
					if (e.time > 0) {
						q.push({ currNode.racer, time + e.time, e.end });
					}
					else {
						q.push({ currNode.racer, time + ((e.length / racerNode.speed) * 60), e.end });
					}
				}
			}
		}
		return minNode;
	}
};

int main() {
	vector<racerNode> racers{ {"Peter", 25, 8}, {"Mary", 20, 3} };
	vector<lifterNode> lifters{
	{ 2, 4, 10 },
	{ 1, 3, 25 },
	{ 6, 5, 15 },
	{ 7, 8, 20 }
	};
	vector<slopeNode> slopes{
		{4, 2, 3.2 },
		{3, 4, 6.9 },
		{4, 1, 1.6 },
		{5, 4, 2.4 },
		{8, 6, 2.2 },
		{8, 7, 3.9 },
		{4, 7, 1.8 }
	};
	unordered_map<int, int> heights{
	{ 1, 6500 },
	{ 2, 6000 },
	{ 3, 9000 },
	{ 4, 7300 },
	{ 5, 8500 },
	{ 6, 7500 },
	{ 7, 7000 },
	{ 8, 8000 }
	};
	
	vector<racerNode> racers{ {"Peter", 25, 1}, {"Mary", 100, 2} };
	vector<lifterNode> lifters{};
	vector<slopeNode> slopes{
		{1, 3, 100 },
		{2, 3, 100 }
	};
	unordered_map<int, int> heights {
	{ 1, 6500 },
	{ 2, 6000 },
	{ 3, 3000 },
	};
	skiCompetition SC(racers, lifters, slopes, heights);
	pair<string, double> result = SC.runCompetition();
	std::cout << result.first << " " << result.second << std::endl;
	std::cin.get();
}