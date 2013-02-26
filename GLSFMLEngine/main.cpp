#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

/* Resources :
 * ===========
 *
 * In order to load the resources like cute_image.png, you have to set up
 * your target scheme :
 *
 * - Select "Edit Scheme…" in the "Product" menu;
 * - Check the box "use custom working directory";
 * - Fill the text field with the folder path containing your resources;
 *        (e.g. your project folder)
 * - Click OK.
 */
 

int main (int argc, const char * argv[])
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile("cute_image.jpg"))
    	return EXIT_FAILURE;
    sf::Sprite sprite(texture);

    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile("sansation.ttf"))
    	return EXIT_FAILURE;
    sf::Text text("Hello SFML", font, 50);
    text.setColor(sf::Color::Black);

    // Load a music to play
    sf::Music music;
    if (!music.openFromFile("nice_music.ogg"))
    	return EXIT_FAILURE;

    // Play the music
    music.play();
        

    // Start the game loop
    while (window.isOpen())
    {
    	// Process events
    	sf::Event event;
    	while (window.pollEvent(event))
    	{
    		// Close window : exit
    		if (event.type == sf::Event::Closed)
    			window.close();
            
    		// Escape pressed : exit
    		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    			window.close();
    	}

    	// Clear screen
    	window.clear();
    	
    	// Draw the sprite
    	window.draw(sprite);
    	
    	// Draw the string
    	window.draw(text);

    	// Update the window
    	window.display();
    }

	return EXIT_SUCCESS;
}
