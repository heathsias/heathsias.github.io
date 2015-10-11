# Heath Sias (23808695)
# ICS 32 -> Lab 5

# Othello w/GUI (play_othello.py)


import othello
import tkinter



### PUBLIC CLASSES


class OthelloApplication:
    def __init__(self):
        ''' _______________
        '''
        self.game_state = None
        self.game_grid = []
        
        self.root_window = tkinter.Tk()
        self.root_window.title('Othello v.0.5 by Heath Sias')
        self.root_window.resizable(width = False, height = False)

        self.num_rows = 1        
        self.num_cols = 1

        self.player_one = 'player_one'
        self.player_two = 'player_two'
        self.nw_corner = 'nw_corner'
        self.criteria = 'criteria'
        
        self.player_one_string = tkinter.StringVar()
        self.player_one_string.set('P1:')

        self.player_one_score = tkinter.StringVar()
        self.player_one_score.set('0')
        
        self.player_two_string = tkinter.StringVar()
        self.player_two_string.set('P2:')

        self.player_two_score = tkinter.StringVar()
        self.player_two_score.set('0')
               
        self.criteria_string = tkinter.StringVar()
        self.criteria_string.set('v.0.5')


        ## widgets

        self.canvas = tkinter.Canvas(
            master = self.root_window,
            width = 300, height = 300,
            background = '#008000',
            borderwidth = -3,
            relief = 'flat')
       
        self.status_frame = tkinter.Frame(
            master = self.root_window,
            width = 200, height = 600)

        self.title_frame = tkinter.Frame(
            master = self.root_window,
            width = 200, height = 600)

        TITLE = ['O','T','H','E','L','L','O']        
        for number in range(1, 8):
            title_letter = tkinter.Label(
                master = self.title_frame,
                text = '{}'.format(TITLE[number-1]),
                font = ('Helvetica', 24),
                borderwidth = 0)            
            title_letter.grid(
                row = number - 1, column = 0, padx = 0, pady = 0,
                sticky = tkinter.E + tkinter.W)

        self.player1_label = tkinter.Label(
            master = self.status_frame,
            textvariable = self.player_one_string,
            font = ('Helvetica', 18),
            anchor = 'e')

        self.player1_score_label = tkinter.Label(
            master = self.status_frame, width = 2,
            textvariable = self.player_one_score,
            font = ('Helvetica', 18),
            anchor = 'w')

        self.player2_label = tkinter.Label(
            master = self.status_frame,
            textvariable = self.player_two_string,
            font = ('Helvetica', 18),
            anchor = 'e')

        self.player2_score_label = tkinter.Label(
            master = self.status_frame, width = 2,
            textvariable = self.player_two_score,
            font = ('Helvetica', 18),
            anchor = 'w')

        self.criteria_label = tkinter.Label(
            master = self.status_frame,
            textvariable = self.criteria_string,
            font = ('Helvetica', 16))

        self.start_button = tkinter.Button(
            master = self.root_window,
            text = 'START', font = ('Helvetica', 18),
            command = self._on_start_button)


        ## layout
        
        self.title_frame.grid(
            row = 0, column = 0, padx = (30,20), pady = 0,
            sticky = tkinter.N + tkinter.S + tkinter.E + tkinter.W)
        
        self.canvas.grid(
            row = 0, column = 1, padx = 10, pady = 0,
            sticky = tkinter.N + tkinter.S + tkinter.E + tkinter.W)
        
        self.status_frame.grid(
            row = 0, column = 2, padx = (0,10), pady = 0,
            sticky = tkinter.N + tkinter.S + tkinter.E + tkinter.W)

        self.player1_label.grid(
            row = 0, column = 0, padx = 10, pady = 10,
            sticky = tkinter.N + tkinter.S + tkinter.E)
        
        self.player1_score_label.grid(
            row = 0, column = 1, padx = 0, pady = 10,
            sticky = tkinter.N + tkinter.S + tkinter.W)

        self.player2_label.grid(
            row = 1, column = 0, padx = 10, pady = 10,
            sticky = tkinter.N + tkinter.S + tkinter.E)

        self.player2_score_label.grid(
            row = 1, column = 1, padx = 0, pady = 10,
            sticky = tkinter.N + tkinter.S + tkinter.W)

        self.criteria_label.grid(
            row = 2, column = 0, columnspan = 2,
            padx = 10, pady = 10,
            sticky = tkinter.N + tkinter.S + tkinter.E + tkinter.W)

        self.start_button.grid(
            row = 0, column = 1, padx = 10, pady = 10)


        ## behavior
            
#        self.root_window.minsize(width = 330, height = 300)

        self.root_window.rowconfigure(0, weight = 1)
        
        self.root_window.columnconfigure(0, weight = 0)
        self.root_window.columnconfigure(1, weight = 3)
        self.root_window.columnconfigure(2, weight = 0)

        self.title_frame.columnconfigure(0, weight = 0)
        for title_row in range(0, 7):
            self.title_frame.rowconfigure(title_row, weight = 1)

        self.status_frame.rowconfigure(0, weight = 1)
        self.status_frame.rowconfigure(1, weight = 1)
        self.status_frame.rowconfigure(2, weight = 1)
        
        self.status_frame.columnconfigure(0, weight = 0)
        self.status_frame.columnconfigure(1, weight = 0)

                
    ## public methods

    def start(self) -> None:
        ''' _______________
        '''
        self.root_window.mainloop()



    ## private methods

    def _on_start_button(self) -> None:
        ''' _______________
        '''
        dialog = PreferenceDialog()
        dialog.show()

        if dialog.was_ok_clicked():
            self.num_rows = dialog.get_num_rows()
            self.num_cols = dialog.get_num_cols()
            
            self.player_one_string.set('{}:'.format(dialog.get_player_one()))
            self.player_one = dialog.get_player_one()[0]
            
            self.player_two_string.set('{}:'.format(dialog.get_player_two()))
            self.player_two = dialog.get_player_two()[0]
            
            self.nw_corner = dialog.get_nw_corner()
            
            self.criteria_string.set('Win by:\n{}'.format(dialog.get_criteria()))
            self.criteria = dialog.get_criteria()[0]
            
            self.start_button.grid_remove()

            self.game_state = othello.OthelloGameState(
                self.num_rows, self.num_cols, self.player_one, self.nw_corner, self.criteria)
            
            self._start_game()


    def _start_game(self) -> None:
        '''
        '''
        total_rows_and_columns = self.num_rows + self.num_cols
        
        row_ratio = self.num_rows / total_rows_and_columns
        col_ratio = self.num_cols / total_rows_and_columns

        game_canvas_width = int(245 + 800 * col_ratio)
        game_canvas_height = int(800 * row_ratio)

        self.root_window.geometry("{}x{}".format(game_canvas_width, game_canvas_height))
        
        self._draw_game_board()
        self._update_labels()
        
        self.canvas.bind('<Configure>', self._on_canvas_resized)
        self.canvas.bind('<Button-1>', self._on_canvas_clicked)


    def _draw_game_board(self) -> None:
        ''' Draw the game grid and pieces
        '''
        self.canvas.delete(tkinter.ALL)
        
        width = self.canvas.winfo_width()
        height = self.canvas.winfo_height()
        
        horizontals = []
        for row_line in range(self.num_rows + 1):
            horizontals.append((row_line/self.num_rows) * height)

        verticals = []
        for col_line in range(self.num_cols + 1):
            verticals.append((col_line/self.num_cols) * width)

        for y_coordinate in horizontals:
            self.canvas.create_line(0, y_coordinate, width, y_coordinate)

        for x_coordinate in verticals:
            self.canvas.create_line(x_coordinate, 0, x_coordinate, height)
        
        for row in range(self.num_rows):           
            for col in range(self.num_cols):
                if self.game_state._game_board[row][col] == othello.BLACK:
                    self.canvas.create_oval(
                        (col/self.num_cols) * width + 3,
                        (row/self.num_rows) * height + 3,
                        ((col+1)/self.num_cols) * width - 3,
                        ((row+1)/self.num_rows) * height - 3,
                        outline = 'black', fill = 'black')

                elif self.game_state._game_board[row][col] == othello.WHITE:
                    self.canvas.create_oval(
                        (col/self.num_cols) * width + 3,
                        (row/self.num_rows) * height + 3,
                        ((col+1)/self.num_cols) * width - 3,
                        ((row+1)/self.num_rows) * height - 3,
                        outline = 'black', fill = 'white')
                

    def _on_canvas_resized(self, event: tkinter.Event) -> None:
        '''
        '''
        self._draw_game_board()


    def _on_canvas_clicked(self, event: tkinter.Event) -> None:
        '''
        '''
        width = self.canvas.winfo_width()
        height = self.canvas.winfo_height()

        col = int(event.x/(width/self.num_cols))
        row = int(event.y/(height/self.num_rows))

        try:
            self.game_state.make_move((row, col))
            self._next_turn()
        except:
            pass

    
    def _next_turn(self) -> None:
        '''
        '''
        self._draw_game_board()
        self._update_labels()
        if self.game_state.game_over():
            self._show_winner()
            self.start_button.grid()
  
            
    def _update_labels(self) -> None:
        ''' Change player labels to highlight who's turn it is and update scores
        '''
        if self.player_one == othello.WHITE:
            self.player_one_score.set(str(self.game_state.white()))
            self.player_two_score.set(str(self.game_state.black()))                          
        else:
            self.player_one_score.set(str(self.game_state.black()))
            self.player_two_score.set(str(self.game_state.white()))
            
        if self.game_state.current_player() == self.player_one:                           
            self.player1_label.config(font = ('Helvetica', 18), fg = 'blue')                        
            self.player2_label.config(font = ('Helvetica', 18), fg = 'black')                        
        else:
            self.player2_label.config(font = ('Helvetica', 18), fg = 'blue')                       
            self.player1_label.config(font = ('Helvetica', 18), fg = 'black')            

            
    def _show_winner(self) -> None:
        '''
        '''
        if self.game_state.winner() == othello.BLACK:
            self.criteria_string.set('Black Wins!')

        else:
            self.criteria_string.set('White Wins!')


class PreferenceDialog:
    def __init__(self):
        ''' ________________
        '''
        self.dialog_window = tkinter.Toplevel()
        self.dialog_window.resizable(width = False, height = False)
        self.ok_clicked = False

        self.row_choice = tkinter.IntVar()
        self.row_choice.set(1)
        
        self.col_choice = tkinter.IntVar()
        self.col_choice.set(1)
        
        self.player_choice = tkinter.StringVar()
        self.player_choice.set('DPlayer1')
        
        self.corner_choice = tkinter.StringVar()
        self.corner_choice.set('DNWCorner')
        
        self.criteria_choice = tkinter.StringVar()
        self.criteria_choice.set('DCriteria')

        ## widgets

        self.top_frame = tkinter.Frame(
            master = self.dialog_window,
            width = 200, height = 50)
        
        self.button_frame = tkinter.Frame(
            master = self.dialog_window,
            width = 200, height = 50)

        ok_button = tkinter.Button(
            master = self.button_frame, text = 'OK',
            width = 10,
            command = self._on_ok_button)

        cancel_button = tkinter.Button(
            master = self.button_frame, text = 'Cancel',
            command = self._on_cancel_button)


        row_label = tkinter.Label(
            master = self.top_frame, text = 'Number of rows')

        row_radio1 = tkinter.Radiobutton(
            master = self.top_frame, text = '4',
            variable = self.row_choice, value = 4)

        row_radio2 = tkinter.Radiobutton(
            master = self.top_frame, text = '6',
            variable = self.row_choice, value = 6)

        row_radio3 = tkinter.Radiobutton(
            master = self.top_frame, text = '8',
            variable = self.row_choice, value = 8)

        row_radio4 = tkinter.Radiobutton(
            master = self.top_frame, text = '10',
            variable = self.row_choice, value = 10)

        row_radio5 = tkinter.Radiobutton(
            master = self.top_frame, text = '12',
            variable = self.row_choice, value = 12)

        row_radio6 = tkinter.Radiobutton(
            master = self.top_frame, text = '14',
            variable = self.row_choice, value = 14)

        row_radio7 = tkinter.Radiobutton(
            master = self.top_frame, text = '16',
            variable = self.row_choice, value = 16)

           
        col_label = tkinter.Label(
            master = self.top_frame, text = 'Number of columns')

        col_radio1 = tkinter.Radiobutton(
            master = self.top_frame, text = '4',
            variable = self.col_choice, value = 4)
            
        col_radio2 = tkinter.Radiobutton(
            master = self.top_frame, text = '6',
            variable = self.col_choice, value = 6)
            
        col_radio3 = tkinter.Radiobutton(
            master = self.top_frame, text = '8',
            variable = self.col_choice, value = 8)
            
        col_radio4 = tkinter.Radiobutton(
            master = self.top_frame, text = '10',
            variable = self.col_choice, value = 10)
            
        col_radio5 = tkinter.Radiobutton(
            master = self.top_frame, text = '12',
            variable = self.col_choice, value = 12)
            
        col_radio6 = tkinter.Radiobutton(
            master = self.top_frame, text = '14',
            variable = self.col_choice, value = 14)
            
        col_radio7 = tkinter.Radiobutton(
            master = self.top_frame, text = '16',
            variable = self.col_choice, value = 16)
            

        p1_label = tkinter.Label(
            master = self.dialog_window, text = 'Player to start')

        p1_radio1 = tkinter.Radiobutton(
            master = self.dialog_window, text = 'Black',
            variable = self.player_choice, value = 'Black')

        p1_radio2 = tkinter.Radiobutton(
            master = self.dialog_window, text = 'White',
            variable = self.player_choice, value = 'White')

        
        nw_label = tkinter.Label(
            master = self.dialog_window, text = 'NW starting square')

        nw_radio1 = tkinter.Radiobutton(
            master = self.dialog_window, text = 'Black',
            variable = self.corner_choice, value = othello.BLACK)

        nw_radio2 = tkinter.Radiobutton(
            master = self.dialog_window, text = 'White',
            variable = self.corner_choice, value = othello.WHITE)

        
        crit_label = tkinter.Label(
            master = self.dialog_window, text = 'Win by')

        crit_radio1 = tkinter.Radiobutton(
            master = self.dialog_window, text = 'Most Pieces',
            variable = self.criteria_choice, value = 'Most')
        
        crit_radio2 = tkinter.Radiobutton(
            master = self.dialog_window, text = 'Fewest Pieces',
            variable = self.criteria_choice, value = 'Fewest')


        ## layout

        # top_frame
        self.top_frame.grid(
            row = 0, column = 0, columnspan = 4, padx = 10, pady = 10,
            sticky = tkinter.N + tkinter.S + tkinter.E + tkinter.W)
        
        row_label.grid(row = 0, column = 0, sticky = 'E', padx = (40,0))
        row_radio1.grid(row = 0, column = 1, sticky = 'W')
        row_radio2.grid(row = 0, column = 2, sticky = 'W')
        row_radio3.grid(row = 0, column = 3, sticky = 'W')
        row_radio4.grid(row = 0, column = 4, sticky = 'W')
        row_radio5.grid(row = 0, column = 5, sticky = 'W')
        row_radio6.grid(row = 0, column = 6, sticky = 'W')
        row_radio7.grid(row = 0, column = 7, sticky = 'W')
        
        col_label.grid(row = 1, column = 0, sticky = 'E')
        col_radio1.grid(row = 1, column = 1, sticky = 'W')
        col_radio2.grid(row = 1, column = 2, sticky = 'W')
        col_radio3.grid(row = 1, column = 3, sticky = 'W')
        col_radio4.grid(row = 1, column = 4, sticky = 'W')
        col_radio5.grid(row = 1, column = 5, sticky = 'W')
        col_radio6.grid(row = 1, column = 6, sticky = 'W')
        col_radio7.grid(row = 1, column = 7, sticky = 'W')

        # middle (dialog_window)
        p1_label.grid(row = 1, column = 0, sticky = 'E', padx = (63.5,0))
        p1_radio1.grid(row = 1, column = 1, sticky = 'W')
        p1_radio2.grid(row = 2, column = 1, sticky = 'W')

        nw_label.grid(row = 1, column = 2, sticky = 'E')
        nw_radio1.grid(row = 1, column = 3, sticky = 'W')
        nw_radio2.grid(row = 2, column = 3, sticky = 'W', padx = (0,20))

        crit_label.grid(row = 3, column = 0, sticky = 'E', pady = (10,0))
        crit_radio1.grid(row = 3, column = 1, sticky = 'W', pady = (10,0))
        crit_radio2.grid(row = 4, column = 1, sticky = 'W')

        # button_frame
        self.button_frame.grid(
            row = 5, column = 0, columnspan = 5, padx = 10, pady = 10,
            sticky = tkinter.N + tkinter.S + tkinter.E)

        ok_button.grid(row = 0, column = 0, padx = (0,5))
        cancel_button.grid(row = 0, column = 1, padx = (5,0))


    ## public methods

    def show(self) -> None:
        ''' Give control to the dialog box and make it modal
        '''
        self.dialog_window.grab_set()
        self.dialog_window.wait_window()

    def was_ok_clicked(self) -> bool:
        ''' Determine in OK was clicked'''
        return self.ok_clicked

    def get_num_rows(self) -> int:
        ''' Return number of rows'''
        return self.num_rows

    def get_num_cols(self) -> int:
        ''' Return number of columns'''        
        return self.num_cols

    def get_player_one(self) -> str:
        ''' Return who will play first'''
        return self.player_one

    def get_player_two(self) -> str:
        ''' Return who will play second'''
        if self.player_one == 'White':
            return 'Black'
        elif self.player_one == 'Black':
            return 'White'

    def get_nw_corner(self) -> str:
        ''' Return the player who will begin with pieces in the
            NW and SW corners of the four central starting squares
        '''
        return self.nw_corner

    def get_criteria(self) -> str:
        ''' Return the winning criteria'''
        return self.criteria



    ## private methods

    def _on_ok_button(self) -> None:
        if (self.row_choice.get() in [4,6,8,10,12,14,16]) and\
           (self.col_choice.get() in [4,6,8,10,12,14,16]) and\
           (self.player_choice.get() in ['White','Black']) and\
           (self.corner_choice.get() in ['W','B']) and\
           (self.criteria_choice.get() in ['Most','Fewest']):
            
            self.ok_clicked = True
            self.num_rows = self.row_choice.get()
            self.num_cols = self.col_choice.get()
            self.player_one = self.player_choice.get()
            self.nw_corner = self.corner_choice.get()
            self.criteria = self.criteria_choice.get()
            self.dialog_window.destroy()
            
        else:
            self._on_cancel_button()
            

    def _on_cancel_button(self) -> None:
        self.dialog_window.destroy()


        

### DRIVER
        
if __name__ == '__main__':
    OthelloApplication().start()
    
