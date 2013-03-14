//
//  TextureHandler.h
//  GLSFMLEngine
//
//  Created by Evan Chapman on 3/10/13.
//  Copyright (c) 2013 Evan Chapman. All rights reserved.
//

#ifndef __GLSFMLEngine__TextureHandler__
#define __GLSFMLEngine__TextureHandler__

#include <iostream>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics.hpp>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

/*
===============================================================================
 Texture Handler
 Creates a texture context from file and returns associated Texture ID Number For use by object
 
===============================================================================
 */

class TextureHandler {

public:
    TextureHandler();
    ~TextureHandler();
    int Load( const std::string& filename );
    
private:
    int totalImages;
    
    int LoadImage( const std::string& filename , sf::Image* bufferImage );
};

#endif /* defined(__GLSFMLEngine__TextureHandler__) */