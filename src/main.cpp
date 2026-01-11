#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace sf;
using namespace std;

char random_letter_generator() { return 'A' + rand() % 26; }

float random_X_value(int x) { return rand() % (x + 1); }

int main() {

  // settings
  ContextSettings settings;
  settings.antiAliasingLevel = 4;

  // load font - FiraCodeNerdFontMono-Light
  const string Fira_Path = "assets/Fonts/FiraCodeNerdFontMono-Light.ttf";
  Font firacode;
  if (!firacode.openFromFile(Fira_Path)) {
    cerr << "Error loading firacode" << endl;
    return -1;
  }

  // create start menu window
  RenderWindow startWindow(VideoMode({1920, 1080}), "Start Menu",
                           State::Windowed, settings);
  startWindow.setVerticalSyncEnabled(true);

  // create title text
  Text titleText(firacode);
  titleText.setString("Keyboard Hero");
  titleText.setCharacterSize(72);
  titleText.setFillColor(Color::White);
  FloatRect titleTextSize = titleText.getLocalBounds();
  titleText.setOrigin({titleTextSize.size.x / 2.f, titleTextSize.size.y / 2.f});
  titleText.setPosition(
      {startWindow.getSize().x / 2.f, startWindow.getSize().y / 3.f});

  // create matrix-style background
  Text randomLetterText(firacode);
  srand(time(nullptr));
  char random_letter = random_letter_generator();
  randomLetterText.setString(random_letter);
  randomLetterText.setCharacterSize(36);
  randomLetterText.setFillColor(Color::Green);
  FloatRect randomLetterTextSize = randomLetterText.getLocalBounds();
  randomLetterText.setOrigin(
      {randomLetterTextSize.size.x / 2.f, randomLetterTextSize.size.y / 2.f});
  float randomXvalue = random_X_value(startWindow.getSize().x);
  randomLetterText.setPosition({randomXvalue, -20.f});
  float randomLetterSpeed = 600.f;

  // initialize clock
  Clock clock;

  // start start menu loop
  while (startWindow.isOpen()) {

    // start delta time
    float dt = clock.getElapsedTime().asSeconds();
    if (dt >= 1.f / 60.f) {

      clock.restart();

      // start event handling
      while (const auto event = startWindow.pollEvent()) {
        if (event->is<Event::Closed>()) {
          startWindow.close();
        }
      } // END start menu event handling
      startWindow.clear();

      // draw titleText to start menu
      startWindow.draw(titleText);

      //  draw random letter
      startWindow.draw(randomLetterText);
      randomLetterText.move({0.f, randomLetterSpeed * dt});
      if (randomLetterText.getPosition().y >= startWindow.getSize().y) {
        char randomletter = random_letter_generator();
        float randomX = random_X_value(startWindow.getSize().x);
        randomLetterText.setString(randomletter);
        randomLetterText.setPosition({randomX, -20.f});
        startWindow.draw(titleText);
      }

      // update window
      startWindow.display();
    }
  } // END start menu loop

  // create game window
  RenderWindow window(VideoMode({1920, 1080}), "Keyboard Hero");

  // start game loop //
  while (window.isOpen()) {
    while (const auto event = window.pollEvent()) {
      if (event->is<Event::Closed>()) {
        window.close();
      }
    }

    window.clear();

    window.display();
  }

  return 0;
}
