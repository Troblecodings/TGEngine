#include "Files.h"

namespace nio {

	using namespace std;

	vector<char> getBinarys(char* filepath) {
		ifstream inputstream(filepath, ios::binary | ios::ate);
		size_t size_of_file = inputstream.tellg();
		vector<char> buffer(size_of_file);
		inputstream.seekg(0);
		inputstream.read(buffer.data(), size_of_file);
		inputstream.close();
		return buffer;
	}

	vector<Tag> readXML(char* path) {
		ifstream input(path, ios::binary);
		char indicator;
		TagType tag = NaN;
		Tag tag();
		bool tagtype;
		bool btag;
		vector<char> buffer = {};
		while (!input.eof())
		{
			char args[1];
			input.read(args, 1);
			tagtype = (args[0] == '<');
			btag = (args[0] == '<');
			if (args[0] == '>' || args[0] == '/') btag = false;
			if (args[0] == ' ') {
				cout << buffer.data() << endl;
				buffer.clear();
			}
			if (tagtype) {
				vappend(buffer, args);
			}
		}
	}

	void vappend(vector<char> buffer, char* string) {
		size_t size = buffer.size();
		size_t sz = strlen(string);
		buffer.resize(size + sz);
		for (size_t i = size; i < buffer.size(); i++)
		{
			buffer[i] = string[i - size];
		}
	}
}