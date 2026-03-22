//inicjacja biblooteki - Kacper
#include <SFML/Graphics.hpp>

//Kuba: Głowna petla gry, Obsluga klawiatury i mszy
//Łukasz: Zamkniecie gry, parametryzowanie innych parametrow
//Kacper: Inicjjacja biblioteki, parametryzofanie trybu graficznego
//4: Czyszczenie ekranu, obsluga bledow

int main() {
	sf::RenderWindow window( sf::VideoMode( { 200, 200 } ), "SFML works!" );
	sf::CircleShape shape( 100.f );
	shape.setFillColor( sf::Color::Blue );

	while ( window.isOpen() )
	{
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() )
				window.close();
		}

		window.clear();
		window.draw( shape );
		window.display();
	}
}
