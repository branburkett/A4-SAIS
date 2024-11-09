#include "Search.h"
#include <iostream>
#include <string>

using namespace std;

int BinarySearchSuffixArray(const string& pattern, const string& text, const SuffixArray& suffixArr) {
	int left = 0;
	int right = suffixArr.size() - 1;

	while (left <= right) {

		if (pattern.compare("") == 0)
			return 0;

		int mid = (left + right) / 2;
		std::string suffix = text.substr(suffixArr[mid]); // get the suffix starting at suffixArr[mid]
		// Compare pattern with suffix
		if (suffix.compare(0, pattern.size(), pattern) == 0) {
			return mid; // pattern is found at this position in the suffix array
		} else if (suffix < pattern) {
			left = mid + 1; // move search to RHS
		} else {
			right = mid - 1; // move search to LHS
		}
	}

	return -1; // pattern isnt found in suffixArr
}


vector<size_t> KasaiLCP(const string& text, const SuffixArray& suffixArr) {
	size_t n = suffixArr.size();
	vector<size_t> lcp(n, 0);
	vector<size_t> rank(n, 0);

	// build rank arr
	for (size_t i = 0; i < n; i++) {
		rank.at(suffixArr.at(i)) = i;
	}

	// build lcp arr
	size_t h = 0;
	for (size_t i = 0; i < n; i++) {
		if (rank.at(i) == n-1) {
			h = 0;
			continue;
		}
		size_t j = suffixArr.at(rank.at(i) + 1);

		while (i+h < n && j+h < n && text[i+h] == text[j+h])
			h++;

		lcp.at(rank.at(i)) = h;
		if (h > 0)
			h--;

	}

	return lcp;
}

vector<size_t> searchFor(const string& pattern,
                         const string& text,
                         const SuffixArray& suffixArr) {

	vector<size_t> result;

	int index = BinarySearchSuffixArray(pattern, text, suffixArr);

	if (index == -1)
		return result; // no occurrence found

	vector<size_t> lcpArr = KasaiLCP(text, suffixArr);
	size_t len = pattern.length();

	result.push_back(suffixArr.at(index));

	// find occurences to the left of the found index
	for (int i = index - 1; i > 0; i--) {
		if (lcpArr.at(i) >= len) {
			result.push_back(suffixArr.at(i));
		}
		else
			break;
	}

	// find occurences to the right of the found index
	for (size_t i = index + 1; i < suffixArr.size(); i++) {
		if (lcpArr.at(i) >= len) {
			result.push_back(suffixArr.at(i));
		}
		else
			break;
	}


return result;
}
