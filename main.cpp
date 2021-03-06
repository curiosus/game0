#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

/*
http://curiosus.me
jepeterson@gmail.com
2015-05-24-14:04MDT
game0
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

  float x() {
    return shape.getPosition().x;
  }

  float y() {
    return shape.getPosition().y;
  }

  float left() {
    return x() - shape.getSize().x / 2.f;
  }

  float right() {
    return x() + shape.getSize().x / 2.f;
  }

  float top() {
    return y() - shape.getSize().y / 2.f;
  }

  float bottom() {
    return y() + shape.getSize().y / 2.f;
  }
  
};

//Ball Class
//Ball that bounces around and is struck by the Striker
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

  float x() {
    return shape.getPosition().x;
  }

  float y() {
    return shape.getPosition().y;
  }

  float left() {
    return x() - shape.getRadius(); 
  }

  float right() {
    return x() + shape.getRadius();
  }

  float top() {
    return y() - shape.getRadius();
  }

  float bottom() {
    return y() + shape.getRadius();
  }
      
};

//Target class
//These form the wall of targets to be hit with the ball
constexpr float targetWidth{60.f};
constexpr float targetHeight{20.f};
struct Target {
  RectangleShape shape;
  bool destroyed{false};
  
  Target(float x, float y) {
    cout << x << ", " << y << endl;
    shape.setPosition(x, y);
    shape.setSize({targetWidth, targetHeight});
    shape.setFillColor(Color::Green);
    shape.setOrigin(targetWidth / 2.f, targetHeight / 2.f);
  }


  float x() {
    return shape.getPosition().x;
  }

  float y() {
    return shape.getPosition().y;
  }

  float left() {
    return x() - shape.getSize().x / 2.f;
  }

  float right() {
    return x() + shape.getSize().x / 2.f;
  }

  float top() {
    return y() - shape.getSize().y / 2;
  }

  float bottom() {
    return y() + shape.getSize().y / 2.f;  
  }

};

template<class T1, class T2> bool isIntersection(T1& p1, T2& p2) {
  return p1.right() >= p2.left() && p1.left() <= p2.right()
         && p1.bottom() >= p2.top() && p1.top() <= p2.bottom(); 
}

void collision(Striker& striker, Ball& ball) {
  if (isIntersection(striker, ball)) {
    ball.velocity.y = -ballVelocity;
    if (ball.x() < striker.x()) {
      ball.velocity.x = -ballVelocity;
    } else {
      ball.velocity.x = ballVelocity;
    }
  } 
}

void collision(Ball& ball, Target& target) {
  if (isIntersection(ball, target)) {

    target.destroyed = true;

    float intersectLeft{ball.right() - target.left()};
    float intersectRight{target.right() - ball.left()};
    float intersectTop{ball.bottom() - target.top()};
    float intersectBottom{target.bottom() - ball.top()};

    bool ballHitOnLeft{abs(intersectLeft) < abs(intersectRight)};
    bool ballHitFromTop{(abs(intersectTop) < abs(intersectBottom))}; 

    float intersectX{ballHitOnLeft ? intersectLeft : intersectRight};
    float intersectY{ballHitFromTop ? intersectTop : intersectBottom};

    if (abs(intersectX) < abs(intersectY)){
      ball.velocity.x = ballHitOnLeft ? -ballVelocity : ballVelocity;
    } else {
      ball.velocity.y = ballHitFromTop ? - ballVelocity : ballVelocity;
    }
    
  }
}

int main() {

  Striker striker;
  Ball ball;
  vector<Target> targets;
  for (int i{0}; i < 3; i++ ) {
    for (int j{0}; j < 16; j++) {
      targets.emplace_back((j+1) * (targetWidth + 5), (i+1) * (targetHeight + 5));
    }
  }
  cout << targets.size() << endl;

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

    ball.update();
    striker.update();
    collision(striker, ball); 
    window.draw(striker.shape);
    window.draw(ball.shape); 
    for (Target& target : targets) {
      if (!target.destroyed) {
        collision(ball, target);
        window.draw(target.shape);
      }
    }
    window.display();

  }
  return 0;
}
