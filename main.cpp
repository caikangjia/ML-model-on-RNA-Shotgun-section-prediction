
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cmath> // pow
#include <cassert>  //assert
#include <utility>  //pair
#include <map>
#include <set>
#include <math.h>
#include "csvstream.h"

using namespace std;
set<string> unique_words(const string &str);
//The class of the .

class Classifier{

    private:

	// input
	// pair<label, word>
	map<string, int> word_to_num_post_ctn; // 3
	map<string, int> label_to_num_post_ctn; // 4
    map<pair<string, string>, int> label_word_to_num_post_ctn; //5
    map<pair<string, string>, double> word_to_p_map; // P(w|C) 0.xxx
	set<string> words; // 2
	int num_trn_post; //1

    public:
	string predict(string content, double &log_prob); //return tag // ln(P(w|C))

    void test_print(){		
        cout << "MAP TEST :\n\n";
        for (auto i : label_word_to_num_post_ctn){
            cout << i.first.first << " : " << i.first.second << " , count = " 
                << i.second << "\n";
        }
        cout << "\n\nSET TEST :\n";
        cout << words.size() << endl;
    } // test aim only. Don't use in final code.

	void debug_print_each_post(ostream &oss, map<string, string> &temp){
		oss << "  label = " << temp["tag"] 
			<< ", content = " << temp["content"] << "\n";
	} // in debug mode, what to print to os.

	void print_train_result(ostream &oss){
		oss << "trained on " << num_trn_post << " examples\n";
	} // in debug mode, what to print to os.

	double log_prior_p(const string &label){
		if (label_to_num_post_ctn.find(label) == label_to_num_post_ctn.end()){
			return 0.00;
		}		
		return log(label_to_num_post_ctn[label] / (num_trn_post + 0.00f));
	}

	double log_likelihhod(const string &label, const string &word){
		pair<string, string> p(label, word);
		map<pair<string, string>, int> &words_map = label_word_to_num_post_ctn;
		if (words.find(word) == words.end()){
			return log(1/(num_trn_post + 0.0f));
		}
		else if (words_map.find(p) == words_map.end()){
			return log(word_to_num_post_ctn[word] / (num_trn_post + 0.0f));
		}
		else{
			return log(words_map[p] / (label_to_num_post_ctn[label] + 0.0f));
		}
	}

	void set_condtnl_prob(){}

	void train_data_store(map<string, string> &input_train){
		map<pair<string, string>, int> &words_map = label_word_to_num_post_ctn;
			//for better viewing experience.
		std::pair<std::string, std::string> p;
			//pair of <label word>
		set<string> tmp_words = unique_words(input_train["content"]);
			//this is the unique set containing all words in a single post.
		string tag = input_train["tag"];
			//the tag of this post. 
		if (label_to_num_post_ctn.find(tag) == label_to_num_post_ctn.end()){
			label_to_num_post_ctn.insert(pair<string, int> (tag, 1));
		} else {++label_to_num_post_ctn[tag];} 
			//to fulfill label_to_num_post_ctn "Label - Num of posts containing this label"
		p.first = tag;
		for (auto i : tmp_words){
			if (word_to_num_post_ctn.find(i) == word_to_num_post_ctn.end()){
				word_to_num_post_ctn.insert(pair<string, int> (i, 1));
			} else {++word_to_num_post_ctn[i];} //3
				//to fulfill word_to_num_post_ctn "Word - Num of posts containing this Word"
			words.insert(i); //2
				//to calculate the unqiue words WITHOUT respect to its tag.
			p.second = i;
				//Now the p = <label, word>
			if (words_map.find(p) == words_map.end()){
				words_map.insert(pair<pair<string, string>, int> (p, 1));
			} else {words_map[p] += 1;} //5
				//calculate the tag-word frequency.
		}
	}

    void read_in(ostream &oss, csvstream &cs, bool debug_mode = false){
        map<string, string> temp; // to hold the post.
		num_trn_post = 0;//init.
		if (debug_mode){
			oss << "training data:\n";
		}
        while (cs >> temp){
            ++num_trn_post; //1
				//count the number of unqiue posts in total.
			train_data_store(temp);
				//all set.
			if (debug_mode){
				debug_print_each_post(oss, temp);
			} // if debug mode, print required content to os.
        }
		print_train_result(oss);
		if (debug_mode)
			debug_mode_sum_print(oss);
		oss << endl;
		return;
    }

    void insert(string label, string word){
		// if not find;
		if (label_word_to_num_post_ctn.find(pair<string, string>(label, word)) 
				== label_word_to_num_post_ctn.end())
			label_word_to_num_post_ctn[pair<string, string>(label, word)] = 1;
		else
			label_word_to_num_post_ctn[pair<string, string>(label, word)] += 1;
    }

	void debug_mode_sum_print(ostream &oss) {
		oss << "vocabulary size = " << words.size() << "\n\n";
		oss << "classes:" << endl;
	// this line reserves for the tag details
		for (auto p : label_to_num_post_ctn){
			oss << "  " << p.first << ", " 
				<< p.second << " examples, log-prior = " << log_prior_p(p.first) << "\n";
		}
		oss << "classifier parameters:" << endl;
	// this line reserves for the words' detail; 
		for (auto p : label_word_to_num_post_ctn){
			oss << "  " << p.first.first << ":" << p.first.second << ", ";
			oss << "count = " << p.second << ", "
				<< "log-likelihood = " << log_likelihhod(p.first.first, p.first.second) << "\n";
		}
		// start new lines
		//oss << endl;
		return;
	}

	void assemble_predict(ostream &oss, csvstream &cs){
		map<string, string> post;
		double log_prob = 0.0f;
		int count = 0, correct = 0;
		string predicted_tag;
		while(cs >> post){
			predicted_tag = predict(post["content"], log_prob);
			oss << "  " << "correct = " << post["tag"] << ", predicted = "
				<< predicted_tag << ", log-probability score = ";
			oss << log_prob << "\n";
			oss << "  content = " << post["content"] << "\n";
			++count;
			if (predicted_tag == post["tag"]){++correct;}
			oss << endl;
		}
		oss << "performance: " << correct << " / " << count << " posts predicted correctly" << endl;
	}

};

set<string> unique_words(const string &str) {
	// Fancy modern C++ and STL way to do it
	istringstream source{ str };
	return { istream_iterator<string>{source},
			istream_iterator<string>{} };
}

void nondebug_mode(csvstream &stream) {
	cout << "training data:" << endl;
	int trained_number = 0;
	map<string, string> map;
	while (stream >> map)
		trained_number++;
	cout << "trained on " << trained_number << " examples" << endl;

}

void prediction() {
	return;
}

int error1() {
	cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
	return -1;
}

int error2(string name) {
	cout << "Error opening file: " << name << endl;
	return -1;
}

string Classifier::predict(string content, double &log_prob) {
	// assume the first label is the most likely one
	// result_loc will be updated later
	int result_loc = 0;
	vector<string> tag_label;
	vector<double> possibility;
	set<string> words_content = unique_words(content);
	// store the tags into the tag label
	for (auto i : label_to_num_post_ctn)
		tag_label.push_back(i.first);
	// for each word, find the possibility of it
	for (int i = 0; i < (int)tag_label.size(); ++i) {
		double predicting = 0;
		string current_tag = tag_label.at(i);
		// 0 can't be denominator
		assert(num_trn_post != 0);
		predicting += log((label_to_num_post_ctn[current_tag] + 0.0f) / num_trn_post);
		// calculate possibility for each words
		for (auto single_word : words_content) {
			predicting += log_likelihhod(current_tag, single_word);
	 	}
		possibility.push_back(predicting);
	}
	// two vector should have the same size
	assert(tag_label.size() == possibility.size());
	for (int i = 0; i < (int)tag_label.size(); ++i) {
		// if current position's possibility is larger than previous max
		// update the value of it
		if (possibility.at(i) > possibility.at(result_loc))
			result_loc = i;
		// if current position has the same possiblity as the previous one
		// and the current one is prior to current location alphabetically
		else if (possibility.at(i) == possibility.at(result_loc)
			&& tag_label.at(i) < tag_label.at(result_loc))
			result_loc = i;
	}
	log_prob = possibility.at(result_loc);
	return tag_label.at(result_loc);
}

int main(int argc, char *argv[]) {
	cout.precision(3);
	if (argc != 3 && argc != 4)
		return error1();
	ifstream train_file(argv[1]);
	if (!train_file.is_open())
		return error2(string(argv[1]));
	csvstream train_stream(train_file);

	Classifier clfer;
	clfer.read_in(std::cout, train_stream, argc == 4);

	ifstream test_file(argv[2]);
	if (!test_file.is_open())
		return error2(string(argv[2]));
	csvstream test_stream(test_file);

	cout << "test data:\n";
	clfer.assemble_predict(cout, test_stream);

	train_file.close();
	test_file.close();
	return 0;
}
// g++ -std=c++11 main.cpp -o main
// ./main train_small.csv

