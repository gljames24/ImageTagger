#include <iostream>
#include <fstream>
#include <string>
using namespace std

int main (){
  std::fstream image;
  std::string line, someString;

  image.open("file.txt", ios::in | ios::app);  //The path to your file goes here

  if (image.is_open()){  //You don't have to ask if the file is open but it's more secure
    while (image.peek){
      
    } 
  } 
  else cerr << "Your file couldn't be opened";

  image.close();
  return 0;
}

