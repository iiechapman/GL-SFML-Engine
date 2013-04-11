//
//  Texture.cpp
//  GLSFMLEngine
//
//  Created by Evan Chapman on 4/10/13.
//  Copyright (c) 2013 Evan Chapman. All rights reserved.
//

#include "Texture.h"
using namespace std;

Texture::Texture(string filename) {
    TextureHandler loader;
    textureID = loader.Load(filename);
}

Texture::Texture(){
    
}