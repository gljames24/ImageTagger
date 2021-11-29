#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <exiv2/exiv2.hpp>
#include <iomanip>
#include <cassert>
using namespace std;

#define sigLen 4

enum extension{jpeg,png,err};
enum action{Print = 'p', Remove = 'r',Clear = 'R', Append = 'a'};//For action taken on image metadata: Print, Remove, Clear, and Append

struct Image{
	string path;
	fstream raw;
	enum extension ext;
	Exiv2::Image::AutoPtr metadata;
	Exiv2::IptcData iptcData;
	string creator;
	string time;
	string title;
	vector<string> keywords;
};

bool isJPEG(string header, string ext);
bool isPNG(string header, string ext);
void printIPTC(struct Image *image);

int main(int argc, char * const argv[]){
	//Check if there are enough arguments
	if(argc == 1){
		cerr << "No arguments given!" << endl;
		return -1;
	} 
	
  	//Create Image object
	struct Image image;
	image.path = argv[argc-1];
  	
	try {
		Exiv2::XmpParser::initialize();
		::atexit(Exiv2::XmpParser::terminate);
		#ifdef EXV_ENABLE_BMFF
	   		Exiv2::enableBMFF();
		#endif
	
	 	//Open Image path
		image.raw.open(image.path, ios::in | ios::app);  //Open file using user-defined path
		if (image.raw.is_open()){
			//Check file signature and extension
			char header[sigLen];
			image.raw.read(header,sigLen);
			string ext = image.path.substr(image.path.find_last_of(".")+1);
	  		image.ext = isJPEG(header, ext)? jpeg : isPNG(header, ext)? png : err;
		  	switch (image.ext){
		  		case jpeg:
		  			cout << "This file is a JPEG" << endl;
		  			break;
		  		case png:
		  			 cout << "This file is a PNG" << endl;
		  			 break;
		  		default:
		  			cout << "This file is not a recognized image" << endl;
					return -1;
			}
			
			image.metadata = Exiv2::ImageFactory::open(image.path);
  			assert (image.metadata.get() != 0);
			image.metadata->readMetadata();
 
			Exiv2::IptcData &iptcData = image.metadata->iptcData();
			image.iptcData = iptcData;
			
			//Parse user arguments and take appropriate action
			
			//Main argument actions Print, Remove, Clear, and Append
			enum action act = Print;//Print used as default behavior
			int x=1;
			if(regex_match(argv[x],regex("-(p|r|R|a)"))){
				enum action act = static_cast<action>(argv[x][1]);
				cout << "Action Option: " << (char)act << endl;
				x++;
			}
			else{
				printIPTC(&image);
				return 0;
			}
			
			//Metadata options: Keywords, Creator Name, Date and Time, and Title
			char option = 'q';//Unused option to avoid undefined behavior
			
			//Read in arguments until another option is found or until it reaches the last/path argument
			for(;x<argc-1;x++){
				string arg = argv[x];
				if (arg.length() == 2 && arg.at(0) == '-'){
					option = arg.at(1);
				}
				//Take in arguments for selected option
				else{
					switch(option){
						case 'k':{
							cout << "Keyword Entered: " << arg << endl;
                            switch(act){
                                case 'p':{
                                	printIPTC(&image);
                                	return 0;
                                    break;
                                }
                                case 'a':{
                                	cout << "Not yet implemented" << endl;
                                	break;
                                }
                                default:{
                                	cout << "Not yet implemented" << endl;
                               	}
                            }


							break;
						}
						case 'C':{
							cout << "Creator Name Entered: " << arg << endl;
							break;
						}
						case 'd':{
							cout << "Date and Time Entered: " << arg << endl;
							break;
						}
						case 'T':{ 
							cout << "Title Entered: " << arg << endl;
							break;
						}
						default:{
							cout << "invalid option" << endl;
						}
							
					}
				}
			}
		}
		else cerr << "Your file couldn't be opened" << endl;
	}
	catch (Exiv2::AnyError& e) {
    	std::cout << "Caught Exiv2 exception '" << e << "'\n";
    	return -1;
	}
	image.raw.close();
	return 0;
}

bool isJPEG(string header, string ext){
	const char JPEG_SIGNATURE[sigLen] = {-1,-40,-1,-32}; // This is the "magic" number at the start of every jpg file
  return header == JPEG_SIGNATURE && (ext == "jpg" || ext == "jpeg");
}
bool isPNG(string header, string ext){
  return header.substr(1,3) == "PNG" && ext == "png";
}

void printIPTC(struct Image *image){
	 if (!image->iptcData.empty()) {
		Exiv2::IptcData::iterator end = image->iptcData.end();
		for (Exiv2::IptcData::iterator md = image->iptcData.begin(); md != end-1; ++md) {
			std::cout << md->value() << endl;
		}
	}
}
