#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace sf;
using namespace std;

char random_letter_generator() { return 'A' + rand() % 26; }

float random_X_value(int windowSize) { return rand() % (windowSize + 1); }
float random_speed() { return 300 + rand() % 601; }

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

  // create game window
  RenderWindow window(VideoMode({1920, 1080}), "Keyboard Hero");
  window.setVerticalSyncEnabled(true);

  // create title text
  Text titleText(firacode);
  titleText.setString("Keyboard Hero");
  titleText.setCharacterSize(72);
  titleText.setStyle(Text::Bold);
  titleText.setFillColor(Color::White);
  FloatRect titleTextSize = titleText.getLocalBounds();
  titleText.setOrigin({titleTextSize.size.x / 2.f, titleTextSize.size.y / 2.f});
  titleText.setPosition({window.getSize().x / 2.f, window.getSize().y / 4.f});

  // create menu text
  Text menuText(firacode);
  menuText.setString("ENTER -> Play\n\nR -> Rules\n\nS -> Settings\n\nC -> "
                     "Controls\n\nQ -> Quit");
  menuText.setCharacterSize(36);
  menuText.setFillColor(Color::White);
  FloatRect menuTextSize = menuText.getLocalBounds();
  menuText.setOrigin({menuTextSize.size.x / 2.f, menuTextSize.size.y / 2.f});
  menuText.setPosition({window.getSize().x / 2.f, window.getSize().y / 1.5f});

  // sound buffers
  SoundBuffer titleSongBuffer;
  SoundBuffer titleTransSoundBuffer;

  // load to buffers
  if (!titleSongBuffer.loadFromFile("assets/Audio/StartMenu-Song.wav")) {
    cerr << "Failed to load title song" << endl;
  }
  if (!titleTransSoundBuffer.loadFromFile(
          "assets/Audio/StartMenuTransition-Sound.wav")) {
    cerr << "Failed to load title transition sound" << endl;
  }

  // sound players
  Sound titleSong(titleSongBuffer);
  Sound titleTransSound(titleTransSoundBuffer);

  // initialize clocks
  Clock gameClock;
  Clock spawnClock;
  float spawnInterval = 0.2f;

  // start title song
  titleSong.setLooping(true);
  titleSong.play();
  bool isLooping = titleSong.isLooping();

  // start game loop //
  while (window.isOpen()) {

    // start delta time
    float dt = gameClock.getElapsedTime().asSeconds();
    if (dt >= 1.f / 60.f) {

      gameClock.restart();

      // start event handling
      while (const auto event = window.pollEvent()) {
        if (event->is<Event::Closed>()) {
          window.close();
          titleSong.stop();
        }

        // keyboard input - key pressed //
        if (auto *key = event->getIf<sf::Event::KeyPressed>()) {

          // Q -> quit
          if (key->scancode == Keyboard::Scancode::Q) {
            window.close();
            titleSong.stop();
          }
          // ENTER -> close start screen, begin game
          if (key->scancode == Keyboard::Scancode::Enter) {
            window.close();
            titleSong.stop();
            titleTransSound.play();
          }
        } // END key pressed handling
      } // END game loop event handling

      // start drawing - menu window
      window.clear();

      // draw game menu titling text
      window.draw(titleText);
      window.draw(menuText);

      // create matrix-style background - game menu
      if (spawnClock.getElapsedTime().asSeconds() > spawnInterval) {
        Text randomLetterText(firacode);
        char random_letter = random_letter_generator();
        randomLetterText.setString(random_letter);
        randomLetterText.setCharacterSize(36);
        randomLetterText.setFillColor(Color::Green);
        // every 13th letter drops red
        if (randomLetters.size() % 13 == 0) {
          randomLetterText.setFillColor(Color::Red);
        }
        FloatRect randomLetterTextSize = randomLetterText.getLocalBounds();
        randomLetterText.setOrigin({randomLetterTextSize.size.x / 2.f,
                                    randomLetterTextSize.size.y / 2.f});
        float randomXvalue = random_X_value(window.getSize().x);
        randomLetterText.setPosition({randomXvalue, -20.f});

        // add letters to vector with random speed
        float randomSpeed = random_speed();
        randomLetters.push_back({randomLetterText, randomSpeed});

        spawnClock.restart();
      }

      // update all falling letters
      float spawnDT = gameClock.restart().asSeconds();
      for (auto &letter : randomLetters) {
        letter.randomLetterText.move({0, letter.randomLetterSpeed * dt});
        window.draw(letter.randomLetterText);
      }

      window.display();
    } // END delta time
  } // END game loop

  return 0;
}
