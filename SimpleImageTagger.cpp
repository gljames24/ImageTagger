#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main (){
  std::fstream image;
  std::string line, someString;
  std::string fileLoc;
  
  cout << "Enter file location: ";
  cin >> fileLoc;

  image.open(fileLoc, ios::in | ios::app);  //The path to your file goes here

  if (image.is_open()){  //You don't have to ask if the file is open but it's more secure
  	if (image.get() == (char)0xFF && image.get() == (char)0xD8 && image.get() == (char)0xFF){
		cout << "This file is a JPG" << endl;	
  	}
  	else{
  		cout << "This file is not a JPG" << endl;	
  	}
  
  } 
  else cerr << "Your file couldn't be opened" << endl;

  image.close();
  return 0;
}
