#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <map>

using namespace std;

multimap<string, pair <string, string>> openFile(multimap<string, pair<string, string>> dictionary);
void findValuesInMap(multimap<string, pair<string, string>> dictionary);
vector<string> getIfPresentName(multimap<string, pair<string, string>> dictionary, string name);
vector<string> getIfPresentPartOfSpeech(multimap<string, pair<string, string>> dictionary, string name);
vector<string> removeDuplicates(vector<string> unique);

int main() {
	
	multimap<string, pair<string, string>> dictionary;

	cout << "! Opening data file... ./Data.CS.SFSU.txt" << endl;
	dictionary = openFile(dictionary);
	cout << "! Closing data file... ./Data.CS.SFSU.txt" << endl;
	
	cout << endl;
	cout << "----- DICTIONARY 340 C++ -----" << endl;
	cout << endl;

	findValuesInMap(dictionary);

	return 0;
}


multimap<string, pair <string, string>> openFile(multimap<string, pair<string, string>> dictionary) {
	ifstream myfile("Data.CS.SFSU.txt");
	string line;
	string tempString;


	if (!myfile) { //Always test the file open. 
		cout << "Error opening output file" << endl;
		system("pause");
		//return -1;
	}
	cout << endl;
	cout << "! Loading Data..." << endl;

	while (getline(myfile, line)) {
		string key;
		istringstream tokenStream(line);

		string value;
		string name;
		while (getline(tokenStream, value, '|')) {
			istringstream innerStream(value);
			int pos = (int)value.find("=>");
			if (pos == -1) {
				name = value;
			}
			else {
				string partOfspeech = value.substr(0, pos - 2);
				string definition = value.substr(pos + 4);
				dictionary.emplace(name, make_pair(partOfspeech, definition));
			}
		}
	}
	
	cout << "! Loading Complete..." << endl;
	myfile.close();
	return dictionary;
}

void findValuesInMap(multimap<string, pair<string, string>> dictionary) {
	while (true) {
		cout << "Search: " << flush;
		string input;
		getline(cin, input);

		vector<string> arguments;
		string word;
		istringstream iss(input);
		while (iss >> word) arguments.push_back(word);

		string search;
		string search1;
		string search2;

		if (input == "!q" || input == "!Q") {
			cout << "\n-------THANK YOU-------" << endl;
			break;
		}

		if (arguments.size() == 1) {
			search = arguments[0];
			vector<string> values;
			cout << "|" << endl;
			values = getIfPresentName(dictionary, search);

			if (values.empty())
				cout << "<Not Found>" << endl;
			else
				for (string i : values)
					cout << i << endl;
			cout << "|" << endl;
		}

		else if (arguments.size() == 2) {
			search = arguments[0];
			search1 = arguments[1];

			cout << "|" << endl;
			if (search1 == "distinct") {
				vector<string> values;
				vector<string> values2;

				values = getIfPresentName(dictionary, search);
				values2 = removeDuplicates(values);

				if (values2.empty())
					cout << "<Not Found>" << endl;
				else
					for (string i : values2)
						cout << i << endl;
			}
			else {
				search = arguments[0];
				search1 = arguments[1];

				vector<string> values;
				vector<string> values2;
				vector<string> valuesCommon;

				values = getIfPresentName(dictionary, search);
				values2 = getIfPresentPartOfSpeech(dictionary, search1);

				sort(values.begin(), values.end());
				sort(values2.begin(), values2.end());

				set_intersection(values.begin(), values.end(), values2.begin(), values2.end(),
					inserter(valuesCommon, valuesCommon.begin()));

				if (values2.empty())
					cout << "<2nd argument must be a part of speech or \"distinct\">" << endl;
				else if (values.empty())
					cout << "<Not Found>" << endl;
				else
					for (string i : valuesCommon)
						cout << i << endl;
			}
			cout << "|" << endl;
		}
		else if (arguments.size() == 3) {
			search = arguments[0];
			search1 = arguments[1];
			search2 = arguments[2];

			cout << "|" << endl;
			if (search2 == "distinct") {
				vector<string> values;
				vector<string> values2;
				vector<string> valuesCommon;
				vector<string> valuesFinal;

				values = getIfPresentName(dictionary, search);
				values2 = getIfPresentPartOfSpeech(dictionary, search1);

				sort(values.begin(), values.end());
				sort(values2.begin(), values2.end());

				set_intersection(values.begin(), values.end(), values2.begin(), values2.end(),
					inserter(valuesCommon, valuesCommon.begin()));

				valuesFinal = removeDuplicates(valuesCommon);

				if (values2.empty())
					cout << "2nd argument must be a part of speech or \"distinct\" >" << endl;
				else if (values.empty())
					cout << "<Not Found>" << endl;
				else
					for (string i : valuesFinal)
						cout << i << endl;
			}
			else
				cout << "3nd argument must be a part of speech or \"distinct\" >" << endl;
			cout << "|" << endl;
		}
	}
}

vector<string> getIfPresentName(multimap<string, pair<string, string>> dictionary, string name) {
	vector<string> response;
	auto it = dictionary.begin();
	// Iterate through the map
	while (it != dictionary.end()) {
		// Check if value of this entry matches with given key
		if (it->first == name) {
			// Yes found
			// Push the key in given map
			string temp = it->first;
			temp[0] = toupper(temp[0]);
			temp += " [" + it->second.first + "] : ";
			temp += it->second.second;
			//response.push_back(it->first);
			response.push_back(temp);
			
		}
		// Go to next entry in map
		it++;
	}
	return response;
}

vector<string> getIfPresentPartOfSpeech(multimap<string, pair<string, string>> dictionary, string name) {
	vector<string> response;
	auto it = dictionary.begin();
	while (it != dictionary.end()) {
		// Check if value of this entry matches with given value
		if (it->second.first == name) {
			// Yes found
			// Push the key in given map
			string temp = it->first;
			temp[0] = toupper(temp[0]);
			temp += " [" + it->second.first + "] : ";
			temp += it->second.second;
			//response.push_back(it->first);
			response.push_back(temp);
		}
		// Go to next entry in map
		it++;
	}
	return response;
}

vector<string> removeDuplicates(vector<string> unique) {
	for (int i = 0; i < unique.size(); i++) {
		for (int j = i + 1; j < unique.size(); j++) {
			if (unique[i] == (unique[j])) { 
				unique.erase(unique.begin() + j);
				j--;
			}
		}
	}
	return unique;
}
