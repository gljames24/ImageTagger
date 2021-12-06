#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <exiv2/exiv2.hpp>
using namespace std;

#define sigLen 4

enum extension{jpeg,png,err};
enum action{Print = 'p', Remove = 'r', Clear = 'R', Append = 'a'};//For action taken on image metadata: Print, Remove, Clear, and Append

class Image{
	public:
		//Member Variables
		string path;
		fstream raw;
		enum extension ext;
		Exiv2::Image::AutoPtr metadata;
		Exiv2::IptcData iptcData;
		string creator;
		string date;
		string time;
		string title;
		string caption;
		vector<string> keywords;

		//Member Functions
		bool isJPEG(string header, string ext){
			const char JPEG_SIGNATURE[sigLen] = {-1,-40,-1,-32}; // This is the "magic" number at the start of every jpg file
  			return header == JPEG_SIGNATURE && (ext == "jpg" || ext == "jpeg");
		}
		bool isPNG(string header, string ext){
  			return header.substr(1,3) == "PNG" && ext == "png";
		}

	//Image Constructor
	Image(string path){//Couldn't get image to use a constructor
		this->path = path;

			//Open Image path
		raw.open(this->path, ios::in | ios::app);  //Open file using user-defined path
		if (raw.is_open()){
			//Check file signature and extension
			char header[sigLen];
			raw.read(header,sigLen);
			string extS = this->path.substr(this->path.find_last_of(".")+1);
	  		ext = this->isJPEG(header, extS)? jpeg : this->isPNG(header, extS)? png : err;
		  	switch (ext){
		  		case jpeg:
		  			//cout << "This file is a JPEG" << endl;
		  			break;
		  		case png:
		  			 //cout << "This file is a PNG" << endl;
		  			 break;
		  		default:
		  			cerr << "This file is not a recognized image" << endl;
					//return -1;
			}

			metadata = Exiv2::ImageFactory::open(this->path);
  			assert (metadata.get() != 0);
			metadata->readMetadata();

			Exiv2::IptcData &iptcDataR = metadata->iptcData();
			iptcData = iptcDataR;

			creator = iptcData["Iptc.Application2.Byline"].toString();



		}
		else{
			cerr << "Your file couldn't be opened" << endl;
		}
		raw.close();
	}

};


//Main argument actions Print, Remove, Clear, and Append
class Option{
	public:
	virtual void print(Image *image){}
	virtual void append(Image *image,string tags){}
	virtual void remove(Image *image){}
};

//Metadata options: Keywords, Creator Name, Date and Time, and Title
class Keywords: public Option{
	public:
	void print(Image *image){
		if(image->keywords.size()>=1){
			for(string keyword: image->keywords){
				cout << keyword << endl;
			}
		}
	}
	void append(Image *image,string tag){

	}
	void remove(Image *image){

	}
};

class Creator: public Option{
	public:
	void print(Image *image){
		cout << "Creator: " << image->creator << endl;
	}
	void append(Image *image,string tag){
		image->creator=tag;
	}
	void remove(Image *image){
		image->creator = "";
	}
};

class Date: public Option{
	public:
	void print(Image *image){
		cout << "Date: " << image->date << endl;
	}
	void append(Image *image,string tag){
		image->date = tag;
	}
	void remove(Image *image){
		image->date = "";
	}
};

class Time: public Option{
	public:
	void print(Image *image){
		cout << "Time: " << image->time << endl;
	}
	void append(Image *image,string tag){
		image->time = tag;
	}
	void remove(Image *image){
		image->time = "";
	}
};

class Title: public Option{
	public:
	void print(Image *image){
		cout << "Title: " << image->title << endl;
	}
	void append(Image *image,string tag){
		image->title = tag;
	}
	void remove(Image *image){
		image->title = "";
	}
};

class Caption: public Option{
	public:
	void print(Image *image){
		cout << "Caption: " << image->caption << endl;
	}
	void append(Image *image,string tag){
		image->caption = tag;
	}
	void remove(Image *image){
		image->caption = "";
	}
};

int main(int argc, char * const argv[]){
	//Check if there are enough arguments
	if(argc == 1){
		cerr << "No arguments given!" << endl;
		return -1;
	}

	try {
		Exiv2::XmpParser::initialize();
		::atexit(Exiv2::XmpParser::terminate);
		#ifdef EXV_ENABLE_BMFF
	   		Exiv2::enableBMFF();
		#endif

		//Create Image object
		Image image(argv[argc-1]);

		//Iptc Data Type Class Options
		Option *option;
		Keywords keywords;
		Creator creator;
		Date date;
		Time time;
		Title title;
		Caption caption;

		//Keywords and Print used as default behavior
		enum action act = Print;
		option = &keywords;

		//Print keyword metadata by default if there are no options given
		string arg2 = argv[1];
		if(argc == 2 || (argc == 3 && arg2 == "-p") ){
			//cout << "Printing" << endl;
			if (!image.iptcData.empty()) {
				Exiv2::IptcData::iterator end = image.iptcData.end();
				for (Exiv2::IptcData::iterator md = image.iptcData.begin(); md != end-1; ++md) {
					cout << md->key() << ": " << md->value().toString() <</* " Count: " << md->count() <<*/ endl;
				}
			}
			return 0;
		}

		char opt = 'q';//Unused option to avoid undefined behavior
		bool fileChanged= false, verbose= false, k= false, C= false, d= false, t= false, T= false, c = false;
		//Parse user arguments and take appropriate action by reading arguments until another option is found or until it reaches the last/path argument
		for(int x=1;x<argc-1;x++){
			string arg = argv[x];

			//Remove, remove all, or append
			if(regex_match(argv[x],regex("-(p|r|a|v)"))){
				act = static_cast<action>(argv[x][1]);
				//cout << "Action Option: " << (char)act << endl;
				//Main argument actions Print, Remove, Clear, and Append
				switch(act){
					case 'v':{
						verbose = true;
						break;
					}
	               	case 'p':{
	               		if (verbose)
	               		cout << "Action Option: Print" << endl;
	               		option->print(&image);
	                    break;
	                }
	                case 'a':{
	                	if (verbose)
	                   	cout << "Action Option: Append" << endl;
	                   	x++;
	                   	option->append(&image,argv[x]);
	                   	fileChanged = true;
	                   	break;
	                }
	                case 'r':{
	                	if (verbose)
	                   	cout << "Action Option: Remove" << endl;
	                   	x++;
	                   	option->remove(&image);
	                   	fileChanged = true;
	                   	break;
	                }

	                default:{
	                  	cerr << "Action Option: Error, improper action" << endl;
	               	}
	            }
			}
			else if (arg.length() == 2 && arg.at(0) == '-'){
				opt = arg.at(1);
				//Metadata options: Keywords, Creator Name, Date and Time, and Title
				switch(opt){
					case 'k':{
						if (verbose)
						cout << "Keyword Entered" << endl;
						option = &keywords;
						k = true;
						break;
					}
					case 'C':{
						if (verbose)
						cout << "Creator Name Entered" << endl;
						option = &creator;
						C = true;
						break;
					}
					case 'd':{
						if (verbose)
						cout << "Date Entered" << endl;
						option = &date;
						d = true;
						break;
					}
					case 't':{
						if (verbose)
						cout << "Time Entered" << endl;
						option = &time;
						t = true;
						break;
					}
					case 'T':{
						if (verbose)
						cout << "Title Entered" << endl;
						option = &title;
						T = true;
						break;
					}
					case 'c':{
						if (verbose)
						cout << "Caption Entered" << endl;
						option = &caption;
						c = true;
						break;
					}
					default:{
						cout << "invalid option" << endl;
					}
				}
			}
		}

		if (fileChanged){
			//Add member variable to iptcData class
			if(C)
			image.iptcData["Iptc.Application2.Byline"] = image.creator;
			if(d)
			image.iptcData["Iptc.Application2.DateCreated"] = image.date;
			if(t)
			image.iptcData["Iptc.Envelope.TimeSent"] = image.time;
			if(T)
			image.iptcData["Iptc.Application2.Headline"] = image.title;
			if(c)
			image.iptcData["Iptc.Application2.Caption"] = image.caption;

			// Set IPTC data and write it to the file
    		image.metadata->setIptcData(image.iptcData);
    		image.metadata->writeMetadata();
		}
		image.raw.close();
	}
	catch (Exiv2::AnyError& e) {
    	std::cout << "Caught Exiv2 exception '" << e << "'\n";
    	return -1;
	}
	return 0;
}
