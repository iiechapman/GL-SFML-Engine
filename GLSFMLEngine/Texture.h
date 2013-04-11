//
//  Texture.h
//  GLSFMLEngine
//
//  Created by Evan Chapman on 4/10/13.
//  Copyright (c) 2013 Evan Chapman. All rights reserved.
//

#ifndef __GLSFMLEngine__Texture__
#define __GLSFMLEngine__Texture__

#include <iostream>
#include <string>
#include <OpenGL/OpenGL.h>
#include "TextureHandler.h"
using namespace std;
#endif /* defined(__GLSFMLEngine__Texture__) */


class Texture {
public:
    Texture(string filename);
    Texture();
    
    string filename;
    GLuint textureID;
};