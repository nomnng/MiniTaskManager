#pragma once
#include <vector>

using namespace std;

struct Utils {
	template<typename T>
		static vector<T> vectorDifference(vector<T> v1, vector<T> v2);
};

template<typename T>
inline vector<T> Utils::vectorDifference(vector<T> v1, vector<T> v2) {
	vector<T> resultVector;
	for (int i = 0; i < v1.size(); i++) {
		bool found = false;
		for (int j = 0; j < v2.size(); j++) {
			if (v1[i] == v2[j]) {
				found = true;
				break;
			}
		}

		if (!found)
			resultVector.push_back(v1[i]);
	}

	return resultVector;
}
