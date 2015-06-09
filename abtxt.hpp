#ifndef __ABTXT_H__
#define __ABTXT_H__

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

#include <boost/algorithm/string.hpp>
using namespace boost::algorithm;

template<typename DT>
bool check_line_data(
  const string &str,
	const int num_cols
){
	istringstream in_str(str);
	int num_ele = 0;
	DT tp_val;
	while(in_str >> tp_val){
		num_ele++;
	}
	return (num_ele==num_cols);
}

// data : stored in row-major order.
// num_rows : number of numerical data rows.
// num_cols : number of numerical data column.
// filename : file name of the input text data
// :: this function has the ability to handle :
//  : empty file(the file no numerical data will 
//    be regard as invalid file);
//  : please refer to http://blog.csdn.net/bendanban/article/details/44600429
//    for more details.
template<typename DT>
int read_from_txt(
  vector<DT> &data,
	int &num_rows, int &num_cols, 
	const string &filename)
{
	ifstream in_file; 
	istringstream in_str;
	string line; // the string to store one line.
	DT tp_var;
	int k;
	data.clear();
	in_file.open(filename.c_str(), ios_base::in);
	if (!in_file.is_open()){
		cerr << "failed to open the file of <" << filename << ">" << endl;
		data.clear();
		num_rows = 0;
		num_cols = 0;
		return 1;
	}
	// read the first line with content and check number of column.
	num_cols = 0;
	while (std::getline(in_file, line)){
		trim(line);
		if (line.size()==0)
			continue;
		in_str.str(line);
		in_str.clear();
		while(in_str >> tp_var) {
			num_cols++;
		}
		break;
	}
	if (0==num_cols){ // empty file
		num_rows = 0;
		data.clear();
		return 1;
	}
	
	in_file.clear();
	// read the remaining data and stored into a vector.
	in_file.seekg(ifstream::beg);
	num_rows = 0;
	while(std::getline(in_file, line)){
		trim(line);// remove black characters from the begining and ending of the line.
		if (line.size()==0)
			continue;
		in_str.clear();
		in_str.str(line); // ready to read the data.
		if (check_line_data<DT>(line, num_cols)){
			for (k = 0; k < num_cols; k++){
				in_str >> tp_var;
				data.push_back(tp_var);
			}
			num_rows++;
		}
		else{
			data.clear();
			num_rows = 0;
			num_cols = 0;
			return 2; // different number of lines
		}
	}
	in_file.close();
	return 0;
}


template<typename DT>
void disp_data(vector<DT> &data, const int &num_rows, const int &num_cols)
{
	int index = 0;
	int r,c;
	if ((data.size())!=(num_rows*num_cols)){
		cerr << "[ERROR] error input parameters" << endl;
		return;
	}

	for (r = 0; r < num_rows; r++){
		for (c = 0; c < num_cols; c++){
			cout << data[r*num_cols+c] << "\t";
		}
		cout << endl;
	}
}

#endif

// an example showing blow.
/* ***************************************************
int main(int argc, char **argv){
	if (argc!=2){
		cout << "[Usage ] " << argv[0] << " <file name> " << endl;
		return 1;
	}
	vector<float> data;
	int n_rows, n_cols;
	read_from_txt(data, n_rows, n_cols, argv[1]);
	cout << n_rows << "x" << n_cols << endl;
	disp_data(data, n_rows, n_cols);
	return 0;
}
*****************************************************/
