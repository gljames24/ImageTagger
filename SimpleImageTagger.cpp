#include <iostream>
#include <fstream>
#include <string>
#include <regex>
using namespace std;

#define sigLen 20
const regex JPEG_SIGNATURE ("ÿØÿ.+"); // This is the "magic" number at the start of every jpg file
const regex PNG_SIGNATURE (".?PNG\n"/*.?\n.?\n\0\0\0.+"*/); // This the PNG signature

bool isJPEG(std::string header);
bool isPNG(std::string header);

int main (){
  std::fstream image;
  std::string imagePath;
  //This is only for testing and should be removed and replaced with CLI arguments
  cout << "Enter file location: ";
  cin >> imagePath;
  cout << "Looking at file in location "<< imagePath << endl;
  image.open(imagePath, ios::in | ios::app);  //The path to your file goes here

  if (image.is_open()){  //Check if image file opened
    char header[sigLen]; //
    image.read(header, sigLen);
    string ext = imagePath.substr(imagePath.find_last_of(".")+1);
    cout << "Header: " << header << " Extension: " << ext << endl;
  	if (isJPEG(header) &&  (ext == "jpg" || ext == "jpeg") ){
		  cout << "This file is a JPEG" << endl;	
  	}
    else if (isPNG(header) && ext == "png"){
		  cout << "This file is a PNG" << endl;	
  	}
  	else{
  		cout << "This file is not a recognized image" << endl;	
  	}
  
  } 
  else cerr << "Your file couldn't be opened" << endl;

  image.close();
  return 0;
}

bool isJPEG(string header){
  return regex_match(header,JPEG_SIGNATURE);
}
bool isPNG(string header){
  return regex_match(header,PNG_SIGNATURE);
}
