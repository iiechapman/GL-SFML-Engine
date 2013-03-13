//
//  Sprite.cpp
//  GLSFMLEngine
//
//  Created by Evan Chapman on 3/10/13.
//  Copyright (c) 2013 Evan Chapman. All rights reserved.
//

#include "Sprite.h"

/*
 ===================
 Constructor
 ===================
 */
Sprite::Sprite() {
    currentFrame = texture.Load( "error.png" );
    
    std::cout << "CRFRAME: " << currentFrame << "\n";
    position.x = -1;
    position.y = 0;
    position.z = 0.0f;
    size.x = 1.0;
    size.y = 1.4;
    
    R=G=B= 1.0f;
    
}

/*
 ========================
 Draw()
    Draw current frame of animation to quad area
 ========================
 */
void Sprite::Draw() {
    //Select Current Frame of animation texture
    glBindTexture( GL_TEXTURE_2D, currentFrame );
    
    //Begin drawing area
    glBegin( GL_QUADS );
    glColor3f( R, G, B );
    
    glTexCoord2f( 0, 0 );//Bottom Left
    glVertex3f( position.x, position.y, position.z );
    glTexCoord2f( 1, 0 );//Bottom Right
    glVertex3f( position.x + size.x, position.y, position.z );
    glTexCoord2f( 1, 1 );//Top Right
    glVertex3f( position.x + size.x, position.y - size.y,position.z );
    glTexCoord2f( 0, 1 );//Top left
    glVertex3f( position.x, position.y - size.y,position.z );
    glEnd();
}



//Animation
//-----------------------------------------
/*
 ===============
 AddAnimation
 Add chunk of animation frames
 ===============
 */
void Sprite::AddAnimation(vector<int> newAnimation) {
    animation.index.push_back( newAnimation );
}

/*
 Remove animation
 Remove chunk of animation frames
 */
void Sprite::RemoveAnimation(int indexToRemove) {
    animation.index.erase(animation.index.begin() + indexToRemove );
}

/*
 ==========================
 Animate
 Advance frame, if frame is beyond boundaries of current animation, loop animation if animation is looped
 ==========================
 */
void Sprite::Animate() {
    currentFrameIndex++;
    long lastFrame = animation.index[currentAnimation].size();
    
    if (currentFrameIndex > lastFrame) {
        if ( isLooping ) {
            currentFrame = animation.index[currentAnimation][0];
            currentFrameIndex = 0;
        } else {
            currentFrame = (int)lastFrame;
        }
    }
}

















