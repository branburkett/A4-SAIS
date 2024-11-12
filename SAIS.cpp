#include "SAIS.h"
#include <iostream>
#include <map>
#include <algorithm>
using namespace std;

struct BucketType {
	int head;
	int tail;
	int length;
};

SuffixArray sais(const vector<size_t>& text) {

	// Create vec to map 'S' and 'L' types
	vector<char> s(text.size(), '\0');

	// point of this vec is to init temp SA to -1 (we can't do this with the SA array bc its a size_t type)
	// to then use this vec to fill out the SA. When we're done we transfer these elements to the SuffixArray type to return it.
	vector<int> temporarySA(text.size(), -1);

	// Print out text vec to look like arr of elements
	for (size_t i = 0; i < text.size(); i++)
		cout << '|' << text[i] << '|';
	cout << endl;

	// Step 1: Compute 'S' and 'L' type
	s.at(text.size() - 1) = 's'; // init $ to S type
	for (int i = text.size() - 2; i >= 0; i--) {
		if ((int)text.at(i) > (int)text.at(i + 1))
			s.at(i) = 'l';
		else if ((int)text.at(i) < (int)text.at(i + 1))
			s.at(i) = 's';
		else
			s.at(i) = s.at(i + 1);
	}

	// Print out s vec to see 'S' and 'L' types
	for (size_t i = 0; i < s.size(); i++)
		cout << '|' << s.at(i) << '|';
   cout << endl;


	// Step 1: Compute LMS on s vec
	for (int i = text.size() - 1; i > 0; i--) {
		if (s.at(i) == 's' && s.at(i - 1) == 'l')
			s.at(i) = '*';
	}

	// Print out s vec modified with LMS types '*'
	for (size_t i = 0; i < text.size(); i++)
		cout << '|' << s.at(i) << '|';
	cout << endl;

	cin.ignore();

	// Step 2: Induced Sorting
		// Count number of unique char occurences
	map<size_t, size_t> bucketCount;
	for (size_t letter : text)
		bucketCount[letter]++;

	for (const auto& [letter, count] : bucketCount)
		cout << "Letter: " << letter << " appears " << count << " times\n";

		// Reverse pass over the s vector to place lms suffixes (*) into the next free slot at the end of their buckets

		// Create the map of Buckets this will run in parallel with the temp SA vector
	map<size_t, BucketType> buckets;
	int currentIndex = 0;
	for (const auto& [letter, count] : bucketCount) {
		BucketType bucket;
		bucket.head = currentIndex;
		bucket.length = count;
		bucket.tail = bucket.head + bucket.length - 1;

		buckets[letter] = bucket;
		currentIndex += count;
	}

	// Display the results
    for (const auto& [value, bucket] : buckets) {
        std::cout << "Letter " << value << ": head = " << bucket.head 
                  << ", tail = " << bucket.tail 
                  << ", length = " << bucket.length << '\n';
	}

	// Create LMS vector
	vector<int> LMS; // make sure to put the indicies in here not the text value
	for (size_t i = 0; i < s.size(); i++) {
		if (s.at(i) == '*') {
			LMS.push_back(i);
		}
	}


	for (int value : LMS)
		std::cout << value << " ";

	std::cout << std::endl;

// way to sort lms doesnt work
/*
	for (size_t i = 0; i < LMS.size() - 2; i++) {
		vector<size_t> sub1(text.begin() + LMS.at(i), text.begin() + LMS.at(i + 1));
		vector<size_t> sub2(text.begin() + LMS.at(i + 1), text.begin() + LMS.at(i + 2));

		// Lexicographical comparison
		if (std::lexicographical_compare(sub1.begin(), sub1.end(), sub2.begin(), sub2.end())) {
			std::cout << "sub1 comes first alphabetically." << std::endl;
		} else if (std::lexicographical_compare(sub2.begin(), sub2.end(), sub1.begin(), sub1.end())) {
			std::cout << "sub2 comes first alphabetically." << std::endl;
		} else {
			std::cout << "The vectors are equal alphabetically." << std::endl;
		}
	}
*/

	sort(LMS.begin(), LMS.end(), [&](int a, int b) {
		vector<int> sub1(text.begin() + a, text.end());
		vector<int> sub2(text.begin() + b, text.end());

		auto it1 = text.begin() + a;
		auto it2 = text.begin() + b;

		// Compare chars until one vec ends or they differ
		while (it1 != text.end() && it2 != text.end()) {
			if (*it1 != *it2) {
				return *it1 < *it2; // Return true if substring starting at a is lexi smaller
			}
			++it1;
			++it2;
		}
		// If one substring is a prefix of the other, the shorter one should come first
		return (it1 == text.end()) && (it2 != text.end());
	});

	 // Output the sorted lms vector
    std::cout << "Sorted lms vector: ";
    for (int index : LMS) {
        std::cout << index << " ";
    }
    std::cout << std::endl;

	// PASS 1 with sorted LMS substrings into the temp SA vec
	for (int i = LMS.size() - 1; i >= 0; --i) {
		int lmsIndex = LMS[i];
		int letter = text[lmsIndex];

		// Find tail of bucket for letter
		int tail = buckets[letter].tail;

		// Find first available position in the bucket, starting from tail
		while (tail >= buckets[letter].head) {
			if (temporarySA[tail] == -1) {
				temporarySA[tail] = lmsIndex;
				break; // found next available pos now break
			}
			--tail;
		}
	}

	    // Output the temp SA array
    std::cout << "tempSA array after placing LMS indices:\n";
    for (int index : temporarySA) {
        std::cout << index << " ";
    }
    std::cout << std::endl;

	// PASS 2 iterate through SA and do L-to-R Front
	for (size_t i = 0; i < temporarySA.size(); ++i) {
		if (temporarySA[i] == -1)
			continue; // if -1 skip

		int saIndex = temporarySA[i] - 1; // get SA value and sub 1

		if (saIndex < 0 || saIndex >= (int)s.size())
			continue; // check bounds

		if (s[saIndex] == 'l') { // check if L type in s vec
			int letter = text[saIndex]; // get letter from text

			// Find bucket
			int head = buckets[letter].head;
			int tail = buckets[letter].tail;

			// Try to place saIndex in the front of bucket
			while (head <= tail) {
				if (temporarySA[head] == -1) {
					temporarySA[head] = saIndex;
					break; // placed saIndex at front of bucket
				}
				++head;
			}
		}
	}

	    // Output the SA array after processing
    std::cout << "SA array after processing 'L' types:\n";
    for (int index : temporarySA) {
        std::cout << index << " ";
    }
    std::cout << std::endl;

	int times = 0;
	// PASS 3 R-to-L 'S' type to front of bucket
	for (size_t i = temporarySA.size() - 1; i >= 0; --i) {
		if ((int)i <= -1)
			break;
		if (temporarySA[i] == -1)
			continue; // skip -1 values

		std::cout << "tempSA[" << i << "] = " << temporarySA[i] << std::endl;
		times += 1;
		if (times == 23)
			break;


		int saIndex = temporarySA[i] - 1; // get value - 1

		//std::cout << " saIndex " << saIndex;

		if (saIndex < 0 || saIndex >= (int)s.size()) // check bounds
			continue;

		if (s[saIndex] == 'l')
			continue; // skip l types

		int letter = text[saIndex]; // get letter at text

		// find bucket
		int head = buckets[letter].head;
		int tail = buckets[letter].tail;

		while (tail >= head) {
			temporarySA[tail] = saIndex;
			--tail;
		}

		if (tail >= head) {
			buckets[letter].tail = tail;
		}
	}

	    // Output the SA array after processing 'S' or '*' types
    std::cout << "SA array after processing 'S' or '*' types:\n";
    for (int index : temporarySA) {
        std::cout << index << " ";
    }
    std::cout << std::endl;

  return {};
}
