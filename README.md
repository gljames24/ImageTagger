# SimpleImageMetadataEditor
A simple, but powerful image metadata editor.
Group Members: James Garrard and Seth Stephenson

  This application's goal is to make inserting and retrieving image metadata as easy as possible. This requires being able to manage IPTC data. Currently the only supported files will be JPG/JPEG and PNG.

This program relies on the exiv2 library.
Compile with -std=c++11 smile.cpp -o smile -I/usr/local/include -L/usr/local/lib -lexiv2

The program will take in input following this convention: 
smile -[k|C|d|T] -[p|a|r|R] \<metadata\> \<path to file\>
  
The first option is keywords, Creator name, date and time, and Title. The letter 'c' is left for caption to be added in the future. 
The second option is print, append, remove, and Remove all respectively. If no action argument is given, print is assumed.
