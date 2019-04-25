using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;

namespace TankBattle
{
    /// <summary>
    /// Class that stores information about the player controller.
    ///
    /// </summary>
    public class PlayerController : Player
    {
        /// <summary>
        /// Constructor that records the name, tank type, and colour of the player.
        ///
        /// </summary>
        /// <param name="name">Name of the player.</param>
        /// <param name="tank"> Player's tank type.</param>
        /// <param name="colour"> Player's tank colour.</param>
        public PlayerController(string name, Tank tank, Color colour) : base(name, tank, colour)
        {
        }

        public override void NewRound()
        {
        }

        /// <summary>
        /// Enables tank controls when the players turn begins.
        ///
        /// </summary>
        /// <param name="gameplayForm">The form (aka the visual elements of the game) the player inhabits.</param>
        /// <param name="currentGame">The game the player is currently playing.</param>
        public override void NewTurn(SkirmishForm gameplayForm, Gameplay currentGame) 
        {
            gameplayForm.EnableTankControls();
        }

        public override void ProjectileHit(float x, float y)
        {
        }
    }
}
