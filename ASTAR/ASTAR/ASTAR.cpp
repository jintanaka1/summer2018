// ASTAR.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <cmath>
#include <algorithm>
#include <vector>
#include <iostream>
#include <map>
using namespace std;

const int x_range = 6;
const int y_range = 6;
const float res = 0.5;

const int node_count = ((x_range / res) + 1) * ((y_range / res) + 1);

double neighbor_check[8][2] = {
	{ -1, -1 },{ -1, 0 },{ -1, 1 },{ 0, -1 },{ 0, 1 },{ 1, -1 },{ 1, 0 },{ 1, 1 }
};

pair<int, int> position;

vector<int> wall(node_count, 0); //everything in 'wall' 0 for node_count

pair<float, float> current_coord(int N) {
	float x = N % int((x_range / res) + 1);
	float y = N / int((x_range / res) + 1);
	x *= res;
	y *= res;
	return make_pair(x, y);
}

int getNodeN(float x, float y) {//changes coordinate to node

	if (x < 0 || y < 0 || x > x_range || y > y_range) {
		return -1; //when return, function ends
	}

	int N = (x / res) + (y / res) * ((x_range / res) + 1);
	return N;
}

vector<int> astar(int start, int end) {
	vector<int> full_path;
	vector<int> prior_path(node_count);
	prior_path[start] = { start };
	vector<int> open_set = { start };
	vector<int> closed_set;
	vector<double> open_scores;
	vector<double> cost(node_count, INFINITY);
	cost[start] = 0;

	double dist, temp_cost;
	double est_dist;
	vector<double> est_list(node_count);
	vector<double> score(node_count, INFINITY);

	for (int i = 0; i < node_count; i++) {
		est_dist = sqrt(pow(current_coord(i).first - current_coord(end).first, 2) + pow(current_coord(i).second - current_coord(end).second, 2));//this line could slow down the program
		est_list[i] = est_dist;
	}
	score[start] = est_list[start];

	vector<double>::iterator lowest_score;
	int lowest_node, current_node, neighbor_node;
	while (!open_set.empty()) {
		open_scores = {};
		for (unsigned int i = 0; i < open_set.size(); i++) {
			open_scores.push_back(score[open_set[i]]);
		}
		lowest_score = min_element(open_scores.begin(), open_scores.end());
		lowest_node = distance(open_scores.begin(), lowest_score);
		current_node = open_set[lowest_node];

		if (current_node == end) {
			full_path = { current_node };
			while (current_node != start) {
				current_node = prior_path[current_node];
				full_path.push_back(current_node);
			}
			reverse(full_path.begin(), full_path.end());
			return full_path;
		}
		remove(open_set.begin(), open_set.end(), current_node);
		open_set.pop_back();
		closed_set.push_back(current_node);

		vector<int> neighbors = {};
		pair<float, float> new_neighbor;
		for (int i = 0; i < 8; i++) {
			new_neighbor = make_pair(current_coord(current_node).first + neighbor_check[i][0] * res, current_coord(current_node).second + neighbor_check[i][1] * res);
			neighbors.push_back(getNodeN(new_neighbor.first, new_neighbor.second));
			// [(getNodeN(new_neighbor.first, new_neighbor.second))] makes new_neighbor a node instead of a coordinate
			if ((getNodeN(new_neighbor.first, new_neighbor.second)) == -1) {
				neighbors.pop_back();
			}
			//if the new_neighbor node is a wall (or 1) then add it to the closed_set
			else if (wall[getNodeN(new_neighbor.first, new_neighbor.second)] == 1) {
				closed_set.push_back(getNodeN(new_neighbor.first, new_neighbor.second));
			}
		}

		for (unsigned int i = 0; i < neighbors.size(); i++) {
			neighbor_node = neighbors[i];
			if (count(closed_set.begin(), closed_set.end(), neighbor_node) > 0) {
				continue;
			}
			if (count(open_set.begin(), open_set.end(), neighbor_node) == 0) {
				open_set.push_back(neighbor_node);
			}
			dist = sqrt(pow(current_coord(neighbor_node).first - current_coord(current_node).first, 2) + pow(current_coord(neighbor_node).second - current_coord(current_node).second, 2));
			temp_cost = cost[current_node] + dist;
			if (temp_cost >= cost[neighbor_node]) {
				continue;
			}
			prior_path[neighbor_node] = current_node;
			cost[neighbor_node] = temp_cost;
			score[neighbor_node] = cost[neighbor_node] + est_list[neighbor_node];
		}
	}
	return full_path;
}

int main() {
	//0 = ground; 1 = wall
	wall[110] = 1;
	wall[111] = 1;
	wall[112] = 1;
	wall[113] = 1;
	wall[114] = 1;
	wall[101] = 1;
	wall[88] = 1;
	wall[75] = 1;

	vector<int> full_path;
	full_path = astar(150, 86);
	//Iterate through each node in full_path
	for (int i = 0; i < full_path.size(); i++) {
		cout << "Node: " << full_path[i];
		cout << " x: " << current_coord(full_path[i]).first;
		cout << " y: " << current_coord(full_path[i]).second << endl;
	}
}