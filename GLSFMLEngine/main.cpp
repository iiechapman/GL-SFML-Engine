#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <iostream>
#include "TextureHandler.h"
#include "GameTypes.h"
#include "Sprite.h"
#include "KeyCodes.h"
#include "Input.h"

/*
 * Basic OpenGL 2d mario engine
 * Evan Chapman 2013
 */

//Debug header
vector<Sprite*> sprite;
Sprite* marioSprite;
Sprite* testSprite;

vector2d_t tempVec;

const int MAX_FPS = 60;
bool PLAYMUSIC = true;
bool DEPTH_ENABLED = false;
bool FULLSCREEN = true;
bool running;
bool falling;
bool jumping;
bool stopped;

//button and sprite variables
//buttons_t       pressed;

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
float   timeOfLastDraw    = 0.0f;
float   deltaDraw         = 0.0f;

int     FPS;
int     framesPerSecond;
int     totalFramesRendered;

range_t leftAnim;
range_t jumpright;
range_t jumpleft;
range_t rightAnim;
range_t stoppedright;
range_t stoppedleft;


sf::Music titleSong;
sf::SoundBuffer hurtSound;
sf::SoundBuffer startSound;
sf::Sound soundPlayer;


void Init();
void Display();
void Reshape( int w,int h );
void Keyboard( unsigned char key,int x, int y );
void KeyboardUp( unsigned char key, int x, int y );
void SpecialKey( int key, int x ,int y );
void SpecialKeyUp( int key,int x ,int y );
void SpriteState();
void SetScene();
void Animate();
void CalculateFPS();
void Timer();
void Idle();
void Debug();
void CalculateDrawTime();

int main ( int argc,char ** argv ) {
    Init();
    
    soundPlayer.setBuffer(startSound);
    
    
    //Create opengl context
    std::cout << "Initializing Opengl...\n";
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition( 900, 300 );
    glutInitWindowSize( 512, 448 );
    glutGameModeString("512x448:32");
    soundPlayer.play();
    if ( !FULLSCREEN ) {
        glutCreateWindow( "Opengl 2D Engine  V1.0" );
    } else {
        glutEnterGameMode();
    }
    glClearColor( 0.0f, 0.37f, 0.71f, 0.0f );
    glutDisplayFunc( Display );
    glutReshapeFunc( Reshape );
    glutIdleFunc( Idle );
    glutKeyboardFunc( Keyboard );
    glutKeyboardUpFunc( KeyboardUp );
    glutSpecialFunc( SpecialKey );
    glutSpecialUpFunc( SpecialKeyUp );
    glutJoystickFunc(Joystick,1);
    //Created opengl Context
    
    //Play music
    if (PLAYMUSIC){
        titleSong.play();
    }
    
    Debug();

    displayList = glGenLists( 1 );
    
    glutMainLoop();
    
    for (vector<Sprite*>::iterator i = sprite.begin() ; i < sprite.end() ; i++ ) {
        delete (*i);
        (*i) = 0;
    }
    
    testSprite = 0;
    marioSprite = 0;
    
	return EXIT_SUCCESS;
}

void Idle() {
    glutForceJoystickFunc();
    
    //if escaped
    if ( pressed.escape ) {
        glutLeaveGameMode();
        soundPlayer.setBuffer(hurtSound);
        soundPlayer.play();
        titleSong.stop();
        exit(0);
        std::cout << "Exit \n";
    }
    
    //if pressed up
    if ( pressed.kup || pressed.jup) {

        tempVec.y = 0.9f;
        
    } else if( pressed.kdown || pressed.jdown) {
        tempVec.y = -0.9f;
        
    } else {
        tempVec.y = 0.0f;
    }
    
    if ( pressed.kleft || pressed.jleft ) {
        tempVec.x = -1.0f;
        sprite[1]->isAnimated = true;
        sprite[1]->SetAnimation(0);

    } else if( pressed.kright || pressed.jright) {
        tempVec.x = 1.0f;
        sprite[1]->isAnimated = true;
        sprite[1]->SetAnimation(1);
    } else {
        tempVec.x = 0.0f;
        sprite[1]->StopAnimation();
        
    }
    
    sprite[1]->SetVelocity(tempVec);

    
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
    
    for (vector<Sprite*>::iterator i = sprite.begin() ; i != sprite.end() ; ++i ){
        (*i)->Update(deltaDraw);
    }
    
    Animate();
    
    SetScene();
    
    CalculateFPS();
    glutPostRedisplay();
    CalculateDrawTime();
}

void Init(){
    // Load a song to play
    if ( !titleSong.openFromFile( "overworld.ogg" ) ) {
        std::cout << "Failed to load song...\n";
    }
    
    if ( !hurtSound.loadFromFile("hurt.wav")) {
        std::cout << "Failed to load sound ... \n";
    }
    
    if (!startSound.loadFromFile("start.wav")) {
        std::cout << "Failed to load sound... \n";
    }
    
}


void Display() {
    glClear( GL_COLOR_BUFFER_BIT );

    glCallList( displayList );
    
    glutSwapBuffers();
    timeOfLastDraw = glutGet( GLUT_ELAPSED_TIME ) * .001;
    totalFramesRendered++;
}

void Reshape(int w, int h) {
    glViewport( 0, 0, (GLsizei)w, (GLsizei)h );
    glMatrixMode( GL_PROJECTION );
    glOrtho( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 );
}


void SetScene() {
    glNewList( displayList, GL_COMPILE );
    
    //set line drawing width for axes
    glLineWidth( 1 );
    
    
    glColor3f( 0.1f, 0.2f, 1.0f );
    glutWireTeapot( 0.5f );
    
    //TERRIBLE 60 frame cap code
    glutWireSphere( 0.5f, 200, 200 );
    
    //start textured drawing
    glEnable( GL_TEXTURE_2D );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    //Start Debug Drawing
    for ( std::vector<Sprite*>::iterator i = sprite.begin() ; i != sprite.end() ; ++i ) {
        (*i)->Draw();
    }
    
    glDisable( GL_TEXTURE_2D );
    glDisable( GL_BLEND );

    if ( DEPTH_ENABLED ) {
        //Set line width for axes
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
    }
    
    glEndList();
}

void CalculateFPS() {
    //std::cout << "FPS: " << FPS << "\n";
    curTime = glutGet( GLUT_ELAPSED_TIME ) * .001;
    
    deltaTime = curTime - lastTime;
    
    if ( deltaTime >= 1.0f ) {
        lastTime = curTime;
        FPS = totalFramesRendered;
        totalFramesRendered = 0;
    }
}

void CalculateDrawTime() {
    deltaDraw = glutGet( GLUT_ELAPSED_TIME ) * .001 - timeOfLastDraw;
    //std::cout << "Draw time: " << deltaDraw << "\n";
}

void Animate(){
    for (vector<Sprite*>::iterator i = sprite.begin() ; i != sprite.end() ; ++i ){
        (*i)->Animate(deltaDraw);
    }
}

void Debug() {
    std::cout << "Debug Section: " << "\n";
    
    vector<int> blankAnimation;
    
    testSprite = new Sprite();
    testSprite->AddAnimation(blankAnimation);
    testSprite->SetAnimation(0);
    testSprite->AddFrame("background1.png", 0);
    testSprite->SetPosition(-1, 1, 0);
    testSprite->SetSize(2, 2, 0);
    sprite.push_back(testSprite);
    
    marioSprite = new Sprite();
    marioSprite->SetSize(.23, .25, 0);
    marioSprite->SetPosition(-0.9f, -0.62f, 0.0f);
    marioSprite->AddAnimation(blankAnimation);
    marioSprite->SetAnimation(0);
    marioSprite->SetAnimationDelay(0.04f);
    marioSprite->AddAnimation(blankAnimation);
    marioSprite->SetAnimation(0);
    marioSprite->AddFrame("marioleft.png", 0);
    marioSprite->AddFrame("marioleft2.png", 0);
    marioSprite->AddFrame("marioleft3.png", 0);
    marioSprite->AddAnimation(blankAnimation);
    marioSprite->AddFrame("marioright.png", 1);
    marioSprite->AddFrame("marioright2.png", 1);
    marioSprite->AddFrame("marioright3.png", 1);

    sprite.push_back(marioSprite);
    
    
    marioSprite = 0;
    
    marioSprite = new Sprite();
    marioSprite->AddAnimation(blankAnimation);
    marioSprite->SetAnimation(0);
    marioSprite->SetAnimationDelay(0.07f);
    marioSprite->AddFrame("block1.png", 0);
    marioSprite->AddFrame("block2.png", 0);
    marioSprite->AddFrame("block3.png", 0);
    marioSprite->AddFrame("block4.png", 0);
    marioSprite->SetPosition(-.2, -.2, 0);
    marioSprite->SetSize(.2, .22, 0.0f);
    
    sprite.push_back(marioSprite);
    

    
    }















