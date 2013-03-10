#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <iostream>
#include "TextureHandler.h"
#include "GameTypes.h"


/*
 * Basic OpenGL 2d mario engine
 * Evan Chapman 2013
 */

bool PLAYMUSIC = false;
bool running;
bool falling;
bool jumping;
bool stopped;

//button and sprite variables
buttons_t       pressed;
dimensions_t    position;
dimensions_t    deltaPosition;
dimensions_t    size;

//tile variables
dimensions_t tilePos;
dimensions_t tileSize;

//animation variables
float marioAnimTime     = 0.0f;
float animDelayTotal    = 1.00f;
float animTick          = 0.068f;
float animDelta         = 0.0f;
float animLastTime      = 0.0f;


direction_t marioDirection;
range_t     marioAnim;

GLuint displayList;

//time variables
float   delta       = .0025f;
float   lastTime    =   0.0f;
float   curTime     =   0.0f;
float   deltaTime   =   0.0f;

int     FPS;
int     framesPerSecond;
int     totalFramesRendered;

range_t leftAnim;
range_t jumpright;
range_t jumpleft;
range_t rightAnim;
range_t stoppedright;
range_t stoppedleft;

int curFrame;

GLuint textureID = 0;

sf::Image textures[20];
sf::Image tile[20];
sf::Music titleSong;


void Init();
void Display();
void Reshape( int w,int h );
void Keyboard( unsigned char key,int x, int y );
void KeyboardUp( unsigned char key, int x, int y );
void SpecialKey( int key, int x ,int y );
void SpecialKeyUp( int key,int x ,int y );
void TextureLoad();
void SpriteState();
void SetScene();
void Animate();
void CalculateFPS();
void Timer();
void Idle();

int main ( int argc,char ** argv ) {
    Init();
    
    if (PLAYMUSIC){
    titleSong.play();
    }
    
    std::cout << "Initializing Opengl...\n";
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
    glutInitWindowPosition( 900, 300 );
    glutInitWindowSize( 640, 480 );
    glutCreateWindow( "Texture Opengl Test" );
    glClearColor( 0.0f, 0.37f, 0.71f, 0.0f );
    glutDisplayFunc( Display );
    glutReshapeFunc( Reshape );
    glutIdleFunc( Idle );
    glutKeyboardFunc( Keyboard );
    glutKeyboardUpFunc( KeyboardUp );
    glutSpecialFunc( SpecialKey );
    glutSpecialUpFunc( SpecialKeyUp );
    
    TextureLoad();
    displayList = glGenLists( 1 );
    
    glutMainLoop();
    
    glDeleteTextures( 5, &textureID );
    
	return EXIT_SUCCESS;
}

void Idle() {  
    if ( pressed.up ) {
        deltaPosition.y = delta*2;
        falling = false;
        jumping = true;
    } else if( pressed.down ) {
        deltaPosition.y = -delta;
    } else {
        deltaPosition.y = 0.0f;
    }
    
    if ( pressed.left ) {
        marioDirection = left;
        marioAnim = leftAnim;
        running = true;
        stopped = false;
        deltaPosition.x = -delta;

    } else if( pressed.right ) {
        marioDirection = right;
        marioAnim = rightAnim;
        running = true;
        stopped = false;
        
        deltaPosition.x = delta;
    } else {
        running = false;
        stopped = true;
        deltaPosition.x = 0.0f;
    }
    
    
    if ( pressed.plus ) {
        deltaPosition.z = -delta;
    } else if(pressed.minus) {
        deltaPosition.z = delta;
    } else {
        deltaPosition.z = 0.0f;
    }
    
    if ( jumping || falling ) {
        switch ( marioDirection ) {
                case left:
                marioAnim = jumpleft;
                break;
                
            case right:
                marioAnim = jumpright;
                break;
        }
    }
    
    //if player hits bottom of screen , move down
    if ( position.y > -.600 && !jumping ) {
        deltaPosition.y = -delta;
        falling = true;
    } else {
        jumping = false;
        falling = false;
    }
    
    if ( stopped && !falling && !jumping ) {
        switch ( marioDirection ) {
            case left:
                marioAnim = stoppedleft;
                break;
                
            case right:
                marioAnim = stoppedright;
                break;
        }
    }

    position.x += deltaPosition.x;
    position.y += deltaPosition.y;
    position.z += deltaPosition.z;
    
    std::cout   << "x: " << position.x
                << " y: " << position.y
                << " z: " << position.z << "\n";
    
    //Rotate camera using WASD and parentheses
    if ( pressed.rbracket ) {
        glRotatef(1, 0.0f,0.0f, delta/1000);
    }
    
    if ( pressed.lbracket ) {
        glRotatef(1, 0.0f,0.0f, -delta/1000);
    }
    
    if ( pressed.w ) {
        glRotatef(1, -delta/1000,0.0f, 0.0f);
    }
    
    if ( pressed.a ) {
        glRotatef(1, 0.0f,-delta/1000, 0.0f);
    }
    
    if ( pressed.s ) {
        glRotatef(1, delta/1000,0.0f, 0.0f);
    }
    
    if ( pressed.d ) {
        glRotatef(1, 0.0f,delta/1000, 0.0f);
    }
    
    Animate();
    SetScene();
    
    CalculateFPS();
    glutPostRedisplay();
}

void Init(){
    // Load a song to play
    if ( !titleSong.openFromFile( "cielastheme.ogg" ) ) {
        std::cout << "Failed to load song...\n";
    }
    
    leftAnim.start  = 0;
    leftAnim.end    = 2;
    
    rightAnim.start = 3;
    rightAnim.end   = 5;
    
    jumpleft.start  = 6;
    jumpleft.end    = 6;
    
    jumpright.start = 7;
    jumpright.end   = 7;
    
    stoppedleft.start   = 0;
    stoppedleft.end     = 0;
    
    stoppedright.start  = 3;
    stoppedright.end    = 3;
    
    position.x = 0;
    position.y = 0;
    
    tilePos.x = -.50;
    tilePos.y = -0.802;
    
    deltaPosition.x = 0.0f;
    deltaPosition.y = 0.0f;
    
    size.x = .15f;
    size.y = .2f;
    
    tileSize.x = 1.09f;
    tileSize.y = .13f;
    
    marioDirection = right;
    marioAnim      = rightAnim;
    
    running = false;
    jumping = false;
}

void TextureLoad(){
    GLsizei x,y ;
    
    //load mario character texture files
    if ( !textures[0].loadFromFile( "marioleft.png" ) ) {
        std::cout << "Failed to load texture...\n";
    } else {
        std::cout << "Loaded texture...\n";
    }
    
    if ( !textures[1].loadFromFile( "marioleft2.png") ) {
        std::cout << "Failed to load texture...\n";
    } else {
        std::cout << "Loaded texture...\n";
    }
    
    if ( !textures[2].loadFromFile( "marioleft3.png") ) {
        std::cout << "Failed to load texture...\n";
    } else {
        std::cout << "Loaded texture...\n";
    }
    
    if (!textures[3].loadFromFile( "marioright.png" ) ) {
        std::cout << "Failed to load texture...\n";
    } else {
        std::cout << "Loaded texture...\n";
    }

    if (!textures[4].loadFromFile( "marioright2.png" ) ) {
        std::cout << "Failed to load texture...\n";
    } else {
        std::cout << "Loaded texture...\n";
    }
    
    if ( !textures[5].loadFromFile( "marioright3.png" ) ) {
        std::cout << "Failed to load texture...\n";
    } else {
        std::cout << "Loaded texture...\n";
    }
    
    if ( !textures[6].loadFromFile( "mariojumpleft.png" ) ) {
        std::cout << "Failed to load texture...\n";
    } else {
        std::cout << "Loaded texture...\n";
    }
    
    if ( !textures[7].loadFromFile( "mariojumpright.png" ) ) {
        std::cout << "Failed to load texture...\n";
    } else {
        std::cout << "Loaded texture...\n";
    }
    
    if ( !tile[0].loadFromFile( "tile1.png" ) ) {
        std::cout << "Failed to load texture...\n";
    } else {
        std::cout << "Loaded texture...\n";
    }
    
    //Set up texture 0
    x = textures[0].getSize().x;
    y = textures[0].getSize().y;
    
    glBindTexture( GL_TEXTURE_2D, 0 );
    glTexImage2D(
                 GL_TEXTURE_2D, 0,
                 GL_RGBA, x, y, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE,
                 textures[0].getPixelsPtr()
                 );
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    
    //setup texture 1
    x = textures[0].getSize().x;
    y = textures[0].getSize().y;
    
    glBindTexture(GL_TEXTURE_2D, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, textures[1].getPixelsPtr());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    
    //setup texture 2
    x = textures[0].getSize().x;
    y = textures[0].getSize().y;
   
    glBindTexture(GL_TEXTURE_2D, 2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, textures[2].getPixelsPtr());
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    
    //setup texture 3
    x = textures[0].getSize().x;
    y = textures[0].getSize().y;
    
    glBindTexture(GL_TEXTURE_2D, 3);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, textures[3].getPixelsPtr());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    
    //setup texture 4
    x = textures[0].getSize().x;
    y = textures[0].getSize().y;
    
    glBindTexture(GL_TEXTURE_2D, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, textures[4].getPixelsPtr());
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    
    //setup texture 5
    x = textures[0].getSize().x;
    y = textures[0].getSize().y;
    
    glBindTexture(GL_TEXTURE_2D, 5);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, textures[5].getPixelsPtr());
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    
    //setup texture 6
    x = textures[0].getSize().x;
    y = textures[0].getSize().y;
    
    glBindTexture(GL_TEXTURE_2D, 6);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, textures[6].getPixelsPtr());
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    
    //setup texture 7
    x = textures[0].getSize().x;
    y = textures[0].getSize().y;
    
    glBindTexture(GL_TEXTURE_2D, 7);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, textures[7].getPixelsPtr());
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    
    //setup tile texture
    x = tile[0].getSize().x;
    y = tile[0].getSize().y;
    
    glBindTexture(GL_TEXTURE_2D, 8);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, tile[0].getPixelsPtr());
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
}

void Display() {
    glClear( GL_COLOR_BUFFER_BIT );
    
    glCallList( displayList );

    glutSwapBuffers();
    totalFramesRendered++;
}

void Reshape(int w, int h) {
    glViewport( 0, 0, (GLsizei)w, (GLsizei)h );
    glMatrixMode( GL_PROJECTION );
    glOrtho( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 );
}

void Keyboard( unsigned char key,int x, int y ) {
    //std::cout << "Key: " << key << "\n";
    switch ( key ) {
        case '=':
            pressed.plus = true;
            break;
            
        case '-':
            pressed.minus = true;
            break;
            
        case ']':
            pressed.rbracket = true;
            break;
            
        case '[':
            pressed.lbracket = true;
            break;

        case 'w':
            pressed.w = true;
            break;
            
        case 'a':
            pressed.a = true;
            break;
            
        case 's':
            pressed.s = true;
            break;
            
        case 'd':
            pressed.d = true;
            break;
            
        default:
            break;
    }
}

void KeyboardUp( unsigned char key, int x, int y ) {
    switch ( key ) {
        case '=':
            pressed.plus = false;
            break;
            
        case '-':
            pressed.minus = false;
            break;
            
        case ']':
            pressed.rbracket = false;
            break;
            
        case '[':
            pressed.lbracket = false;
            break;
            
        case 'w':
            pressed.w = false;
            break;
            
        case 'a':
            pressed.a = false;
            break;
            
        case 's':
            pressed.s = false;
            break;
            
        case 'd':
            pressed.d = false;
            break;
    }
}

void SpecialKey( int key, int x, int y ) {
    /*
     Special Key codes:
     Up     = 101
     Down   = 103
     Left   = 100
     Right  = 102
    */
    //std::cout << "Special Key: " << key << "\n";
    switch ( key ) {
        case 101:
            pressed.up = true;
            break;
            
        case 103:
            pressed.down = true;
            break;
            
        case 100:
            pressed.left = true;
            break;
            
        case 102:
            pressed.right = true;
            break;
            
        default:
            break;
    }
}

void SpecialKeyUp( int key, int x, int y ) {
    switch ( key ) {
        case 101:
            pressed.up = false;
            break;
            
        case 103:
            pressed.down = false;
            break;
            
        case 100:
            pressed.left = false;
            break;
            
        case 102:
            pressed.right = false;
            break;
            
        default:
            break;
    }
}

void SetScene() {
    glNewList( displayList, GL_COMPILE );
    
    //set line drawing width for axes
    glLineWidth( 1 );
    
    //start textured drawing
    glEnable( GL_TEXTURE_2D );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    //start player drawing
    glBindTexture( GL_TEXTURE_2D, curFrame );
    
    glBegin(GL_QUADS);
    glColor3f( 1.0f, 1.0f, 1.0f );
    glTexCoord2f( 0, 0 );
    glVertex3f( position.x, position.y,position.z );//bl
    glTexCoord2f( 1, 0 );
    glVertex3f( position.x + size.x, position.y,position.z );//br
    glTexCoord2f( 1, 1 );
    glVertex3f( position.x + size.x, position.y - size.y,position.z );//tr
    glTexCoord2f( 0, 1 );
    glVertex3f( position.x, position.y - size.y,position.z );//tl
    glEnd();
    
    //start tile drawing
    
    glBindTexture(GL_TEXTURE_2D, 8);
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0, 0);
    glVertex3f(tilePos.x, tilePos.y,tilePos.z);//bl
    glTexCoord2f(1, 0);
    glVertex3f(tilePos.x + tileSize.x, tilePos.y,tilePos.z);//br
    glTexCoord2f(1, 1);
    glVertex3f(tilePos.x + tileSize.x, tilePos.y - tileSize.y,tilePos.z);//tr
    glTexCoord2f(0, 1);
    glVertex3f(tilePos.x, tilePos.y - tileSize.y,tilePos.z);//tl
    glEnd();
    
    glDisable( GL_TEXTURE_2D );
    glDisable( GL_BLEND );
    
    glColor3f( 0.5f, 0.2f, 1.0f );
    glutWireTeapot( 0.5f );
    
    glutWireSphere( 0.1f, 20, 20 );
    
    glLineWidth( 4 );
    glBegin( GL_LINES );
    
    //X Coordinate line
    glColor3f( 1.0f, 0.0f, 0.0f );
    glVertex3f( 0, 0, 0);
    glVertex3f( 0.8, 0, 0 );
    
    //Y Coordinate line
    glColor3f( 0.0f, 1.0f, 0.0f );
    glVertex3f( 0.0f, 0.0f, 0.0f );
    glVertex3f( 0.0f, 0.8, 0.0 );
    
    //Z Coordinate line
    glColor3f( 0.0f, 0.0f, 1.0f);
    glVertex3f( 0, 0, 0 );
    glVertex3f( 0, 0, 0.8 );
    
    glEnd();
    
    glEndList();
}

void CalculateFPS() {
    //std::cout << "FPS: " << FPS << "\n";
    curTime = glutGet( GLUT_ELAPSED_TIME ) * .001;
    
    deltaTime = curTime - lastTime;
    
    if ( deltaTime >= 1.0f ) {
        lastTime = curTime;
        FPS = totalFramesRendered;
    }
}

void Animate(){
    //Tick from update here, see what frame, loop if needed
    animDelta = curTime - animLastTime;
    //std::cout << "AD = " << curTime << "-" << animLastTime << "=" << curTime - animLastTime << "\n";
    if ( animDelta >= animTick ) {
        animLastTime = curTime;
        if ( running ) {
            curFrame++;
        }
        
        if ( !running ) {
            curFrame = marioAnim.start;
        }
        
        //std::cout << "CurFrame: " << curFrame << "\n";
    }
    
    if ( curFrame > marioAnim.end ) {
        curFrame = marioAnim.start;
    }
    
    if ( curFrame < marioAnim.start ) {
        curFrame = marioAnim.start;
    }
}


















