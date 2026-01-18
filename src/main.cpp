#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace sf;
using namespace std;

char random_letter_generator() { return 'A' + rand() % 26; }

char random_left_letter_generator() {
  vector<char> leftHandLetters = {'q', 'w', 'e', 'r', 't', 'a', 's', 'd',
                                  'f', 'g', 'z', 'x', 'c', 'v', 'b'};
  int index = rand() % leftHandLetters.size();
  return leftHandLetters[index];
}

char random_right_letter_generator() {
  vector<char> rightHandLetters = {'y', 'u', 'i', 'o', 'p', 'h',
                                   'j', 'k', 'l', 'n', 'm', 'b'};
  int index = rand() % rightHandLetters.size();
  return rightHandLetters[index];
}

float random_X_value(int windowSize) { return rand() % (windowSize + 1); }

float random_left_X_value(int windowSize) {
  float padding = windowSize * 0.15f;
  float minLeftX = padding;
  float maxLeftX = (windowSize * 0.5f) - padding;
  return minLeftX + rand() % static_cast<int>(maxLeftX - minLeftX + 1);
}

float random_right_X_value(int windowSize) {
  float padding = windowSize * 0.15f;
  float minRightX = (windowSize * 0.5f) + padding;
  float maxRightX = windowSize - padding;
  return minRightX + rand() % static_cast<int>(maxRightX - minRightX + 1);
}

float random_speed() { return 300 + rand() % 601; }

struct randomLetter {
  Text randomLetterText;
  float randomLetterSpeed;
};
vector<randomLetter> randomLetters;

struct randomLeftLetter {
  Text randomLeftLetterText;
  float speed;
};
vector<randomLeftLetter> randomLeftLetters;

struct randomRightLetter {
  Text randomRightLetterText;
  float speed;
};
vector<randomRightLetter> randomRightLetters;

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

  // create menu window
  RenderWindow window(VideoMode({1920, 1080}), "Main Menu");
  window.setVerticalSyncEnabled(true);

  // create game window
  RenderWindow gameWindow(VideoMode({1920, 1080}), "KeyboardHero");
  gameWindow.setVerticalSyncEnabled(true);

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
  Clock leftspawnClock;
  Clock rightspawnClock;
  float bgSpawnInterval = 0.2f;
  float spawnInterval = 1.f;

  // start title song
  titleSong.setLooping(true);
  titleSong.play();
  bool isLooping = titleSong.isLooping();

  // start menu window loop //
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
          // ENTER -> close menu window, begin game window
          if (key->scancode == Keyboard::Scancode::Enter) {
            window.close();
            titleSong.stop();
            titleTransSound.play();
            gameWindow.display();
          }
        } // END key pressed handling
      } // END game loop event handling

      // start drawing - menu window
      window.clear();

      // draw menu titling text
      window.draw(titleText);
      window.draw(menuText);

      // create matrix-style background - menu
      if (spawnClock.getElapsedTime().asSeconds() > bgSpawnInterval) {
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
      for (auto &letter : randomLetters) {
        letter.randomLetterText.move({0, letter.randomLetterSpeed * dt});
        window.draw(letter.randomLetterText);
      }

      window.display();
    } // END delta time
  } // END menu window loop

  // start game window loop //
  while (gameWindow.isOpen()) {

    // start delta time
    float dt = gameClock.getElapsedTime().asSeconds();
    if (dt >= 1.f / 60.f) {

      gameClock.restart();

      // start event handling
      while (const auto event = gameWindow.pollEvent()) {
        if (event->is<Event::Closed>()) {
          gameWindow.close();
        }

        // keyboard input - key pressed //
        if (auto *key = event->getIf<sf::Event::KeyPressed>()) {

          // Q -> quit
          if (key->scancode == Keyboard::Scancode::Q) {
            gameWindow.close();
          }

        } // END key pressed handling
      } // END game window loop event handling

      // start drawing - game window
      gameWindow.clear();

      // make left and right hand letters fall
      if (spawnClock.getElapsedTime().asSeconds() > spawnInterval) {

        // left hand letters
        Text leftLetterText(firacode);
        char randomLeftLetter = random_left_letter_generator();
        leftLetterText.setString(randomLeftLetter);
        leftLetterText.setCharacterSize(36);
        leftLetterText.setFillColor(Color::Green);
        FloatRect leftLetterTextSize = leftLetterText.getLocalBounds();
        leftLetterText.setOrigin(
            {leftLetterTextSize.size.x / 2.f, leftLetterTextSize.size.y / 2.f});
        float randomLeftXvalue = random_left_X_value(gameWindow.getSize().x);
        leftLetterText.setPosition({randomLeftXvalue, -20.f});

        // right hand letters
        Text rightLetterText(firacode);
        char randomRightLetter = random_right_letter_generator();
        rightLetterText.setString(randomRightLetter);
        rightLetterText.setCharacterSize(36);
        rightLetterText.setFillColor(Color::Green);
        FloatRect rightLetterTextSize = rightLetterText.getLocalBounds();
        rightLetterText.setOrigin({rightLetterTextSize.size.x / 2.f,
                                   rightLetterTextSize.size.y / 2.f});
        float randomRightXvalue = random_right_X_value(gameWindow.getSize().x);
        rightLetterText.setPosition({randomRightXvalue, -20.f});

        // add left letters to vector with speed
        float leftLetterSpeed = 400.f;
        randomLeftLetters.push_back({leftLetterText, leftLetterSpeed});

        // add right letters to vector with speed
        float rightLetterSpeed = 400.f;
        randomRightLetters.push_back({rightLetterText, rightLetterSpeed});

        spawnClock.restart();
      }

      // update all falling letters
      for (auto &leftLetter : randomLeftLetters) {
        leftLetter.randomLeftLetterText.move({0, leftLetter.speed * dt});
        gameWindow.draw(leftLetter.randomLeftLetterText);
      }
      for (auto &rightLetter : randomRightLetters) {
        rightLetter.randomRightLetterText.move({0, rightLetter.speed * dt});
        gameWindow.draw(rightLetter.randomRightLetterText);
      }

      gameWindow.display();
    } // END delta time
  } // END game window loop

  return 0;
}
