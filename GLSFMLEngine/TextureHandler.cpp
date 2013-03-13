//
//  TextureHandler.cpp
//  GLSFMLEngine
//
//  Created by Evan Chapman on 3/10/13.
//  Copyright (c) 2013 Evan Chapman. All rights reserved.
//

#include "TextureHandler.h"

/*
 ======================
 Constructor
 Loads error image as 0 texture, in case of loading issues
 ======================
 */
TextureHandler::TextureHandler() {
    sf::Image bufferImage;
    LoadImage( "error.png" ,&bufferImage );
}
/*
 =======================
 Load
 Loads file, then generates texture from file, returns texture ID for use by object
 =======================
 */
int TextureHandler::Load( const std::string& filename ) {
    
    GLuint currentTextureID;
    sf::Image bufferImage;
    
    currentTextureID = LoadImage( filename,&bufferImage );
    //If load image fails, return 0
    if ( currentTextureID == 0 ) {
        return 0;
    }
    
    int x = bufferImage.getSize().x;
    int y = bufferImage.getSize().y;
    
    //Set image to texture in Opengl context
    glBindTexture( GL_TEXTURE_2D, currentTextureID);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,
                 x, y, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, bufferImage.getPixelsPtr() );
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    //Return texture ID used
    std::cout << "CRframe used " << currentTextureID << "\n";
    return currentTextureID;
}

/*
 ==========================
 LoadImage
 Loads image file, saves loaded image to bufferImage and returns texture ID for use by object
 ==========================
 */
int TextureHandler::LoadImage( const std::string& filename , sf::Image* bufferImage ) {
    GLuint textureID;
    
    glGenTextures(1, &textureID);
    
    if ( !bufferImage->loadFromFile( filename ) ) {
        std::cout << "Failed to load file " << filename << "\n";
        return NULL;
    }

    std::cout << "Loaded " << filename << " at index " << textureID <<"\n";
    
    totalImages++;
    //push texture ID up function stack to next Load
    std::cout << "Tex id " << textureID << "\n";
    return textureID;
}
