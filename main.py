from gui import ChessboardGUI
from game import Game
from player import Human
from chesslib import *

def main():
    game = Game()
    player1 = Human(game=game, color=BLACK)
    player2 = Human(game=game, color=WHITE)
    gui = ChessboardGUI(api=game, p1=player1, p2=player2)

    gui.run()

if __name__ == '__main__':
    main()

