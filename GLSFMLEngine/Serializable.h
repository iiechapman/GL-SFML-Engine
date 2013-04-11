//
//  Serializable.h
//  GLSFMLEngine
//
//  Created by Evan Chapman on 4/10/13.
//  Copyright (c) 2013 Evan Chapman. All rights reserved.
//

#ifndef __GLSFMLEngine__Serializable__
#define __GLSFMLEngine__Serializable__

#include <iostream>
#include <fstream>
using namespace std;
#endif /* defined(__GLSFMLEngine__Serializable__) */


void save(ofstream& file,char* object , int size);

void load(ofstream& file,char* object, int size);
    
