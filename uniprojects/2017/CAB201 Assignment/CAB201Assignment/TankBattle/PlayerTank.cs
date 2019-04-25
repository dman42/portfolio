using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TankBattle
{
    /// <summary>
    /// Class that involves with tanks specifically attributed to a player.
    ///
    /// </summary>
    public class PlayerTank
    {
        private Player currentplayer;
        private Tank playerTank;
        private int tankXCoord;
        private int tankYCoord;
        private Bitmap tankBitmap;
        private Gameplay mainGame;
        private int tankPower;
        private int currentWeapon;
        private float tankAngle;
        private int durability;

        /// <summary>
        /// Constructor class that assigns a player, the tank coordinates, and the current game to the player tank,
        /// as well as the special kind of tank style and weapon range, and initial angle, current weapon, and 
        /// power the tank has.
        /// 
        /// </summary>
        /// <param name="player">Player associated with the tank.</param>
        /// <param name="tankX">Tank X-coordinates.</param>
        /// <param name="tankY">Tank Y-coordinates.</param>
        /// <param name="game">Game currently being played with the tank.</param>
        public PlayerTank(Player player, int tankX, int tankY, Gameplay game)
        {
            currentplayer = player;
            tankXCoord = tankX;
            tankYCoord = tankY;
            mainGame = game;

            playerTank = currentplayer.GetTank();
            durability = playerTank.GetTankHealth();
            tankAngle = 0;
            tankPower = 25;
            currentWeapon = 0;
            tankBitmap = playerTank.CreateBitmap(currentplayer.PlayerColour(), tankAngle);
        }

        /// <summary>
        /// Summons the player stored in the player tank. 
        ///
        /// </summary>
        /// <returns>The player stored in this player tank.</returns>
        public Player Player()
        {
            return currentplayer;
        }

        /// <summary>
        /// Summons the kind of tank stored in this player tank. 
        ///
        /// </summary>
        /// <returns>The kind of tank stored in this player tank.</returns>
        public Tank GetTank() 
        {
            return playerTank;
        }

        /// <summary>
        /// Summons the angle of the player tank's turret. 
        ///
        /// </summary>
        /// <returns>The angle of the player tank's turret. </returns>
        public float GetAim()
        {
            return tankAngle;
        }

        /// <summary>
        /// Sets the angle of the player tank's turret, as well as updating the bitmap to reflect the turret's
        /// angle. 
        ///
        /// </summary>
        /// <param name="angle">Angle for the turret to be at.</param>
        public void Aim(float angle)
        {
            tankAngle = angle;
            tankBitmap = playerTank.CreateBitmap(currentplayer.PlayerColour(), angle);
        }

        /// <summary>
        /// Summons the power of the player tank. 
        ///
        /// </summary>
        /// <returns>The power of the player tank.</returns>
        public int GetTankPower()
        {
            return tankPower;
        }

        /// <summary>
        /// Sets the power of the player tank. 
        ///
        /// </summary>
        /// <param name="power">Power for the tank to be at.</param>
        public void SetForce(int power)
        {
            tankPower = power;
        }

        /// <summary>
        /// Summons the index of the player tank's weapon. 
        ///
        /// </summary>
        /// <returns>The index of the player tank's weapon.</returns>
        public int GetWeaponIndex()
        {
            return currentWeapon;
        }

        /// <summary>
        /// Sets the weapon of the player tank. 
        ///
        /// </summary>
        /// <param name="newWeapon">Index the player tank's weapon will be at.</param>
        public void SelectWeapon(int newWeapon)
        {
            currentWeapon = newWeapon;
        }

        /// <summary>
        /// Draws the tank.
        ///
        /// </summary>
        /// <param name="graphics">Shell graphics.</param>
        /// <param name="displaySize">Size of the graphics.</param>
        public void Display(Graphics graphics, Size displaySize)
        {
            int drawX1 = displaySize.Width * tankXCoord / Battlefield.WIDTH;
            int drawY1 = displaySize.Height * tankYCoord / Battlefield.HEIGHT;
            int drawX2 = displaySize.Width * (tankXCoord + Tank.WIDTH) / Battlefield.WIDTH;
            int drawY2 = displaySize.Height * (tankYCoord + Tank.HEIGHT) / Battlefield.HEIGHT;
            graphics.DrawImage(tankBitmap, new Rectangle(drawX1, drawY1, drawX2 - drawX1, drawY2 - drawY1));

            int drawY3 = displaySize.Height * (tankYCoord - Tank.HEIGHT) / Battlefield.HEIGHT;
            Font font = new Font("Arial", 8);
            Brush brush = new SolidBrush(Color.White);

            int pct = durability * 100 / playerTank.GetTankHealth();
            if (pct < 100)
            {
                graphics.DrawString(pct + "%", font, brush, new Point(drawX1, drawY3));
            }
        }

        /// <summary>
        /// Summons the x-axis of the player tank. 
        ///
        /// </summary>
        /// <returns>The x-axis of the player tank.</returns>
        public int GetXPos()
        {
            return tankXCoord;
        }

        /// <summary>
        /// Summons the y-axis of the player tank. 
        ///
        /// </summary>
        /// <returns>The y-axis of the player tank.</returns>
        public int YPos()
        {
            return tankYCoord;
        }

        /// <summary>
        /// Gets the tank to shoot.
        ///
        /// </summary>
        public void Shoot()
        {
            Tank currentPlayerTank = GetTank();
            currentPlayerTank.ShootWeapon(GetWeaponIndex(), this, mainGame);
        }

        /// <summary>
        /// Inflicts damage to the tank by subtracting an amount from the tank's health.
        ///
        /// </summary>
        /// <param name="damageAmount">Amount of damage to be afflicted.</param>
        public void Damage(int damageAmount)
        {
            durability -= damageAmount;
        }

        /// <summary>
        /// Checks to see if tank exists depending on whether it's health is below 0 or not. 
        ///
        /// </summary>
        /// <returns>Whether the tank exists or not.</returns>
        public bool TankExists()
        {
            if (durability > 0)
            {
                return true;
            }

            else
            {
                return false;
            }
        }

        /// <summary>
        /// Checks to see if the tank can fall depending on its existense, and whether it would collide with the
        /// tiles if it fell any further. If yes, tank falls by one unit.
        /// Also subtracts one health point for each step fallen.
        /// </summary>
        /// <returns>Whether the tank can fall or not.</returns>
        public bool GravityStep()
        {
            if (!TankExists())
            {
                return false;
            }

            Battlefield bttl = mainGame.GetMap();
            if (bttl.CheckTankCollide(tankXCoord, tankYCoord + 1))
            {
                return false;
            }

            else
            {
                durability--;
                tankYCoord++;
                if (tankYCoord == Battlefield.HEIGHT - Tank.HEIGHT)
                {
                    durability = 0;
                }
                return true;
            }
        }
    }
}
