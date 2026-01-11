#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

int main() {

  // settings
  ContextSettings settings;
  settings.antiAliasingLevel = 4;

  // load font
  const string Fira_Path = "assets/Fonts/FiraCodeNerdFontMono-Light.ttf";
  Font firacode;
  if (!firacode.openFromFile(Fira_Path)) {
    cerr << "Error loading firacode" << endl;
    return -1;
  }

  // create title text
  Text text(firacode);
  text.setString("Keyboard Hero");
  text.setCharacterSize(72);
  text.setFillColor(Color::White);
  text.setPosition({100.f, 100.f});

  // create start menu window
  RenderWindow startWindow(VideoMode({1920, 1080}), "Start Menu");

  // start start menu loop
  while (startWindow.isOpen()) {
    while (const auto event = startWindow.pollEvent()) {
      if (event->is<Event::Closed>()) {
        startWindow.close();
      }
    }
    startWindow.clear();
    startWindow.draw(text);
    startWindow.display();
  }

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
