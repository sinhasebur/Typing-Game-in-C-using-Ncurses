# Typing-Game-in-C-using-Ncurses
TO RUN THE GAME

1. Have the word_pool.txt file in the working directory

2. Compile
gcc -o typing_game typing_game.c -lncurses

3. Run
./typing_game < word_pool.txt

Special Features
1. Handles unusual inputs like backspace and function keys during the game
2. Changing the word_number or Gametime in the code, settings can be changed
3. Escape anytime within the game by enter
4. Play again after finishing
5. Words can be changed by changing words in the word_pool.txt file.
