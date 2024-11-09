#include "SAIS.h"
#include <iostream>
#include <map>
using namespace std;

SuffixArray sais(const vector<size_t>& text) {
  /* TODO: Implement this! */
	vector<char> s(text.size(), '\0');

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

	for (size_t i = 0; i < text.size(); i++)
		cout << '|' << s.at(i) << '|';
   cout << endl;


	// Step 1: Compute LMS
	for (int i = text.size() - 1; i > 0; i--) {
		if (s.at(i) == 's' && s.at(i - 1) == 'l')
			s.at(i) = '*';
	}

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

  return {};
}
