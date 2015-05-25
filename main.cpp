#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

/*
http://curiosus.me
jepeterson@gmail.com
2015-05-24-14:04MDT
*/

using std::vector;
using std::cout;
using std::endl;

using sf::RenderWindow;
using sf::CircleShape;
using sf::RectangleShape;
using sf::Color;
using sf::Event;
using sf::Keyboard;
using sf::Vector2f;

constexpr int fps{60};
constexpr int windowWidth{1200};
constexpr int windowHeight{900};


//Striker Class
//Player uses striker to strike other objects 
//TODO Turn this into a class when I learn classes in my uhhh...class. Actually it's a book or two.
constexpr float strikerWidth{60.f}; 
constexpr float strikerHeight{20.f};
constexpr float strikerVelocity{6.f};
struct Striker {

  RectangleShape shape;
  Vector2f velocity;

  Striker () {
    shape.setPosition(windowWidth / 2.f, windowHeight - strikerHeight);
    shape.setSize({strikerWidth, strikerHeight});
    shape.setFillColor(Color::Red);
    shape.setOrigin(strikerWidth / 2.f, strikerHeight / 2.f);
  }

  void update() {
    if (Keyboard::isKeyPressed(Keyboard::Key::A) && left() > 0) {
      velocity.x = -strikerVelocity; 
    } else if (Keyboard::isKeyPressed(Keyboard::Key::D) && right() < windowWidth) {
      velocity.x = strikerVelocity;  
    } else {
      velocity.x = 0;
    }
    shape.move(velocity);
    
  }

  float left() {
    return shape.getPosition().x - shape.getSize().x / 2.f;
  }

  float right() {
    return shape.getPosition().x + shape.getSize().x / 2.f;
  }
  
};

constexpr float ballRadius{12.f};
constexpr float ballVelocity{6.f};
struct Ball {

  CircleShape shape;  
  Vector2f velocity{-ballVelocity, -ballVelocity};
  
  Ball() {
    shape.setPosition(windowWidth / 2, windowHeight / 2);
    shape.setRadius(ballRadius);
    shape.setFillColor(Color::Yellow);
    shape.setOrigin(ballRadius, ballRadius);
  }

  void update() {
    shape.move(velocity);

    if (left() < 0) {
      velocity.x = ballVelocity;
    } else if (right() > windowWidth) {
      velocity.x = -ballVelocity;
    }
    
    if (top() < 0) {
      velocity.y = ballVelocity;
    } else if (bottom() > windowHeight) {
      velocity.y = -ballVelocity;
    }
  }

  float left() {
    return shape.getPosition().x - shape.getRadius(); 
  }

  float right() {
    return shape.getPosition().x + shape.getRadius();
  }

  float top() {
    return shape.getPosition().y - shape.getRadius();
  }

  float bottom() {
    return shape.getPosition().y + shape.getRadius();
  }
      
};

int main() {

  RenderWindow window{{windowWidth, windowHeight}, "game0"};
  window.setFramerateLimit(fps);

  Striker striker;
  Ball ball;

  while (window.isOpen()) {
    window.clear(Color::Black);
    Event event;
    while (window.pollEvent(event)) {
      if (event.type == Event::Closed) {
        window.close();
        break;
      }
    }

    ball.update();
    striker.update();
    
    window.draw(striker.shape);
    window.draw(ball.shape); 
    window.display();

  }
  return 0;
}
