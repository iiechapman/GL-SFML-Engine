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

#endif /* defined(__GLSFMLEngine__TextureHandler__) */

class TextureHandler {

public:
    TextureHandler();
    int Load( const std::string& filename );
    
private:
    sf::Image bufferImage;
    sf::Image image[1024];
    
    int totalImages;
    
    int LoadImage( const std::string& filename );
};

