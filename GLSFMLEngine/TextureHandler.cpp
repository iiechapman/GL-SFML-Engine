//
//  TextureHandler.cpp
//  GLSFMLEngine
//
//  Created by Evan Chapman on 3/10/13.
//  Copyright (c) 2013 Evan Chapman. All rights reserved.
//

#include "TextureHandler.h"

//Constructor
TextureHandler::TextureHandler() {
    LoadImage( "error.png" );
}

int TextureHandler::Load( const std::string& filename ) {
    //If load image fails, return 0
    if ( LoadImage(filename) == 0 ) {
        return 0;
    }
    
    int x = image[totalImages].getSize().x;
    int y = image[totalImages].getSize().y;
    
    glBindTexture( GL_TEXTURE_2D, totalImages);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,
                 x, y, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, image[totalImages].getPixelsPtr() );
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    
    return totalImages;
}

//Loads image file from disk
int TextureHandler::LoadImage( const std::string& filename ) {
    if ( !image[totalImages].loadFromFile(filename) ) {
        std::cout << "Failed to load file " << filename << "\n";
        return 0;
    }
    std::cout << "Loaded " << filename << "\n";
    totalImages++;
    return 1;
}

/*
 if (!tile[0].loadFromFile("tile1.png")){
 std::cout << "Failed to load texture...\n";
 }else{
 std::cout << "Loaded texture...\n";
 }
 
 //Set up texture 0
 x = textures[0].getSize().x;
 y = textures[0].getSize().y;
 
 glBindTexture(GL_TEXTURE_2D, 0);
 glTexImage2D(GL_TEXTURE_2D, 0,
 GL_RGBA, x, y, 0,
 GL_RGBA, GL_UNSIGNED_BYTE,
 textures[0].getPixelsPtr());
 
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

 */