using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TankBattle
{
    /// <summary>
    /// Class that stores information about the artificial intellegence of the tanks.
    ///
    /// </summary>
    public class ComputerPlayer : Player
    {
        /// <summary>
        /// Constructor that records the name, tank type, and colour of the player.
        ///
        /// </summary>
        /// <param name="name">Name of the player.</param>
        /// <param name="tank"> Player's tank type.</param>
        /// <param name="colour"> Player's tank colour.</param>
        public ComputerPlayer(string name, Tank tank, Color colour) : base(name, tank, colour)
        {
            
        }

        public override void NewRound()
        {
            
        }

        /// <summary>
        /// At the start of a new turn, chooses the first weapon, picks a random aim and power (within legal
        /// limits), and fires.
        ///
        /// </summary>
        /// <param name="gameplayForm">The form (aka the visual elements of the game) the player inhabits.</param>
        /// <param name="currentGame">The game the player is currently playing.</param>
        public override void NewTurn(SkirmishForm gameplayForm, Gameplay currentGame)
        {
            Random rnd = new Random();
            gameplayForm.SelectWeapon(0);
            gameplayForm.Aim(rnd.Next(-90, 91));
            gameplayForm.SetForce(rnd.Next(5, 101));
            gameplayForm.Shoot(); 
        }

        public override void ProjectileHit(float x, float y)
        {
        }
    }
}
