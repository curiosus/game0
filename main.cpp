#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

/*
http://curiosus.me
jepeterson@gmail.com
2015-05-24-14:04MDT
*/

using std::vector;
using sf::RenderWindow;
using sf::CircleShape;
using sf::RectangleShape;
using sf::Color;
using sf::Event;
using sf::Keyboard;

constexpr int fps{60};
constexpr int windowWidth{1200};
constexpr int windowHeight{900};

int main() {
  RenderWindow window{{windowWidth, windowHeight}, "game0"};
  window.setFramerateLimit(fps);
  while (window.isOpen()) {
    window.clear(Color::Black);
    Event event;
    while (window.pollEvent(event)) {
      if (event.type == Event::Closed) {
        window.close();
        break;
      }
    }

    window.display();

  }
  return 0;
}
