using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;

namespace TankBattle
{
    /// <summary>
    /// Class that stores information about the different kinds of blasts and shells.
    ///
    /// </summary>
    public abstract class Attack
    {
        protected Gameplay currentGame;

        /// <summary>
        /// Stores the current gameplay so the classes 'Blast' and 'Shell' can use it.
        ///
        /// </summary>
        /// <param name="game">Current game</param>
        public void RecordCurrentGame(Gameplay game)
        {
            currentGame = game;
        }

        public abstract void Tick();
        public abstract void Display(Graphics graphics, Size displaySize);
    }
}
