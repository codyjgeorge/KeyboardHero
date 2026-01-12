#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace sf;
using namespace std;

char random_letter_generator() { return 'A' + rand() % 26; }

float random_X_value(int windowSize) { return rand() % (windowSize + 1); }

struct randomLetter {
  Text randomLetterText;
  float randomLetterSpeed;
};
vector<randomLetter> randomLetters;

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
  titleText.setStyle(Text::Bold);
  titleText.setFillColor(Color::White);
  FloatRect titleTextSize = titleText.getLocalBounds();
  titleText.setOrigin({titleTextSize.size.x / 2.f, titleTextSize.size.y / 2.f});
  titleText.setPosition(
      {startWindow.getSize().x / 2.f, startWindow.getSize().y / 3.f});

  // create title subtext
  Text titleSubtext(firacode);
  titleSubtext.setString("Press ENTER");
  titleSubtext.setCharacterSize(48);
  titleSubtext.setFillColor(Color::White);
  FloatRect titleSubtextSize = titleSubtext.getLocalBounds();
  titleSubtext.setOrigin(
      {titleSubtextSize.size.x / 2.f, titleSubtextSize.size.y / 2.f});
  titleSubtext.setPosition(
      {startWindow.getSize().x / 2.f, startWindow.getSize().y / 1.5f});

  // initialize clocks
  Clock gameClock;
  Clock spawnClock;
  float spawnInterval = 0.2f;

  // start start menu loop
  while (startWindow.isOpen()) {

    // start delta time
    float dt = gameClock.getElapsedTime().asSeconds();
    if (dt >= 1.f / 60.f) {

      gameClock.restart();

      // start event handling
      while (const auto event = startWindow.pollEvent()) {
        if (event->is<Event::Closed>()) {
          startWindow.close();
        }

        // keyboard input - key pressed //
        if (auto *key = event->getIf<sf::Event::KeyPressed>()) {

          // ENTER -> close start screen, begin game
          if (key->scancode == Keyboard::Scancode::Enter) {
            startWindow.close();
          }
        } // END key pressed handling
      } // END start menu event handling
      startWindow.clear();

      // draw start menu titling text
      startWindow.draw(titleText);
      startWindow.draw(titleSubtext);

      // create matrix-style background
      if (spawnClock.getElapsedTime().asSeconds() > spawnInterval) {
        Text randomLetterText(firacode);
        char random_letter = random_letter_generator();
        randomLetterText.setString(random_letter);
        randomLetterText.setCharacterSize(36);
        randomLetterText.setFillColor(Color::Green);
        FloatRect randomLetterTextSize = randomLetterText.getLocalBounds();
        randomLetterText.setOrigin({randomLetterTextSize.size.x / 2.f,
                                    randomLetterTextSize.size.y / 2.f});
        float randomXvalue = random_X_value(startWindow.getSize().x);
        randomLetterText.setPosition({randomXvalue, -20.f});

        // add letters to vector with speed of 600.f
        randomLetters.push_back({randomLetterText, 600.f});

        spawnClock.restart();
      }

      // update all falling letters
      float spawnDT = gameClock.restart().asSeconds();
      for (auto &letter : randomLetters) {
        letter.randomLetterText.move({0, letter.randomLetterSpeed * dt});
        startWindow.draw(letter.randomLetterText);
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
