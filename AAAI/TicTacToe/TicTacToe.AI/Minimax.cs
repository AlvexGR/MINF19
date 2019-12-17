using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;

namespace TicTacToe.AI
{
    public class Minimax
    {
        public char Player { set; get; }
        public char Computer { set; get; }
        private int _heuristic = 50;

        public Minimax()
        {

        }

        public void ClearBoard(Grid board)
        {
            foreach (Button button in board.Children)
            {
                button.Content = string.Empty;
            }
        }

        public void ChangeBoardState(Grid board, bool state)
        {
            board.IsEnabled = state;
        }

        public bool MiddleFree(Grid board)
        {
            int counter = 0;
            foreach (Button button in board.Children)
            {
                if (counter == 4 && button.Content.ToString() == string.Empty)
                {
                    return true;
                }
                counter++;
            }
            return false;
        }

        public bool ContainEmptySquare(Grid board)
        {
            foreach (Button button in board.Children)
            {
                if (button.Content.ToString() == string.Empty)
                {
                    return true;
                }
            }
            return false;
        }

        public int EvaluateBoard(Grid board)
        {
            var compLst = new List<char>();
            foreach (Button button in board.Children)
            {
                compLst.Add(button.Content.ToString() == string.Empty ? '-' : button.Content.ToString()[0]);
            }

            // Rows check
            if (compLst[0] == compLst[1] && compLst[1] == compLst[2])
            {
                if (compLst[0] == Computer)
                {
                    return _heuristic;
                }
                if (compLst[0] == Player)
                {
                    return -_heuristic;
                }
            }
            if (compLst[3] == compLst[4] && compLst[4] == compLst[5])
            {
                if (compLst[3] == Computer)
                {
                    return _heuristic;
                }
                if (compLst[3] == Player)
                {
                    return -_heuristic;
                }
            }
            if (compLst[6] == compLst[7] && compLst[7] == compLst[8])
            {
                if (compLst[6] == Computer)
                {
                    return _heuristic;
                }
                if (compLst[6] == Player)
                {
                    return -_heuristic;
                }
            }
            // Columns check
            if (compLst[0] == compLst[3] && compLst[3] == compLst[6])
            {
                if (compLst[0] == Computer)
                {
                    return _heuristic;
                }
                if (compLst[0] == Player)
                {
                    return -_heuristic;
                }
            }
            if (compLst[1] == compLst[4] && compLst[4] == compLst[7])
            {
                if (compLst[1] == Computer)
                {
                    return _heuristic;
                }
                if (compLst[1] == Player)
                {
                    return -_heuristic;
                }
            }
            if (compLst[2] == compLst[5] && compLst[5] == compLst[8])
            {
                if (compLst[2] == Computer)
                {
                    return _heuristic;
                }
                if (compLst[2] == Player)
                {
                    return -_heuristic;
                }
            }
            // Diagonals check
            if (compLst[0] == compLst[4] && compLst[4] == compLst[8])
            {
                if (compLst[0] == Computer)
                {
                    return _heuristic;
                }
                if (compLst[0] == Player)
                {
                    return -_heuristic;
                }
            }
            if (compLst[2] == compLst[4] && compLst[4] == compLst[6])
            {
                if (compLst[2] == Computer)
                {
                    return _heuristic;
                }
                if (compLst[2] == Player)
                {
                    return -_heuristic;
                }
            }
            return 0;
        }

        private int MinimaxAlgo(Grid board, int depth, bool isMax)
        {
            int score = EvaluateBoard(board);
            if (score == _heuristic)
            {
                return score - depth;
            }

            if (score == -_heuristic)
            {
                return score + depth;
            }

            if (ContainEmptySquare(board) == false)
            {
                return 0;
            }

            int best;

            if (isMax)
            {
                best = -1000;
                foreach (Button button in board.Children)
                {
                    if (button.Content.ToString() == string.Empty)
                    {
                        button.Content = Computer;
                        best = Math.Max(best, MinimaxAlgo(board, depth + 1, !isMax));
                        button.Content = string.Empty;
                    }
                }
            }
            else
            {
                best = 1000;
                foreach (Button button in board.Children)
                {
                    if (button.Content.ToString() == string.Empty)
                    {
                        button.Content = Player;
                        best = Math.Min(best, MinimaxAlgo(board, depth + 1, !isMax));
                        button.Content = string.Empty;
                    }
                }
            }
            return best;
        }

        public void MakeMoves(Grid board)
        {
            int best = -1000;
            Button buttonToPlay = new Button();
            // always play the middle board if go first
            if (MiddleFree(board))
            {
                int counter = 0;
                foreach (Button button in board.Children)
                {
                    if (counter == 4)
                    {
                        buttonToPlay = button;
                        break;
                    }
                    counter++;
                }
            }
            else
            {
                foreach (Button button in board.Children)
                {
                    if (button.Content.ToString() == string.Empty)
                    {
                        button.Content = Computer;
                        int move = MinimaxAlgo(board, 0, false);
                        button.Content = string.Empty;
                        if (move > best)
                        {
                            best = move;
                            buttonToPlay = button;
                        }
                    }
                }
            }
            buttonToPlay.Content = Computer;
        }
    }
}
