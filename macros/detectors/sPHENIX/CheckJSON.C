#include "/cvmfs/sphenix.sdcc.bnl.gov/gcc-8.3/opt/sphenix/core/rapidjson/include/rapidjson/document.h"
#include "/cvmfs/sphenix.sdcc.bnl.gov/gcc-8.3/opt/sphenix/core/rapidjson/include/rapidjson/ostreamwrapper.h"
#include "/cvmfs/sphenix.sdcc.bnl.gov/gcc-8.3/opt/sphenix/core/rapidjson/include/rapidjson/prettywriter.h"
#include "/cvmfs/sphenix.sdcc.bnl.gov/gcc-8.3/opt/sphenix/core/rapidjson/include/rapidjson/filereadstream.h"
#include "/cvmfs/sphenix.sdcc.bnl.gov/gcc-8.3/opt/sphenix/core/rapidjson/include/rapidjson/error/en.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;

using std::cout;
using std::endl;


int CheckJSON() 
{ 

	std::ifstream ifs("Filelist.txt");

	std::string line;
	while(std::getline(ifs, line)){

		std::cout << "Now Reading File: " << line << std::endl;
		// Open the file 
		std::ifstream fp(line.c_str()); 
		std::string content((std::istreambuf_iterator<char>(fp)), (std::istreambuf_iterator<char>()));
		char readBuffer[65536]; 
 	   


		rapidjson::Document doc; 
//		doc.ParseStream(is); 

		doc.Parse(content.c_str());

		    
		if (doc.HasParseError()) {

			cout << "File: " << line << " IS BAD! Likely Corrupted!" << endl;
	

			cout << "Now I delete: " << line  << endl;


			std::remove(line.c_str()); 
			cout << line << "  is now Deleted" << endl;
		
		}else{

			cout << "File: " << line << " IS Good!" << endl;

		}
/*
		// Read the file into a buffer 
		char readBuffer[65536]; 
		rapidjson::FileReadStream is(fp, readBuffer, 
				sizeof(readBuffer)); 

		// Parse the JSON document 
		rapidjson::Document doc; 
		doc.ParseStream(is); 

		// Close the file 
		fclose(fp); 
*/
		// Use the data in the document 
		// ... 

	}

	return 0; 
}
