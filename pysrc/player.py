from utils import *
import threading
import random
from time import sleep
from search_tree import SearchTree
from chesslib import *
import logging

# FIXME: Make a variable to tell if the thread failed

class Player:
    HUMAN = 'human'
    COMPUTER = 'computer'
    def __init__(self, game, color, player_type):
        self.game = game
        self.color = color
        self.type = player_type

    # FIXME: Figure out how to handle undo moves for AI without losing track
    @run_synchronously
    def undo_move(self, num=1):
        self.game.undo_move(num)


class Human(Player):
    def __init__(self, game, color):
        super().__init__(game, color, Player.HUMAN)

    @run_synchronously
    def handle_move(self, col, row, promo=None):
        self.game.handle_move((col, row), promo)


# Ai class that can analyse a game and take control of a specific color
class Computer(Player):
    def __init__(self, game, color):
        super().__init__(game, color, Player.COMPUTER)
        self.running = True
        self.search_tree = SearchTree(game)
        #self.search_tree.populate(3)

    @run_in_thread
    def start(self):
        while self.running:
            if self.game.turn != self.color:
                self.game.switch_turn_event.wait()
            else:
                self.make_move()
        logging.info('Quitting player')
        exit()

    def quit(self):
        self.running = False
        # Make sure to alert all players
        self.game.alert_players()

    def make_move(self):
        # Don't do anything on checkmate or stalemate
        if self.game.game_state == Rules.CHECKMATE or self.game.game_state == Rules.STALEMATE:
            # Pause
            self.game.switch_turn_event.wait()
            return

        LOCK.acquire()
        # Calculate 2 layers deeper if game has already started
        #if self.game.moves:
        #    self.search_tree.populate_continue(depth=2, moves_made=self.game.moves[-2:])
        self.search_tree.populate(1)

        best_move_node = self.search_tree.get_best_move()

        move = best_move_node.move
        promo = best_move_node.promo

        self.game.full_move(move[0], move[1], promo)

        LOCK.release()

