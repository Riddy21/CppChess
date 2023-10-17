import pygame
import popup
from player import *
from utils import run_in_thread
from chesslib import *
import logging

# Set up the colors
TAN = (236, 235, 205)
GREEN = (104, 139, 80)
GREY = (200, 200, 200)
RED = (255, 200, 200)

class ChessboardGUI:
    def __init__(self, api, p1, p2, interactive=True):
        if p1.color == p2.color:
            raise RuntimeError("Player colors cannot be the same")
        self.api = api
        self.p1 = p1
        self.p2 = p2

        # If popups and user prompts are to be created
        self.interactive=interactive

        # Initialize Pygame
        pygame.init()

        # Set up the window
        self.WIDTH = 400
        self.HEIGHT = 400
        self.window = pygame.display.set_mode((self.WIDTH, self.HEIGHT))
        pygame.display.set_caption("Pygame Chessboard")

        # Define the size of each square
        self.SQUARE_SIZE = self.WIDTH // BOARD_WIDTH

        self.piece_images = {
                (ROOK, BLACK): pygame.image.load("./assets/Chess_tile_rd.png"),
                (KNIGHT, BLACK): pygame.image.load("./assets/Chess_tile_nd.png"),
                (BISHOP, BLACK): pygame.image.load("./assets/Chess_tile_bd.png"),
                (QUEEN, BLACK): pygame.image.load("./assets/Chess_tile_qd.png"),
                (KING, BLACK): pygame.image.load("./assets/Chess_tile_kd.png"),
                (PAWN, BLACK): pygame.image.load("./assets/Chess_tile_pd.png"),
                (ROOK, WHITE): pygame.image.load("./assets/Chess_tile_rl.png"),
                (KNIGHT, WHITE): pygame.image.load("./assets/Chess_tile_nl.png"),
                (BISHOP, WHITE): pygame.image.load("./assets/Chess_tile_bl.png"),
                (QUEEN, WHITE): pygame.image.load("./assets/Chess_tile_ql.png"),
                (KING, WHITE): pygame.image.load("./assets/Chess_tile_kl.png"),
                (PAWN, WHITE): pygame.image.load("./assets/Chess_tile_pl.png"),
                }

    def draw_board(self):
        # Clear the window
        for row in range(BOARD_HEIGHT):
            for col in range(BOARD_WIDTH):
                color = GREEN if (row%2 == 1 and col%2 == 0) or (row%2 == 0 and col%2 == 1)else TAN
                col, row = self.orient((col, row))
                x = col * self.SQUARE_SIZE
                y = row * self.SQUARE_SIZE
                rect = pygame.Rect(x, y, self.SQUARE_SIZE, self.SQUARE_SIZE)
                pygame.draw.rect(self.window, color, rect)

    # Draw the highlights of the board for next move
    def draw_poss_moves(self, locations):
        for col, row in locations:
            col, row = self.orient((col, row))
            self.draw_dot(col, row)

    # Draws the highligh on the king that is in check
    def draw_check_highlight(self, turn, game_state):
        if game_state != Rules.CHECK:
            return

        if BLACK == turn:
            coords = self.api.get_piece_coords('k')
        elif WHITE == turn:
            coords = self.api.get_piece_coords('K')
        if len(coords) != 1:
            logging.error('more than one king')
            return
        for col, row in coords:
            col, row = self.orient((col, row))
            self.draw_highlight(col, row)

    # draw the pieces from the game object
    def draw_pieces(self, board):
        # Draw the chess pieces
        for row in range(BOARD_HEIGHT):
            for col in range(BOARD_WIDTH):
                piece = (board[col, row].type, board[col, row].color)
                self.draw_piece(piece, col, row)

    # Draw highlight on piece
    def draw_highlight(self, col, row):
        x = col * self.SQUARE_SIZE
        y = row * self.SQUARE_SIZE
        rect = pygame.Rect(x, y, self.SQUARE_SIZE, self.SQUARE_SIZE)
        pygame.draw.rect(self.window, RED, rect)

    # Draw dot on pieces
    def draw_dot(self, col, row):
        x = col * self.SQUARE_SIZE + 0.5 * self.SQUARE_SIZE
        y = row * self.SQUARE_SIZE + 0.5 * self.SQUARE_SIZE
        rect = pygame.draw.circle(self.window, GREY, (x, y), self.SQUARE_SIZE/7)

    def draw_piece(self, piece, col, row):
        piece_image = self.piece_images.get(piece)
        if piece_image:
            col, row = self.orient((col, row))
            x = col * self.SQUARE_SIZE
            y = row * self.SQUARE_SIZE
            width, height = piece_image.get_size()
            self.window.blit(piece_image, (x+(self.SQUARE_SIZE- width)/2, y+(self.SQUARE_SIZE-height)/2))

    def handle_click(self, pos, player):
        # Calculate the clicked square
        col = pos[0] // self.SQUARE_SIZE
        row = pos[1] // self.SQUARE_SIZE

        # Get the chessboard position in algebraic notation
        position = f"{chr(97+col)}{8-row}"

        # Call the API method to interact with the chessboard
        col, row = self.orient((col, row))

        # If it is a pawn promo, pull up the UI to do it
        promo = self.check_promo((col, row))

        player.handle_move(col, row, promo)

    def check_promo(self, pos):
        # If its not a valid move
        if pos not in self.api.get_current_poss_moves():
            return None

        if self.api.is_pawn_promo(self.api.selected_coord, pos):
            return self.prompt_promo()

        return None

    def prompt_mate_quit(self, game_state):
        if game_state == Rules.CHECKMATE and self.api.turn == BLACK:
            ans = popup.askyesno(title="Checkmate!",
                                 message="Checkmate! White wins!\nWould you like to quit?")
        elif game_state == Rules.CHECKMATE and self.api.turn == WHITE:
            ans = popup.askyesno(title="Checkmate!",
                                 message="Checkmate! Black wins!\nWould you like to quit?")
        elif game_state == Rules.STALEMATE:
            ans = popup.askyesno(title="Stalemate!",
                                 message="Stalemate!\nWould you like to quit?")
        else:
            ans = False
            return ans

        if not ans:
            self.get_current_player().undo_move(2)
        return ans

    def get_current_player(self):
        if self.api.turn == self.p1.color:
            return self.p1
        else:
            return self.p2

    def get_prev_player(self):
        if self.api.turn == self.p1.color:
            return self.p2
        else:
            return self.p1

    def prompt_promo(self):
        promo_dict = {'Queen' : QUEEN,
                      'Rook'  : ROOK,
                      'Knight': KNIGHT,
                      'Bishop': BISHOP,
                      }
        ans = popup.askchoice(title="Promotion",
                              message="Choose Piece",
                              options=promo_dict.keys(),
                              default='Queen')
        return promo_dict[ans]


    def orient(self, coords):
        # Don't flip if comp vs comp
        if self.get_current_player().type == Player.COMPUTER and\
                self.get_prev_player().type == Player.COMPUTER:
            return coords

        # don't flip if computer is black
        if self.get_current_player().type == Player.COMPUTER and\
                self.api.turn == BLACK:
            return coords

        # flip if computer is white
        if self.get_current_player().type == Player.COMPUTER and\
                self.api.turn == WHITE:
            return 7-coords[0], 7-coords[1]
        
        # Alternate turns
        if self.api.turn == WHITE:
            return coords
        
        # alternate turns
        if self.api.turn == BLACK:
            return 7-coords[0], 7-coords[1]

    def run(self):
        # Main game loop
        running = True

        try:
            while running:
                current_player = self.get_current_player()
                for event in pygame.event.get():
                    if event.type == pygame.QUIT:
                        running = False
                    elif event.type == pygame.MOUSEBUTTONDOWN:
                        if self.interactive:
                            # Get the position of the mouse click
                            pos = pygame.mouse.get_pos()
                            if current_player.type == Player.HUMAN:
                                self.handle_click(pos, current_player)

                    elif event.type == pygame.KEYDOWN:
                        # Ctrl-Z was pressed
                        if event.key == pygame.K_z and \
                                (pygame.key.get_mods() & pygame.KMOD_CTRL or \
                                 pygame.key.get_mods() & pygame.KMOD_META):
                            if self.interactive:
                                if current_player.type == Player.HUMAN:
                                    if Player.COMPUTER in (self.p1.type, self.p2.type):
                                        self.get_current_player().undo_move(2)
                                    else:
                                        self.get_current_player().undo_move(1)
                        # Save if Ctrl-S is pressed
                        if event.key == pygame.K_s and \
                                (pygame.key.get_mods() & pygame.KMOD_CTRL or \
                                 pygame.key.get_mods() & pygame.KMOD_META):

                            import datetime
                            filename = str(datetime.datetime.now()).replace(' ', '-')+".txt"
                            self.api.export_board(filename)
                            logging.info('Game saved in "%s"!' % filename)


                # Draw the chess board
                self.draw_board()

                # If in check, draw the error highlights
                self.draw_check_highlight(self.api.turn, self.api.game_state)

                # Draw the pieces
                self.draw_pieces(self.api.board)

                # Draw markers for next moves
                self.draw_poss_moves(self.api.get_current_poss_moves())

                # Update the display
                pygame.display.flip()

                if self.interactive:
                    if self.prompt_mate_quit(self.api.game_state):
                        running = False

                    # FIXME: Fix prompt promo
                    #self.prompt_promo()

                # Have AI do move if ai is enabled
                #if self.get_current_player().type == Player.COMPUTER:
                #        self.get_current_player().make_move()

        except KeyboardInterrupt:
            pass
        self.quit()

    def quit(self):
        # Quit the game
        pygame.quit()
        # only when non-interactive mode
        if self.interactive:
            if self.p1.type == Player.COMPUTER:
                self.p1.quit()
            if self.p2.type == Player.COMPUTER:
                self.p2.quit()
            self.api.quit()
