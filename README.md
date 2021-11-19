# SimpleImageMetadataEditor
A simple, but powerful image metadata editor.
Group Members: James Garrard and Seth Stephenson

  This application's goal is to make inserting and retrieving image metadata as easy as possible. This requires being able to manage IPTC data. Currently the only supported files will be JPG/JPEG and PNG.

This program relies on the exiv2 library.
Compile with -std=c++98 smile.cpp -o smile -I/usr/local/include -L/usr/local/lib -lexiv2
