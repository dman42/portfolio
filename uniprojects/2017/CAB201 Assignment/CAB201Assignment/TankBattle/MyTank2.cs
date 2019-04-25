using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TankBattle
{
    /// <summary>
    /// One of the kinds of tanks that can be drawn, with its own style, weapons, power, etc.
    ///
    /// </summary>
    class MyTank2 : Tank
    {
        /// <summary>
        /// Draws the tank as a number of 1's within a 2D array, including the turret, which is pointed depending on its angle.
        ///
        /// </summary>
        /// <param name="angle">Angle of the turret.</param>
        /// <returns>The array the tank is drawn within.</returns>
        public override int[,] DrawTank(float angle) 
        {
            int[,] graphic = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                   { 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
                   { 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
                   { 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0 },
                   { 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0 },
                   { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

            // Works out how to draw the turret based on the angle of the turret.
            double angledX = Math.Sin((angle * Math.PI) / 180);
            double angledY = -(Math.Cos((angle * Math.PI) / 180));

            LineDraw(graphic, 7, 6, (int) (7 + (5*angledX)), (int) (6 + (5*angledY)));

            return graphic;
        }

        /// <summary>
        /// Gets the tank's initial health.
        ///
        /// </summary>
        /// <returns>The tank's initial health.</returns>
        public override int GetTankHealth()
        {
            return 50;
        }

        /// <summary>
        /// Gets the tank's weapons.
        ///
        /// </summary>
        /// <returns>The tank's weapons.</returns>
        public override string[] ListWeapons() 
        {
            return new string[] {"Standard shell", "Terraranator", "Triple Trouble"};
        }

        /// <summary>
        /// Shoots the tank's weapon, giving the tank's own kind of blast and shell.
        ///
        /// </summary>
        /// <param name="weapon">Weapon used to shoot.</param>
        /// <param name="playerTank">Player's Tank that corresponds with this tank.</param>
        /// <param name="currentGame">Game the tank is currently being used in.</param>
        public override void ShootWeapon(int weapon, PlayerTank playerTank, Gameplay currentGame) 
        {
            float coordX = (float) (playerTank.GetXPos() + (Tank.WIDTH/2));
            float coordY = (float) (playerTank.YPos() + (Tank.HEIGHT / 2));
            Player player = playerTank.Player();
            if (weapon == 1)
            {
                Blast blast = new Blast(150, 4, 8);
                Shell shell2 = new Shell(coordX, coordY, playerTank.GetAim(), playerTank.GetTankPower(), 0.01f, blast, player);
                currentGame.AddAttack(shell2);
            }

            else if (weapon == 2)
            {
                Blast blast = new Blast(100, 4, 4);
                Shell shell3 = new Shell(coordX, coordY, playerTank.GetAim(), playerTank.GetTankPower(), 0.01f, blast, player);
                currentGame.AddAttack(shell3);
                Shell shell4 = new Shell(coordX, coordY, playerTank.GetAim(), playerTank.GetTankPower(), 0.015f, blast, player);
                currentGame.AddAttack(shell4);
                Shell shell5 = new Shell(coordX, coordY, playerTank.GetAim(), playerTank.GetTankPower(), 0.005f, blast, player);
                currentGame.AddAttack(shell5);
            }

            else
            {
                Blast blast = new Blast(200, 4, 4);
                Shell shell = new Shell(coordX, coordY, playerTank.GetAim(), playerTank.GetTankPower(), 0.01f, blast, player);
                currentGame.AddAttack(shell);
            }

        }
    }
}
