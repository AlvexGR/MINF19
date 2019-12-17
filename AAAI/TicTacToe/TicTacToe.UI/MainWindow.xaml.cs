using System.Windows;
using System.Windows.Controls;
using TicTacToe.AI;

namespace TicTacToe.UI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        #region Properties 

        private readonly Minimax ai;

        #endregion

        public MainWindow()
        {
            InitializeComponent();
            ai = new Minimax();
            ai.ChangeBoardState(BoardGrid, false);
        }

        private void MakeMoves(object sender, RoutedEventArgs e)
        {
            (sender as Button).Content = ai.Player;
            if (ValidateGame())
            {
                ComputerMove();
            }
        }

        private bool ValidateGame()
        {
            if (ai.EvaluateBoard(BoardGrid) > 0)
            {
                ResultTextBlock.Text = "Result: Computer wins!";
                return false;
            }
            if (ai.EvaluateBoard(BoardGrid) < 0)
            {
                ResultTextBlock.Text = "Result: You wins!";
                return false;
            }
            if (!ai.ContainEmptySquare(BoardGrid))
            {
                ResultTextBlock.Text = "Result: Tie!";
                return false;
            }
            return true;
        }

        private void PlayButton_Click(object sender, RoutedEventArgs e)
        {
            if (PlayButton.Content.ToString() == "Play")
            {
                ai.ClearBoard(BoardGrid);
                ai.ChangeBoardState(BoardGrid, true);
                ResultTextBlock.Text = "Result: ???";
                PlayButton.Content = "Stop";
                if (PlayerCombobox.Text == "You are X")
                {
                    ai.Player = 'X';
                    ai.Computer = 'O';
                }
                else
                {
                    ai.Player = 'O';
                    ai.Computer = 'X';
                    ComputerMove();
                }
            }
            else
            {
                ai.ChangeBoardState(BoardGrid, false);
                PlayButton.Content = "Play";
            }
        }

        private void ComputerMove()
        {
            ai.ChangeBoardState(BoardGrid, false);
            ai.MakeMoves(BoardGrid);
            ai.ChangeBoardState(BoardGrid, true);
            ValidateGame();
        }
    }
}
