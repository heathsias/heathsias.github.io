# Heath Sias (23808695)
# ICS 32 -> Lab 5

# Othello Game Logic (othello.py)


WHITE = 'W'
BLACK = 'B'
EMPTY = '-'

DIRECTIONS = [[-1, 0], [-1, 1], [0, 1], [1, 1], [1, 0], [1, -1], [0, -1], [-1, -1]]
#               'N'      'NE'     'E'    'SE'     'S'     'SW'     'W'      'NW'



### PUBLIC CLASSES


class OthelloGameState:
    ''' Create a game state representing an active game of Othello, with attributes and
        methods required to propogate the game.

        Public methods: current_player, opposing_player, rows, columns, white, black,
                        game_over, winner, make_move, get_winner
    '''
    
    def __init__(self, rows: int, columns: int, player_one: str, nw_color: str, criteria: str):       
        self._rows = rows
        self._columns = columns        
        self._current_player = player_one
        self._passes = 0
        self._game_over = False
        self._winning_criteria = criteria

        self._num_white = 2
        self._num_black = 2
        
        self._nw_se_color = nw_color
        self._game_board = self._initialize_game_board()



    ## public methods


    def current_player(self) -> str:
        ''' Return the current player's color
        '''
        return self._current_player


        
    def opposing_player(self) -> str:
        ''' Return the opposing player's color
        '''
        if self._current_player == BLACK:
            return WHITE
        
        elif self._current_player == WHITE:
            return BLACK
        
        else:
            raise ColorError



    def rows(self) -> int:
        ''' Returns the number of rows on the game board
        '''
        return self._rows



    def columns(self) -> int:
        ''' Returns the number of columns on the game board
        '''
        return self._columns

    

    def white(self) -> int:
        ''' Returns White's score
        '''
        return self._num_white



    def black(self) -> int:
        ''' Returns White's score
        '''
        return self._num_black



    def game_over(self) -> bool:
        ''' Determines if the game is over
        '''
        return self._game_over
    


    def winner(self) -> str:
        ''' Determine who won the game according to the winning criteria
        '''
        if self._num_black == self._num_white:
            return EMPTY
        
        elif self._winning_criteria == 'M':
            if self._num_black > self._num_white:
                return BLACK
            else:
                return WHITE

        elif self._winning_criteria == 'F':
            if self._num_black < self._num_white:
                return BLACK
            else:
                return WHITE

         

    def make_move(self, move: (int, int)) -> None:
        ''' Determine if move is valid: if so, place the piece, flip resulting
            enemy pieces, and update scores.
        '''
        row, column = move[0], move[1]
        
        if self._is_valid(row, column):
            self._place_piece(row, column)
            self._capture_cells(self._cells_to_capture(row, column))
        else:
            raise InvalidMoveError

        self._update_game_state()



    ## private methods


    def _update_game_state(self) -> None:
        ''' Update score and switch current player to the next player with legal moves.
            If neither player has any legal moves, game is over
        '''
        self._switch_players()
        self._update_scores()
        
        if not self._any_valid_moves():
            self._switch_players()
            
            if not self._any_valid_moves():
                self._game_over = True


        
    def _update_scores(self) -> None:
        ''' Scan the board and set each player's score equal to the number of pieces
            they own on the board
        '''
        black = 0
        white = 0
        
        for row in range(self._rows):            
            for column in range(self._columns):
                if self._game_board[row][column] == BLACK:
                    black += 1
                elif self._game_board[row][column] == WHITE:
                    white += 1
                    
        self._num_black = black
        self._num_white = white
        
        
        
    def _initialize_game_board(self) -> [list]:
        ''' Configures the game board with the center four cells occupied by diagonally
            opposing colors, the top left of those cells either WHITE or BLACK
        '''
        new_board = self._create_blank_board()

        if not self._legal_board_size():
            raise InvalidBoardSizeError
        
        nw_row = (len(new_board) // 2) - 1
        nw_col = (len(new_board[0]) // 2) - 1

        if self._nw_se_color == BLACK:
            ne_sw_color = WHITE
            
        elif self._nw_se_color == WHITE:
            ne_sw_color = BLACK
            
        else:
            raise ColorError
        
        new_board[nw_row][nw_col] = self._nw_se_color
        new_board[nw_row+1][nw_col+1] = self._nw_se_color            
        new_board[nw_row][nw_col+1] = ne_sw_color
        new_board[nw_row+1][nw_col] = ne_sw_color

        return new_board



    def _legal_board_size(self) -> bool:
        ''' Determine if the number of rows and columns are legal for a game of
            Othello, each being an even integer between 4 and 16
        '''
        if self._rows % 2 or self._columns % 2 or \
           self._rows < 4 or self._rows > 16 or \
           self._columns < 4 or self._columns > 16:
            return False
        return True


    
    def _create_blank_board(self) -> [list]:
        ''' Create a game board with the object's number of rows [i] and columns [j],
            each 'cell' occupied by a blank space, ' '
        '''
        blank_board = []
        
        for row in range(self._rows):
            blank_board.append([])
            
            for column in range(self._columns):
                blank_board[row].append(EMPTY)

        return blank_board



    def _is_valid(self, row: int, column: int) -> bool:
        ''' Determining the validity of a given move. If valid, return a list of cells which can
            be captured
        '''
        try:
            row, column = int(row), int(column)
        except:
            return False
            
        to_capture = self._cells_to_capture(row, column)
        
        if self._cell_in_bounds(row, column) and self._cell_is_empty(row, column) and len(to_capture):
            return True
        else:
            return False



    def _any_valid_moves(self) -> bool:
        ''' Determine if there are any valid moves for the current player
        '''
        for row in range(self._rows):            
            for column in range(self._columns):
                if self._is_valid(row, column):
                    return True
        return False



    def _switch_players(self) -> None:
        ''' Make opposing player the current player
        '''
        self._current_player = self.opposing_player()



    def _place_piece(self, row, column) -> None:
        ''' Place the current player's piece in the given cell
        '''
        self._game_board[row][column] = self._current_player



    def _capture_cells(self, list_of_cells: [list]) -> None:
        ''' capture a list of cells, each a two item list, [row, column]
        '''
        for cell in list_of_cells:
            self._place_piece(cell[0], cell[1])

            

    def _cell_in_bounds(self, row: int, column: int) -> bool:
        ''' Determine if a given cell is within the bounds of the game board
        '''
        return row in range(self._rows) and column in range(self._columns)



    def _cell_is_empty(self, row: int, column: int) -> bool:
        ''' Determine if a given cell is empty
        '''
        return self._game_board[row][column] == EMPTY



    def _cells_to_capture(self, row: int, column: int) -> [list]:
        ''' Returns a list of 'captured' pieces as [row, column] pairs
        ''' 
        total_to_capture = []
        
        for direction in DIRECTIONS:            
            captured = self._capture_in_direction(direction, row, column)
            
            if captured:
                total_to_capture.extend(captured)    
        return total_to_capture

        

    def _capture_in_direction(self, direction: list, row: int, column: int) -> [list]:
        ''' Returns a list of possible 'captured cells' in the given direction as
            [row, column] pairs
        '''
        candidates = []
        opposing_player = self.opposing_player()

        next_row = row + direction[0]
        next_column = column + direction[1]
        
        while self._cell_in_bounds(next_row, next_column):
            
            if self._game_board[next_row][next_column] == opposing_player:
                candidates.append([next_row, next_column])
                next_row += direction[0]
                next_column += direction[1]
                
            elif self._game_board[next_row][next_column] == self._current_player:
                return candidates
            
            elif self._game_board[next_row][next_column] == EMPTY:
                return []

   

### EXCEPTIONS
            

class InvalidBoardSizeError(Exception):
    ''' Raise when the number of rows or columns are illegal '''
    pass


class ColorError(Exception):
    ''' Raise when a color is neither BLACK nor WHITE '''
    pass

    
class InvalidMoveError(Exception):
    ''' Raise when a move is not within the confines of the board OR
        the specified cell is not empty OR placing the piece in the
        specified cell will 'capture' no enemy pieces
    '''
    pass
