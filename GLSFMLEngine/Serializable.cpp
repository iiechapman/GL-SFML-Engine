//
//  Serializable.cpp
//  GLSFMLEngine
//
//  Created by Evan Chapman on 4/10/13.
//  Copyright (c) 2013 Evan Chapman. All rights reserved.
//

#include "Serializable.h"
using namespace std;

void save(ofstream& file, char* object , int size) {
    file.write((char*)&object, size);
}

void load(ifstream& file , char* object , int size) {
    file.read((char*)&object, size);
}