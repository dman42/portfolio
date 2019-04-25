using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TankBattle
{
    /// <summary>
    /// Class that stores information about the players of each game.
    ///
    /// </summary>
    abstract public class Player
    {
        private string playerName;
        private Tank playerTank;
        private Color color;
        private int roundsWon;

        /// <summary>
        /// Constructor of class Player that sets the player's name, tank, color, and sets the rounds he/she
        /// has won to 0.
        ///
        /// </summary>
        /// <param name="name">Player's name.</param>
        /// <param name="tank">Player's tank.</param>
        /// <param name="colour">Player's colour.</param>
        public Player(string name, Tank tank, Color colour)
        {
            playerName = name;
            playerTank = tank;
            color = colour;
            roundsWon = 0;
        }

        /// <summary>
        /// Gets tank attributed to the player.
        ///
        /// </summary>
        /// <returns> Tank attributed to the player.</returns>
        public Tank GetTank()
        {
            return playerTank;
        }

        /// <summary>
        /// Gets name attributed to the player.
        ///
        /// </summary>
        /// <returns>Name attributed to the player.</returns>
        public string Name()
        {
            return playerName;
        }

        /// <summary>
        /// Gets colour attributed to the player.
        ///
        /// </summary>
        /// <returns>Colour attributed to the player.</returns>
        public Color PlayerColour()
        {
            return color;
        }

        /// <summary>
        /// Adds one to the amount of rounds the player has won.
        ///
        /// </summary>
        public void AddPoint()
        {
            roundsWon++;
        }

        /// <summary>
        /// Gets amount of rounds player has won.
        ///
        /// </summary>
        /// <returns>Amount of rounds player has won.</returns>
        public int GetWins()
        {
            return roundsWon;
        }

        public abstract void NewRound();

        public abstract void NewTurn(SkirmishForm gameplayForm, Gameplay currentGame);

        public abstract void ProjectileHit(float x, float y);
    }
}
