#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace sf;
using namespace std;

// random letter functions
char random_letter_generator() { return 'A' + rand() % 26; }

char random_left_letter_generator() {
  vector<char> leftHandLetters = {'Q', 'W', 'E', 'R', 'T', 'A', 'S',
                                  'D', 'F', 'G', 'Z', 'X', 'C', 'V'};
  int index = rand() % leftHandLetters.size();
  return leftHandLetters[index];
}

char random_right_letter_generator() {
  vector<char> rightHandLetters = {'Y', 'U', 'I', 'O', 'P', 'H',
                                   'J', 'K', 'L', 'N', 'M', 'B'};
  int index = rand() % rightHandLetters.size();
  return rightHandLetters[index];
}

// random x value functions
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

float random_spawn_float() { return 0.5 + (rand() % 151) / 100.0; }

struct randomLetter {
  Text randomLetterText;
  float randomLetterSpeed;
};
vector<randomLetter> randomLetters;

struct randomLeftLetter {
  Text randomLeftLetterText;
  float speed;
  float center = randomLeftLetterText.getLocalBounds().getCenter().y;
};
vector<randomLeftLetter> randomLeftLetters;

struct randomRightLetter {
  Text randomRightLetterText;
  float speed;
  float center = randomRightLetterText.getLocalBounds().getCenter().y;
};
vector<randomRightLetter> randomRightLetters;

bool isPaused = false;

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
  RenderWindow gameWindow(VideoMode({1920, 1080}), "KeyboardHero");
  gameWindow.setVerticalSyncEnabled(true);

  // create menu window
  RenderWindow window(VideoMode({1920, 1080}), "Main Menu");
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
  menuText.setString("ENTER -> Play\n\n  Q   -> Quit");
  menuText.setCharacterSize(36);
  menuText.setFillColor(Color::Green);
  FloatRect menuTextSize = menuText.getLocalBounds();
  menuText.setOrigin({menuTextSize.size.x / 2.f, menuTextSize.size.y / 2.f});
  menuText.setPosition({window.getSize().x / 2.f, window.getSize().y / 1.5f});

  // create game menu text
  Text gamemenuText(firacode);
  gamemenuText.setString("TAB -> Pause\n\nESC -> Quit");
  gamemenuText.setCharacterSize(36);
  gamemenuText.setFillColor(Color::White);
  FloatRect gamemenuTextSize = gamemenuText.getLocalBounds();
  gamemenuText.setOrigin(
      {gamemenuTextSize.size.x / 2.f, gamemenuTextSize.size.y / 2.f});
  gamemenuText.setPosition(
      {gameWindow.getSize().x / 2.f, gameWindow.getSize().y / 2.f});

  // create left "miss" text
  Text missLeftText(firacode);
  missLeftText.setString("Miss");
  missLeftText.setCharacterSize(42);
  missLeftText.setFillColor(Color::Red);
  FloatRect missLeftTextSize = missLeftText.getLocalBounds();
  missLeftText.setOrigin(
      {missLeftTextSize.size.x / 2.f, missLeftTextSize.size.y / 2.f});
  missLeftText.setPosition(
      {gameWindow.getSize().x / 9.5f, gameWindow.getSize().y / 2.f});
  bool showLMissMessage = false;

  // create right "miss" text
  Text missRightText(firacode);
  missRightText.setString("Miss");
  missRightText.setCharacterSize(42);
  missRightText.setFillColor(Color::Red);
  FloatRect missRightTextSize = missRightText.getLocalBounds();
  missRightText.setOrigin(
      {missRightTextSize.size.x / 2.f, missRightTextSize.size.y / 2.f});
  missRightText.setPosition(
      {gameWindow.getSize().x - (gameWindow.getSize().x / 10.f),
       gameWindow.getSize().y / 2.f});
  bool showRMissMessage = false;

  // create left "correct!" text
  Text correctLeftText(firacode);
  correctLeftText.setString("Correct!");
  correctLeftText.setCharacterSize(42);
  correctLeftText.setFillColor(Color::Cyan);
  FloatRect correctLeftTextSize = correctLeftText.getLocalBounds();
  correctLeftText.setOrigin(
      {correctLeftTextSize.size.x / 2.f, correctLeftTextSize.size.y / 2.f});
  correctLeftText.setPosition(
      {gameWindow.getSize().x / 9.5f, gameWindow.getSize().y / 2.f});
  bool showLCorrectMessage = false;

  // create right "correct!" text
  Text correctRightText(firacode);
  correctRightText.setString("Correct!");
  correctRightText.setCharacterSize(42);
  correctRightText.setFillColor(Color::Cyan);
  FloatRect correctRightTextSize = correctRightText.getLocalBounds();
  correctRightText.setOrigin(
      {correctRightTextSize.size.x / 2.f, correctRightTextSize.size.y / 2.f});
  correctRightText.setPosition(
      {gameWindow.getSize().x - (gameWindow.getSize().x / 10.f),
       gameWindow.getSize().y / 2.f});
  bool showRCorrectMessage = false;

  // create target bar for key press
  RectangleShape targetBox({static_cast<float>(gameWindow.getSize().x), 72.f});
  targetBox.setOrigin(
      {targetBox.getSize().x / 2.f, targetBox.getSize().y / 2.f});
  targetBox.setPosition(
      {gameWindow.getSize().x / 2.f,
       gameWindow.getSize().y - (gameWindow.getSize().y / 8.5f)});
  targetBox.setOutlineColor(Color::Green);
  targetBox.setOutlineThickness(1.f);
  targetBox.setFillColor(Color::Transparent);
  bool inTargetL = false;
  bool inTargetR = false;

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
  Clock leftSpawnClock;
  Clock rightSpawnClock;
  Clock leftMessageClock;
  Clock rightMessageClock;
  float bgSpawnInterval = 0.2f;
  float leftSpawnInterval = random_spawn_float();
  float rightSpawnInterval = random_spawn_float();

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
  } // END main menu window loop

  // start game window (KeyboardHero)loop //
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

          // Escape -> quit
          if (key->scancode == Keyboard::Scancode::Escape) {
            gameWindow.close();
          }

          // Tab -> pause window
          if (key->scancode == Keyboard::Scancode::Tab) {
            isPaused = !isPaused;
          }

          // left hand switch
          switch (key->scancode) {
          case Keyboard::Scancode::Q:
          case Keyboard::Scancode::W:
          case Keyboard::Scancode::E:
          case Keyboard::Scancode::R:
          case Keyboard::Scancode::T:
          case Keyboard::Scancode::A:
          case Keyboard::Scancode::S:
          case Keyboard::Scancode::D:
          case Keyboard::Scancode::F:
          case Keyboard::Scancode::G:
          case Keyboard::Scancode::Z:
          case Keyboard::Scancode::X:
          case Keyboard::Scancode::C:
          case Keyboard::Scancode::V:
            if (inTargetL == false) {
              // if theres no letters in queue to delete (spamming keys)
              if (randomLeftLetters.size() < 1) {
                showLMissMessage = true;
              } else {
                showLMissMessage = true;
                randomLeftLetters.erase(randomLeftLetters.begin());
              }
              leftMessageClock.restart();
            }
            if (inTargetL &&
                getDescription(key->scancode) ==
                    randomLeftLetters[0].randomLeftLetterText.getString()) {
              showLCorrectMessage = true;
              randomLeftLetters.erase(randomLeftLetters.begin());
              leftMessageClock.restart();
            }
            break;
          default:
            break;
          }

          // right hand switch
          switch (key->scancode) {
          case Keyboard::Scancode::Y:
          case Keyboard::Scancode::U:
          case Keyboard::Scancode::I:
          case Keyboard::Scancode::O:
          case Keyboard::Scancode::P:
          case Keyboard::Scancode::H:
          case Keyboard::Scancode::J:
          case Keyboard::Scancode::K:
          case Keyboard::Scancode::L:
          case Keyboard::Scancode::N:
          case Keyboard::Scancode::M:
          case Keyboard::Scancode::B:
            if (inTargetR == false) {
              // if theres no letters in queue to delete (spamming keys)
              if (randomRightLetters.size() < 1) {
                showRMissMessage = true;
              } else {
                showRMissMessage = true;
                randomRightLetters.erase(randomRightLetters.begin());
              }
              rightMessageClock.restart();
            }
            if (inTargetR &&
                getDescription(key->scancode) ==
                    randomRightLetters[0].randomRightLetterText.getString()) {
              showRCorrectMessage = true;
              randomRightLetters.erase(randomRightLetters.begin());
              rightMessageClock.restart();
            }
            break;
          default:
            break;
          }

        } // END key pressed handling

      } // END game window loop event handling

      // start drawing - game window
      gameWindow.clear();

      if (!isPaused) {

        gameWindow.draw(targetBox);

        // make left hand letters fall
        if (leftSpawnClock.getElapsedTime().asSeconds() > leftSpawnInterval) {

          // left hand letters
          Text leftLetterText(firacode);
          char randomLeftLetter = random_left_letter_generator();
          leftLetterText.setString(randomLeftLetter);
          leftLetterText.setCharacterSize(36);
          leftLetterText.setFillColor(Color::Green);
          FloatRect leftLetterTextSize = leftLetterText.getLocalBounds();
          leftLetterText.setOrigin({leftLetterTextSize.size.x / 2.f,
                                    leftLetterTextSize.size.y / 2.f});
          float randomLeftXvalue = random_left_X_value(gameWindow.getSize().x);
          leftLetterText.setPosition({randomLeftXvalue, -20.f});

          // add left letters to vector with speed
          float leftLetterSpeed = 300.f;
          randomLeftLetters.push_back({leftLetterText, leftLetterSpeed});

          // left letter spawn settings
          leftSpawnInterval = random_spawn_float();
          leftSpawnClock.restart();
        }

        // make right hand letters fall
        if (rightSpawnClock.getElapsedTime().asSeconds() > rightSpawnInterval) {

          // right hand letters
          Text rightLetterText(firacode);
          char randomRightLetter = random_right_letter_generator();
          rightLetterText.setString(randomRightLetter);
          rightLetterText.setCharacterSize(36);
          rightLetterText.setFillColor(Color::Green);
          FloatRect rightLetterTextSize = rightLetterText.getLocalBounds();
          rightLetterText.setOrigin({rightLetterTextSize.size.x / 2.f,
                                     rightLetterTextSize.size.y / 2.f});
          float randomRightXvalue =
              random_right_X_value(gameWindow.getSize().x);
          rightLetterText.setPosition({randomRightXvalue, -20.f});

          // add right letters to vector with speed
          float rightLetterSpeed = 300.f;
          randomRightLetters.push_back({rightLetterText, rightLetterSpeed});

          // right letter spawn settings
          rightSpawnInterval = random_spawn_float();
          rightSpawnClock.restart();
        }

        // update all falling letters
        // for each letter object in letters vector
        for (auto &leftLetter : randomLeftLetters) {
          leftLetter.randomLeftLetterText.move({0, leftLetter.speed * dt});
          gameWindow.draw(leftLetter.randomLeftLetterText);
        }
        for (auto &rightLetter : randomRightLetters) {
          rightLetter.randomRightLetterText.move({0, rightLetter.speed * dt});
          gameWindow.draw(rightLetter.randomRightLetterText);
        }

        // delete struct at index 0 when it goes out of bounds
        if (randomLeftLetters[0].randomLeftLetterText.getPosition().y >=
            gameWindow.getSize().y) {
          randomLeftLetters.erase(randomLeftLetters.begin());
          leftMessageClock.restart();
          showLMissMessage = true;
        }
        if (randomRightLetters[0].randomRightLetterText.getPosition().y >=
            gameWindow.getSize().y) {
          randomRightLetters.erase(randomRightLetters.begin());
          rightMessageClock.restart();
          showRMissMessage = true;
        }

        // show left "miss" message
        if (showLMissMessage) {
          gameWindow.draw(missLeftText);
          if (leftMessageClock.getElapsedTime().asSeconds() >= 0.25f) {
            showLMissMessage = false;
          }
        }

        // show right "miss" message
        if (showRMissMessage) {
          gameWindow.draw(missRightText);
          if (rightMessageClock.getElapsedTime().asSeconds() >= 0.25f) {
            showRMissMessage = false;
          }
        }

        // show left "correct!" message
        if (showLCorrectMessage) {
          gameWindow.draw(correctLeftText);
          if (leftMessageClock.getElapsedTime().asSeconds() >= 0.25f) {
            showLCorrectMessage = false;
          }
        }

        // show right "correct!" message
        if (showRCorrectMessage) {
          gameWindow.draw(correctRightText);
          if (rightMessageClock.getElapsedTime().asSeconds() >= 0.25f) {
            showRCorrectMessage = false;
          }
        }

        // left letter collisions with target box
        if (randomLeftLetters[0].randomLeftLetterText.getPosition().y >=
                (targetBox.getPosition().y - 99) &&
            randomLeftLetters[0].randomLeftLetterText.getPosition().y <=
                (targetBox.getPosition().y + 99)) {
          inTargetL = true;
        }
        if (randomLeftLetters[0].randomLeftLetterText.getPosition().y <
                (targetBox.getPosition().y - 99) or
            randomLeftLetters[0].randomLeftLetterText.getPosition().y >
                (targetBox.getPosition().y + 99)) {
          inTargetL = false;
        };

        // right letter collisions with target box
        if (randomRightLetters[0].randomRightLetterText.getPosition().y >=
                (targetBox.getPosition().y - 99) &&
            randomRightLetters[0].randomRightLetterText.getPosition().y <=
                (targetBox.getPosition().y + 99)) {
          inTargetR = true;
        }
        if (randomRightLetters[0].randomRightLetterText.getPosition().y <
                (targetBox.getPosition().y - 99) or
            randomRightLetters[0].randomRightLetterText.getPosition().y >
                (targetBox.getPosition().y + 99)) {
          inTargetR = false;
        };

        gameWindow.draw(gamemenuText);
        gameWindow.display();
      } // END !isPaused
    } // END delta time
  } // END game window loop

  return 0;
}
