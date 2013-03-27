#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundSource.hpp>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <iostream>
#include <cmath>

#include "TextureHandler.h"
#include "GameTypes.h"
#include "Sprite.h"
#include "KeyCodes.h"
#include "Input.h"

using namespace std;

/*
 * Basic OpenGL 2d mario engine
 * Evan Chapman 2013
 */

//Debug header
vector<Sprite*> sprite;
Sprite* marioSprite;
Sprite* testSprite;
Sprite* blockSprite;
Sprite* mario;

vector2d_t tempVec;
vector2d_t secondVec;

GLuint totalTextures;

const int MAX_FPS = 60;
bool PLAYMUSIC = true;
bool DEPTH_ENABLED = false;
bool FULLSCREEN = true;
string windowTitle = "Super Mario Open - By Evan Chapman";
string title = "Super Mario Open - By Evan Chapman";
bool running;
bool falling;
bool jumping;
bool stopped;
bool didCollide;
bool allowJump = false;

dimensions_t    position;
dimensions_t    deltaPosition;
dimensions_t    size;

//Window constants
const float WINDOWWIDTH     = 1024;
const float WINDOWHEIGHT    = 768;
const float WINDOWPOSX      = 300;
const float WINDOWPOSY      = 300;
const float ASPECTRATIO = WINDOWWIDTH / WINDOWHEIGHT;

//Camera variables
const float turnAngle = 100;
const float turnAmount = 10;
float CAMERAOFFSET = 0.0f;


//tile variables
dimensions_t tilePos;
dimensions_t tileSize;

//animation vars
float marioAnimTime     = 0.0f;
float animDelayTotal    = 1.00f;
float animTick          = 0.068f;
float animDelta         = 0.0f;
float animLastTime      = 0.0f;

//time vars
unsigned long long   delta   = 0.002f;
float   lastTime          =  0.0f;
float   curTime           =  0.0f;
float   deltaTime         =  0.0f;
float   timeOfLastDraw    =  0.0f;
float   deltaDraw         =  0.0f;

//game constants
const float BACKGROUNDWIDTH     =   2.0f;
const float BACKGROUNDHEIGHT    =   2.0f;
const float BACKGROUNDX         =  -1.0f;
const float BACKGROUNDY         =   1.0f;
const float SCROLLRIGHT         =   .06f;
const float SCROLLLEFT          =   -0.3;

const float MARIOX          = -.5;
const float MARIOY          = -.62;
const float MARIOWIDTH      = .13;
const float MARIOHEIGHT     = .15;
const float MARIOANIMDELAY  = .04;

const float MARIOFRICTIONX  = 1.8f;

const float MARIOACCEL      = 4.0f;
const float MARIOMAXVELX    = 0.9f;
const float MARIOMAXVELY    = 6.0f;
const float MARIORUNDIFF    = 0.5f;

const float MARIOJUMP       =  2.5f;
const float MARIOGRAV       = -10.0f;
const float MARIOMAXGRAV    = -2.5f;

const float MARIOHIGHJUMPGRAV       = MARIOGRAV + 4 ;
const float MARIOHIGHJUMPMAXGRAV    = MARIOMAXGRAV;

const float MARIODEADGRAV           = MARIOGRAV + 5;
const float MARIODEADMAXGRAV        = MARIOMAXGRAV + .5;

const float BLOCKX          = -.4;
const float BLOCKY          = -.2;
const float BLOCKWIDTH      = .15;
const float BLOCKHEIGHT     = .19;
const float BLOCKANIMDELAY  = .10;

const float TILEX          = -.8;
const float TILEY          = -.85;
const float TILEWIDTH      = 0.1;
const float TILEHEIGHT     = 0.1;
const float TILEANIMDELAY  = .10;

direction_t marioDirection;
range_t     marioAnim;

GLuint displayList;

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

sf::Sound jumpSound;
sf::Sound startGameSound;
sf::Sound skidSound;
sf::Sound hurtSound;

sf::SoundBuffer hurtBuffer;
sf::SoundBuffer startBuffer;
sf::SoundBuffer jumpBuffer;
sf::SoundBuffer skidBuffer;

void Init();
void Display();
void Reshape( int w,int h );
void Keyboard( unsigned char key,int x, int y );
void KeyboardUp( unsigned char key, int x, int y );
void SpecialKey( int key, int x ,int y );
void SpecialKeyUp( int key,int x ,int y );
void SpriteState();
void SetScene();
void ControlCamera();
void Animate();
void CheckInput();
void Update();
void CheckCollisions();
void CalculateFPS();
void Timer();
void Idle();
void LoadObjects();
void CalculateDrawTime();
void Quit();
void Console();

int main ( int argc,char ** argv ) {
    Init();
    startGameSound.play();
    //Create opengl context
    cout << "Initializing Opengl...\n";
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition( WINDOWPOSX, WINDOWPOSY );
    glutInitWindowSize( WINDOWWIDTH, WINDOWHEIGHT );
    glutGameModeString("1024x768:32");
    if ( !FULLSCREEN ) {
        
        glutCreateWindow( windowTitle.c_str() );
    } else {
        glutCreateWindow( windowTitle.c_str() );
        glutFullScreen();
    }
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glutDisplayFunc( Display );
    glutReshapeFunc( Reshape );
    glutIdleFunc( Idle );
    glutKeyboardFunc( Keyboard );
    glutKeyboardUpFunc( KeyboardUp );
    glutSpecialFunc( SpecialKey );
    glutSpecialUpFunc( SpecialKeyUp );
    glutJoystickFunc(Joystick,1);
    //Created opengl Context
    
    LoadObjects();
    displayList = glGenLists( 1 );
    glutMainLoop();
    Quit();

	return EXIT_SUCCESS;
}

void Idle() {
    CheckInput();
    Update();
    Animate();
    CheckCollisions();
    SetScene();
    CalculateFPS();
    ControlCamera();
    glutPostRedisplay();
    Console();
}

void Init(){
    // Load a song to play
    if ( !titleSong.openFromFile( "overworld.ogg" ) ) {
        cout << "Failed to load song...\n";
    }
    
    if ( !hurtBuffer.loadFromFile("hurt.wav")) {
        cout << "Failed to load sound ... \n";
    }
    
    if (!startBuffer.loadFromFile("start.wav")) {
        cout << "Failed to load sound... \n";
    }
    
    if (!jumpBuffer.loadFromFile("jump.wav")) {
        cout << "Could not load sound...\n";
    }
    
    if (!skidBuffer.loadFromFile("skid.wav")) {
        cout << "Could not load file \n";
    }
    
    //Load jump sound
    jumpSound.setBuffer(jumpBuffer);
    startGameSound.setBuffer(startBuffer);
    skidSound.setBuffer(skidBuffer);
    
    //Play music
    if (PLAYMUSIC){
        titleSong.setVolume(50.0f);
        titleSong.play();
    }
}

void Display() {
    timeOfLastDraw = glutGet( GLUT_ELAPSED_TIME ) * .001;
    glClear( GL_COLOR_BUFFER_BIT );
    glCallList( displayList );
    glutSwapBuffers();
    totalFramesRendered++;
    CalculateDrawTime();
}

void Reshape(int w, int h) {
    glViewport( 0, 0, (GLsizei)w, (GLsizei)h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    if (FULLSCREEN){
     gluOrtho2D( -1.0 * ASPECTRATIO , 1.0 * ASPECTRATIO, -1.0 , 1.0);
    }
    if (!FULLSCREEN){
        gluOrtho2D( -1.0  , 1.0 , -1.0 , 1.0);
    }
}


void SetScene() {
    glNewList( displayList, GL_COMPILE );
    
    //set line drawing width for axes
    glLineWidth( 1 );
    glColor3f( 0.1f, 0.2f, 1.0f );
    
    //start textured drawing
    glEnable( GL_TEXTURE_2D );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    //Start Object Drawing
    for ( auto i = sprite.begin() ; i != sprite.end() ; ++i ) {
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
    curTime = glutGet( GLUT_ELAPSED_TIME ) * .001;
    deltaTime = curTime - lastTime;

    if ( deltaTime >= 1.0f ) {
        lastTime = curTime;
        FPS = totalFramesRendered;
        totalFramesRendered = 0;
        char buffer[100];
        sprintf(buffer, "Super Mario Open - By Evan Chapman - FPS: %d", FPS);
        windowTitle = buffer;
        glutSetWindowTitle(windowTitle.c_str());
    }
}

void CalculateDrawTime() {
    deltaDraw = glutGet( GLUT_ELAPSED_TIME ) *.001 - timeOfLastDraw;
}

void Update() {
    for (auto i = sprite.begin() ; i != sprite.end() ; ++i ){
        (*i)->Update(deltaDraw);
    }
}

void Animate(){
    for (auto i = sprite.begin() ; i != sprite.end() ; i++ ){
        (*i)->Animate(deltaDraw);
    }
}

void Quit() {
    cout << "Quitting..." << endl;
    cout << sprite.size()  << " objects to delete " << endl;
    for (unsigned long i = sprite.size() - 1 ; i > 0 ; i-- ) {
        delete sprite[i];
        sprite.pop_back();
    }
}

void CheckInput() {
    glutForceJoystickFunc();
    
    //if escaped
    if ( pressed.escape ) {
        if ( FULLSCREEN ) {
            glutLeaveGameMode();
        }
        hurtSound.setBuffer(hurtBuffer);
        hurtSound.play();
        titleSong.stop();
        Quit();
        cout << "Exit \n";
        exit(0);
    }
    
    //Reset Mario at will
    if (pressed.r) {
        mario->Reset();
        mario->SetPosition(MARIOX, MARIOY, 0);
        titleSong.openFromFile("overworld.ogg");
        titleSong.play();
    }
    
    if (pressed.m) {
        glutReshapeWindow(WINDOWWIDTH, WINDOWHEIGHT );
        glutPositionWindow(WINDOWPOSX, WINDOWPOSY );
        FULLSCREEN = false;
    }
    
    if (pressed.f) {
        FULLSCREEN = true;
        glutFullScreen();
        glLoadIdentity();
        gluOrtho2D( -1.0 * ASPECTRATIO , 1.0 * ASPECTRATIO, -1.0 , 1.0);
    }
    
    //press left or right
    if ( (pressed.kleft || pressed.jleft) && !mario->isDead ) {
        tempVec.x = -MARIOACCEL;
        mario->direction = leftd;
        mario->isAnimated = true;
        mario->isStopping = false;
        
    } else if((pressed.kright || pressed.jright) && !mario->isDead) {
        tempVec.x = MARIOACCEL;
        mario->direction = rightd;
        mario->isAnimated = true;
        mario->isStopping = false;
    } else {
        tempVec.x = 0.0f;
        mario->isStopping = true;
    }
    
    //Press run button
    if (pressed.shift || held.jbutton1 || pressed.z) {
        secondVec.x = MARIOMAXVELX + MARIORUNDIFF;
        secondVec.y = MARIOMAXVELY;
        mario->SetMaxVelocity(secondVec);
        secondVec.x = -MARIOMAXVELX - MARIORUNDIFF;
        secondVec.y = -MARIOMAXVELY;
        mario->SetMinVelocity(secondVec);
    } else {
        secondVec.x = MARIOMAXVELX;
        secondVec.y = MARIOMAXVELY;
        mario->SetMaxVelocity(secondVec);
        secondVec.x = -MARIOMAXVELX;
        secondVec.y = -MARIOMAXVELY;
        mario->SetMinVelocity(secondVec);
    }
    
    //Reset marios ability to jump
    if ((mario->topCollision) && !allowJump && !pressed.x) {
            allowJump = true;
    }
    
    //Press Jump button make mario jump
    if ( (pressed.x || pressed.jbutton2)
        && allowJump
        ) {
        allowJump = false;
        jumpSound.play();
        mario->Jump();
    } else {
        tempVec.y = 0.0f;
    }
    
    //change jump height based on length of jump button hold
    vector2d_t  tempGravity;
    if ( pressed.x || held.jbutton2) {
        tempGravity.y = MARIOHIGHJUMPMAXGRAV;
        mario->SetMaxGravity(tempGravity);
        tempGravity.y = MARIOHIGHJUMPGRAV + (mario->GetVelocity().x /2 );
        mario->SetGravity(tempGravity);
    } else {
        tempGravity.y = MARIOMAXGRAV;
        mario->SetMaxGravity(tempGravity);
        tempGravity.y = MARIOGRAV +  (mario->GetVelocity().x /2 ) ;
        mario->SetGravity(tempGravity);
    }
    
    if (mario->isDead) {
        tempGravity.y = MARIODEADMAXGRAV;
        mario->SetMaxGravity(tempGravity);
        tempGravity.y = MARIODEADGRAV;
        mario->SetGravity(tempGravity);
    }
    
    if ( mario->Turning() == true ) {
        if ( skidSound.getStatus() == 0 ) {
            skidSound.play();
        }
    }
    
    if (pressed.plus) {
        mario->SetPosition(mario->GetPosition().x, mario->GetPosition().y, mario->GetPosition().z + deltaDraw);
    }
    if (pressed.minus) {
        mario->SetPosition(mario->GetPosition().x, mario->GetPosition().y, mario->GetPosition().z - deltaDraw);
    }
    
    //if mario moves past scroll section
    if (mario->GetPosition().x > SCROLLRIGHT) {
        float scrollShift = mario->GetPosition().x - SCROLLRIGHT ;
        CAMERAOFFSET -= scrollShift;
        mario->SetPosition(SCROLLRIGHT, mario->GetPosition().y, mario->GetPosition().z);
        for (auto i = sprite.begin() ; i != sprite.end(); i++ ) {
            Sprite* object = *i;
            if (object->isScrolling) {
                object->SetPosition(object->GetPosition().x- scrollShift, object->GetPosition().y, object->GetPosition().z);
            }
        }
    }
    
    if (mario->GetPosition().x < SCROLLLEFT) {
        float scrollShift = mario->GetPosition().x - SCROLLLEFT ;
        CAMERAOFFSET += scrollShift;
        mario->SetPosition(SCROLLLEFT, mario->GetPosition().y, mario->GetPosition().z);
        for (auto i = sprite.begin() ; i != sprite.end(); i++ ) {
            Sprite* object = *i;
            if (object->isScrolling) {
                object->SetPosition(object->GetPosition().x - scrollShift, object->GetPosition().y, object->GetPosition().z);
            }
        }
    }
    
    mario->SetAcceleration( tempVec );
    mario->SetAnimationDelay(.98 - abs(mario->GetVelocity().x));
    
}

void ControlCamera() {
    //Rotate camera using WASD and parentheses
    float angle = 0.0f;
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    
    //rotate about Z axis
    if ( pressed.rbracket ) {
        angle = -turnAngle;
        z = turnAmount;
    }
    
    if ( pressed.lbracket ) {
        angle = turnAngle;
        z = turnAmount;
    }
    //rotate about x axis
    if ( pressed.w ) {
        angle = turnAngle;
        x = turnAmount;
    }
    if ( pressed.s ) {
        angle = -turnAngle;
        x = turnAmount;
    }
    //rotate about y axis
    if ( pressed.a ) {
        angle =turnAngle;
        y = turnAmount;
    }
    
    if ( pressed.d ) {
        angle = -turnAngle;
        y = turnAmount;
    }
    
    glRotatef(angle * deltaDraw, x * deltaDraw, y * deltaDraw, z * deltaDraw);
    
}

void CheckCollisions() {
    
    //check for off screen bottom
    if (mario->GetPosition().y <= -1.15f && !mario->isDead) {
        titleSong.stop();
        titleSong.openFromFile("dead.ogg");
        titleSong.play();
        mario->Dead();
    }
    
    if (mario->GetPosition().y <= -4.8f && mario->isDead) {
        titleSong.stop();
        titleSong.openFromFile("overworld.ogg");
        if (PLAYMUSIC) {
            titleSong.play();
        }
        mario->Reset();
    }
    
    for (auto i = sprite.begin() ; i < sprite.end() ; i++ ) {
        Sprite* test = (*i);
        if (test == mario) {
            continue;
        } else if (test != mario) {
            if (mario->Colliding(*test)) {
                didCollide = true;
            }
            else didCollide = false;
        }
    }
    
    if (didCollide){
        if (mario->bottomCollision) {
            mario->Fall();
            tempVec.x = mario->GetVelocity().x;
            tempVec.y = mario->GetMaxGravity().y;
            mario->SetVelocity(tempVec);
        }
        
        if (mario->topCollision && allowJump) {
            mario->StopFall();
            tempVec.x = mario->GetVelocity().x;
            tempVec.y = 0;
            mario->SetVelocity(tempVec);
        }
        
        if (mario->rightCollision) {
            tempVec.x = 0;
            tempVec.y = mario->GetVelocity().y;
            mario->SetVelocity(tempVec);
        }
    
        if (mario->leftCollision) {
            tempVec.x = 0;
            tempVec.y = mario->GetVelocity().y;
            mario->SetVelocity(tempVec);
        }
    } else {
       mario->bottomCollision = mario->topCollision = mario->leftCollision = mario->rightCollision = false;
    }
    
    if (!mario->isColliding && !mario->isJumping && !mario->bottomCollision) {
        mario->Fall();
    }
}

void LoadObjects() {
    cout << "Loading Objects: " << "\n";
    
    //Use blank canvas for animation
    vector<int> blankAnimation;
    
    //Create background
    testSprite = new Sprite();
    testSprite->name = "Background";
    testSprite->AddAnimation(blankAnimation);
    testSprite->AddFrame("background1.png", 0);
    totalTextures++;
    testSprite->SetPosition(BACKGROUNDX, BACKGROUNDY, 0);
    testSprite->SetSize(BACKGROUNDWIDTH, BACKGROUNDHEIGHT, 0);
    sprite.push_back(testSprite);
    
    //Create test tiles
    for (int i = 0 ; i < 15 ; i++) {
        blockSprite = new Sprite();
        char buffer[20]  ;
        sprintf(buffer, "tile %d",i);
        blockSprite->name = buffer;
        blockSprite->AddAnimation(blankAnimation);
        blockSprite->SetAnimationDelay(TILEANIMDELAY);
        blockSprite->AddFrame("tile1.png", 0);
        totalTextures++;
        blockSprite->SetPosition(TILEX + (i *.1), TILEY, 0);
        blockSprite->SetSize(TILEWIDTH, TILEHEIGHT, 0.0f);
        blockSprite->IsBoundary(true);
        blockSprite->isScrolling = true;
        sprite.push_back(blockSprite);
    }
    
    //Create block 1
    blockSprite = new Sprite();
    blockSprite->name = "Block";
    blockSprite->AddAnimation(blankAnimation);
    blockSprite->SetAnimationDelay(BLOCKANIMDELAY);
    blockSprite->AddFrame("block1.png", 0);
    totalTextures++;
    blockSprite->AddFrame("block2.png", 0);
    totalTextures++;
    blockSprite->AddFrame("block3.png", 0);
    totalTextures++;
    blockSprite->AddFrame("block4.png", 0);
    totalTextures++;
    blockSprite->SetPosition(BLOCKX, BLOCKY, 0);
    blockSprite->SetSize(BLOCKWIDTH, BLOCKHEIGHT, 0.0f);
    blockSprite->SetLooping(true);
    blockSprite->isAnimated = true;
    blockSprite->IsBoundary(true);
    blockSprite->isScrolling = true;
    sprite.push_back(blockSprite);
    blockSprite = 0;
    
    //Create block 1
    blockSprite = new Sprite();
    blockSprite->name = "Block 2";
    blockSprite->AddAnimation(blankAnimation);
    blockSprite->SetAnimationDelay(BLOCKANIMDELAY);
    blockSprite->AddFrame("block1.png", 0);
    totalTextures++;
    blockSprite->AddFrame("block2.png", 0);
    totalTextures++;
    blockSprite->AddFrame("block3.png", 0);
    totalTextures++;
    blockSprite->AddFrame("block4.png", 0);
    totalTextures++;
    blockSprite->SetPosition(BLOCKX + .9, BLOCKY, 0);
    blockSprite->SetSize(BLOCKWIDTH, BLOCKHEIGHT, 0.0f);
    blockSprite->SetLooping(true);
    blockSprite->isAnimated = true;
    blockSprite->IsBoundary(true);
    blockSprite->isScrolling = true;
    sprite.push_back(blockSprite);
    blockSprite = 0;
    
    //Create block 1
    blockSprite = new Sprite();
    blockSprite->name = "Block 3";
    blockSprite->AddAnimation(blankAnimation);
    blockSprite->SetAnimationDelay(BLOCKANIMDELAY);
    blockSprite->AddFrame("block1.png", 0);
    totalTextures++;
    blockSprite->AddFrame("block2.png", 0);
    totalTextures++;
    blockSprite->AddFrame("block3.png", 0);
    totalTextures++;
    blockSprite->AddFrame("block4.png", 0);
    totalTextures++;
    blockSprite->SetPosition(BLOCKX + .75, BLOCKY -.46, 0);
    blockSprite->SetSize(BLOCKWIDTH, BLOCKHEIGHT, 0.0f);
    blockSprite->SetLooping(true);
    blockSprite->isAnimated = true;
    blockSprite->IsBoundary(true);
    blockSprite->isScrolling = true;
    sprite.push_back(blockSprite);
    blockSprite = 0;
    
    //Create block 1
    blockSprite = new Sprite();
    blockSprite->name = "Block 4";
    blockSprite->AddAnimation(blankAnimation);
    blockSprite->SetAnimationDelay(BLOCKANIMDELAY);
    blockSprite->AddFrame("block1.png", 0);
    totalTextures++;
    blockSprite->AddFrame("block2.png", 0);
    totalTextures++;
    blockSprite->AddFrame("block3.png", 0);
    totalTextures++;
    blockSprite->AddFrame("block4.png", 0);
    totalTextures++;
    blockSprite->SetPosition(BLOCKX -.4, BLOCKY -.46, 0);
    blockSprite->SetSize(BLOCKWIDTH, BLOCKHEIGHT, 0.0f);
    blockSprite->SetLooping(true);
    blockSprite->isAnimated = true;
    blockSprite->IsBoundary(true);
    blockSprite->isScrolling = true;
    sprite.push_back(blockSprite);
    blockSprite = 0;
    
    //Create mario
    marioSprite = new Sprite();
    marioSprite->SetSize(MARIOWIDTH, MARIOHEIGHT, 0);
    marioSprite->SetPosition(MARIOX, MARIOY, 0.0f);
    marioSprite->AddAnimation(blankAnimation);
    marioSprite->SetAnimationDelay(MARIOANIMDELAY);
    
    for (int i = 0 ; i < 18 ; i++ ) {
        marioSprite->AddAnimation(blankAnimation);
    }
    
    marioSprite->name = "Mario";
    //running/stopped left animation
    marioSprite->AddFrame("marioleft.png", stoppedl);
    totalTextures++;
    marioSprite->AddFrame("marioleft.png", runningl);
    totalTextures++;
   // marioSprite->AddFrame("marioleft2.png", runningl);
    //totalTextures++;
    marioSprite->AddFrame("marioleft3.png", runningl);
    totalTextures++;
    
    //running stoppped right animation
    marioSprite->AddFrame("marioright.png", stoppedr);
    totalTextures++;
    
    marioSprite->AddFrame("marioright.png", runningr);
    totalTextures++;
    //marioSprite->AddFrame("marioright2.png", runningr);
    //totalTextures++;
    marioSprite->AddFrame("marioright3.png", runningr);
    totalTextures++;
    
    //jumping left animation
    marioSprite->AddFrame("mariojumpleft.png", jumpingl);
    totalTextures++;
    
    //jumping right animation
    marioSprite->AddFrame("mariojumpright.png", jumpingr);
    totalTextures++;
    
    //falling left animation
    marioSprite->AddFrame("mariojumpleft.png", fallingl);
    totalTextures++;
    
    //falling right animation
    marioSprite->AddFrame("mariojumpright.png", fallingr);
    totalTextures++;
    
    //turning left animation
    marioSprite->AddFrame("marioturnright.png", turningl);
    totalTextures++;
    
    //turning right animation
    marioSprite->AddFrame("marioturnleft.png", turningr);
    totalTextures++;
    //dying left animation
    marioSprite->AddFrame("mariodying.png", dyingl);
    totalTextures++;
    
    //dying right animation
    marioSprite->AddFrame("mariodying.png", dyingr);
    totalTextures++;
    marioSprite->SetLooping(true);
    
    //Temporary vector for objects
    vector2d_t tVec;
    
    tVec.x = MARIOFRICTIONX;
    marioSprite->SetFriction(tVec);
    
    tVec.y = MARIOJUMP;
    marioSprite->SetJumpStrength(tVec);
    
    marioSprite->StopAnimation();
    marioSprite->direction = rightd;
    marioSprite->SetAnimation(stoppedr);
    marioSprite->IsBoundary(true);
    marioSprite->isScrolling = false;
    sprite.push_back(marioSprite);
    mario = marioSprite;
    marioSprite = 0;
}

//Console text output used for debugging
void Console() {
    //cout << "Jump " << mario->isJumping << endl << "Fall: " << mario->isFalling << endl;
    //cout << "Vel X: " << mario->GetVelocity().x << endl << "Vel Y: " << mario->GetVelocity().y << endl;
    //cout << "JSy: " << mario->GetJumpStrength().y << endl;
    //cout << "MaxVx " << mario->GetMaxVelocity().x << endl;
    //cout << "MaxVy " << mario->GetMaxVelocity().y << endl;
    //cout << "Coll: " << mario->isColliding << endl;
    // cout << "T: " << mario->topCollision << " B: " << mario->bottomCollision << " L: " << mario->leftCollision << " R: " << mario->rightCollision << endl;
    //cout << "Did collide: " << didCollide << endl;
    //cout << "sjump " << mario->stoppedJumping << endl;
    //cout << "allow jump " << allowJump <<  endl;
    cout << "X: " << mario->GetPosition().x  << "Y: " << mario->GetPosition().y << endl;
}






