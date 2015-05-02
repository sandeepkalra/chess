#include <locale>
#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <regex>
#include <list>
using namespace std;


struct sKid {
    string section_name;
    string first_name;
    string last_name;
    string middle_name;
    bool has_middle_name;
    string casis_result;
    string lme_result;
    string patsy_result;
	float best;
	sKid(string section, string first,string second, string middle=""){
		section_name = section;
		first_name = first;
		last_name = second;
		if(middle != "") {
			middle_name = middle;
			has_middle_name = true;
		} else has_middle_name  = false;
	}
};

string prj_path          = "./";
string lme_result_file   = prj_path + "lme/results.txt";
string casis_result_file = prj_path + "casis/results.txt";
string patsy_result_file = prj_path + "patsy/results.txt";

string sections[] = {   "k", "1st", "2nd", "3rd", "4th", "5th", "u1600", "u600", "u300", "mon", "tue", "wed", "thu", "fri" };
string master_results     = "results.txt";

vector<sKid> kids;
vector<sKid> semiFinal;
vector<sKid> Final;

vector<string> LME;
vector<string> CASIS;
vector<string> PATSY;
void read_result_file(string file_name, vector<string> &v){
		ifstream file;
		file.open(file_name);

		if(file.is_open()){
			file.seekg(0, file.end);
			size_t sz = file.tellg();;
			file.seekg(0, file.beg);
			char*buffer = new char[sz];
			file.read(buffer,sz);
			for(char *line  = strtok(buffer, "\n"); line!=0; line= strtok(0,"\n")){
				v.push_back(line);
			}

			file.close();
			delete[] buffer;
		} else {
		cout<<"ERR:"<<__LINE__<<"\r\n";
		}
	
}

void init_kids_entry() {
	for(auto section: sections) {
		// open the section file.
		string file_name =  prj_path + section + ".txt";
		ifstream file;
		file.open(file_name);
		if(!file.is_open())
		 {	cout<<"could not open file : " << file_name<<"\r\n"; }
		else {
		vector<string> names;
		file.seekg(0, file.end);
		 size_t sz = file.tellg();
		file.seekg(0, file.beg);
		char *buffer = new char [sz];
		file.read(buffer, sz);
		// Get all names!
		for (char* name = strtok(buffer, "\n"); name != 0; name = strtok(0, "\n")) {
			names.push_back (name);
		}		
		// make list.
		for(auto k: names)
		{
			vector<string> parts;
			char *p = new char[200];
			memset(p,0,200);
			strcpy(p,k.c_str());
			for(char *part = strtok(p, " "); part!=0; part = strtok(0, " ")){
				parts.push_back(part);
			}
			if(parts.size() == 2) {
				kids.push_back(sKid(section, parts[0], parts[1]));
			} else if(parts.size() == 3) {
				kids.push_back(sKid(section, parts[0], parts[1], parts[2]));
			}
			delete p;
		}
		delete[] buffer;
		file.close();
		}
	}
}


// http://stackoverflow.com/questions/3152241/case-insensitive-stdstring-find
// templated version of my_equal so it could work with both char and wchar_t
template<typename charT>
struct my_equal {
    my_equal( const std::locale& loc ) : loc_(loc) {}
    bool operator()(charT ch1, charT ch2) {
        return std::toupper(ch1, loc_) == std::toupper(ch2, loc_);
    }
private:
    const std::locale& loc_;
};

// find substring (case insensitive)
template<typename T>
int ci_find_substr( const T& str1, const T& str2, const std::locale& loc = std::locale() )
{
    typename T::const_iterator it = std::search( str1.begin(), str1.end(), 
        str2.begin(), str2.end(), my_equal<typename T::value_type>(loc) );
    if ( it != str1.end() ) return it - str1.begin();
    else return -1; // not found
}



void locate_results() {
	//for(auto k: kids) 
	for(vector<sKid>::iterator it = kids.begin(); it != kids.end(); ++it)
	{
		sKid k = *it;
		// find his/her name in all 3 files : LME, PATSY and CASIS
		for(auto lme: LME){
			bool found = false;
			if(-1 != ci_find_substr(lme, k.first_name)) {
				if(k.has_middle_name) {
					if(-1 != ci_find_substr(lme, k.middle_name)) {
						found = true;
					}
				} 
				if(!found) {
						if(-1 != ci_find_substr(lme, k.last_name)) {
						found = true;
					}
				}
				if(found) {
					it->lme_result = lme;
				}
			}
		} // LME 

		for(auto lme: PATSY){
			bool found = false;
			if(-1 != ci_find_substr(lme, k.first_name)) {
				if(k.has_middle_name) {
					if(-1 != ci_find_substr(lme, k.middle_name)) {
						found = true;
					}
				} 
				if(!found) {
						if(-1 != ci_find_substr(lme, k.last_name)) {
						found = true;
					}
				}
				if(found) {
					it->patsy_result = lme;
				}
			}
		} // PATSY 

		for(auto lme: CASIS){
			bool found = false;
			if(-1 != ci_find_substr(lme, k.first_name)) {
				if(k.has_middle_name) {
					if(-1 != ci_find_substr(lme, k.middle_name)) {
						found = true;
					}
				} 
				if(!found) {
						if(-1 != ci_find_substr(lme, k.last_name)) {
						found = true;
					}
				}
				if(found) {
					it->casis_result = lme;
				}
			}
		} // CASIS 
	}
}

void find_final_results() {
	regex expression ("\\) (n|[0-9])* (\\d).(\\d)");
	
	for(auto person: semiFinal)
	{
			float best = 0;
			sKid Student(person.first_name, person.last_name, person.middle_name);
			//cout<<"["<<person.section_name<<"] ["<< person.first_name<<" " << person.middle_name <<" "<< person.last_name<<"] ";
			if(person.lme_result!="") {
				smatch match;		
				regex_search(person.lme_result, match, expression);
				string result = string(match[2]) + "." + string(match[3]);
			//	cout<<",LME("<<result<<")";
				float cost = stof(result);
				Student.lme_result = result;
				best = (cost>best)?cost:best;
			}
			if(person.patsy_result != "") {
				smatch match;		
				regex_search(person.patsy_result, match, expression);
				string result = string(match[2]) + "." + string(match[3]);
			//	cout<<",Patsy("<<result<<")";
				float cost = stof(result);
				Student.patsy_result = result;
				best = (cost>best)?cost:best;
			}
			if(person.casis_result != "") {
				smatch match;		
				regex_search(person.casis_result, match, expression);
				string result = string(match[2]) + "." + string(match[3]);
		//		cout<<",Casis("<<result<<")";
				float cost = stof(result);
				Student.casis_result = result;
				best = (cost>best)?cost:best;
			}
			Student.first_name = person.first_name;
			Student.last_name = person.last_name;
			Student.middle_name = person.middle_name;
			Student.section_name = person.section_name;
			Student.best = best;
			Final.push_back(Student);
			//cout<<"\t\t Best("<<best<<")";
			//cout<<"\r\n";
	}
}

//string sections[] = {   "k", "1st", "2nd", "3rd", "4th", "5th", "u1600", "u600", "u300", "mon", "tue", "wed", "thu", "fri" };
struct node {
	float best;
	string first_name;
	string last_name;
	string section_name;
};

map<string , list<node*>> buckets;

void print_pretty_result() 
{
	for(auto s: sections) {
		for(auto kido: Final)
		{
			if(kido.section_name == s) 
			{
				node *n = new node();
				n->best = kido.best;
				n->first_name  = kido.first_name;
				n->last_name = kido.last_name;
				n->section_name = kido.section_name;
				buckets[s].push_back(n);
			}
		}
	}

	for(auto s: sections) {
		auto l = buckets[s];
		l.sort([](node*lhs,node*rhs){ return (rhs->best < lhs->best); });
		cout<<"------ "<<s<<" ------\n";
		for(auto i: l) {
			cout<<"  "<<i->first_name<< " "<< i->last_name<< " ("<< i->best <<") \n";
		}
		cout<<endl;
	}


}

int main() {
    init_kids_entry();
	read_result_file(lme_result_file, LME);
	read_result_file(casis_result_file, CASIS);
	read_result_file(patsy_result_file, PATSY);
	locate_results();
	for(auto i: kids) { 
		if(i.lme_result != ""  || i.patsy_result != "" || i.casis_result!="")
		{
			semiFinal.push_back(i);
		}
	}
	find_final_results();
	cout<<"Total Players:" << semiFinal.size()<<"\r\n";
	print_pretty_result();
}	
