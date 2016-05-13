#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main() {
	// get file name that contains ALL data points
	string file_name;
	cout << "Enter file path name: ";
	getline(cin, file_name);

	// this is the number of data points that each file will have
	int max_line_count;
	cout << "Enter maximum number of data lines(one data line = pixel line + output line): ";
	cin >> max_line_count;

	// open data points file
	ifstream long_file;
	long_file.open(file_name);

	// check that file opened correctly
	if (!long_file.is_open())
	{
		cout << "Failed to open" << endl;
		cout << "Exiting program.." << endl;
		return 0;
	}

	// check that lines are an even number
	int line_count = 0;
	string line;
	while (getline(long_file, line))
	{
		line_count++;
	}
	long_file.close();
	cout << "Final line count: " << line_count << endl;

	if (line_count % 2 != 0)
	{
		cout << "Wrong number of lines" << endl;
		cout << "Exiting program...." << endl;
		return 0;
	}

	// request CNN input
	int input_size;
	cout << "Enter number of inputs for neural network: ";
	cin >> input_size;

	// start breaking file
	long_file.open(file_name);
	bool available_line = true;
	string partial_file_name = "data_";
	int line_counter = 0; // define outside so it can be used to fix last file
	int file_index = 0; // define outside so it can be used to fix last file
	ofstream partial_file;
	for (file_index = 0; available_line; file_index++)
	{
		// open file
		partial_file.open(partial_file_name + to_string(file_index) + ".data");

		// insert proper header for cnn to be read
		partial_file << max_line_count << " " << input_size << " " << "1" << endl;

		// should read max_line_count * 2 since each data line has one line of pixels and one line for the output bit
		for (line_counter = 0; line_counter < max_line_count * 2 && (available_line = (bool)getline(long_file, line)); line_counter++)
		{
			partial_file << line << endl;
		}

		partial_file.close();
	}

	// fix last file
	/*
	Last file will be marked as having max_line_count lines but it will not
	need to use the last state of line_counter and file_index to fix header
	*/
	file_index--; // to take into account last increase that was done before check
	ifstream file_to_fix;
	file_to_fix.open(partial_file_name + to_string(file_index) + ".data");
	// create file with temp added to name
	// will be renamed later
	partial_file.open(partial_file_name + to_string(file_index) + "_temp.data");
	// write fixed header
	// line_counter is devided by two because you want the number of data points
	// NOT the numeber of text lines
	partial_file << line_counter/2 << " " << input_size << " " << "1" << endl;
	// skip first line with old header
	getline(file_to_fix, line);
	// read through file and copy to new temp
	while (getline(file_to_fix, line))
	{
		partial_file << line << endl;
	}

	// close files
	file_to_fix.close();
	partial_file.close();

	// delete file that data was pulled from
	cout << partial_file_name + to_string(file_index) + ".data" << endl;
	if (remove((partial_file_name + to_string(file_index) + ".data").c_str()) != 0)
	{
		cout << "Error removing last file that needed fixing" << endl;
		cout << "Program exiting" << endl;
		return 0;
	}
	else
	{
		cout << "Last file to be fixed removal succeful" << endl;
	}

	// rename file to standard name (temp -> normal)
	if (rename((partial_file_name + to_string(file_index) + "_temp.data").c_str(), (partial_file_name + to_string(file_index) + ".data").c_str()))
	{
		cout << "Error renaming last that was generated" << endl;
		cout << "Program exiting" << endl;
		return 0;
	}
	else
	{
		cout << "Last file to be fixed renaming succeful" << endl;
	}

	return 0;
}