using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;

namespace TankBattle
{
    /// <summary>
    /// Class that draws and puts into action the projectiles shot by the tanks.
    ///
    /// </summary>
    public class Shell : Attack
    {
        private float xCoord;
        private float yCoord;
        private float gForce;
        private Blast exp;
        private Player selectedPlayer;
        private float xVelocity;
        private float yVelocity;

        /// <summary>
        /// Constructor of class Shell that takes into account various aspects of the weapon's blast, 
        /// damage, position, angle, power, velocitites, as well as the player shooting it and the gravity.
        ///
        /// </summary>
        /// <param name="x">X-coordinate of the shell.</param>
        /// <param name="y">Y-coordinate of the shell.</param>
        /// <param name="angle">Angle the shell is being shot at.</param>
        /// <param name="power">Power the shell is being shot at.</param>
        /// <param name="gravity">Rate of downward acceleration that affects the shell.</param>
        /// <param name="explosion">The 'blast' the shell has upon impact.</param>
        /// <param name="player">The player shooting the shell.</param>
        public Shell(float x, float y, float angle, float power, float gravity, Blast explosion, Player player)
        {
            xCoord = x;
            yCoord = y;
            gForce = gravity;
            exp = explosion;
            selectedPlayer = player;

            float angleRadians = (90 - angle) * (float)Math.PI / 180;
            float magnitude = power / 50;
            xVelocity = (float)Math.Cos(angleRadians) * magnitude;
            yVelocity = (float)Math.Sin(angleRadians) * -magnitude;
        }

        /// <summary>
        /// Function that increments the coordinates of the shell based on set velocities, wind, and gravity.
        ///
        /// </summary>
        public override void Tick()
        {
            for (int i = 0; i < 10; i++)
            {
                xCoord += xVelocity;
                yCoord += yVelocity;
                xCoord += (currentGame.Wind() / 1000.0f);
                // If shell is outside the battlefield...
                if (xCoord < 0 || xCoord > Battlefield.WIDTH || yCoord > Battlefield.HEIGHT)
                {
                    currentGame.CancelEffect(this); // Removes shell.
                }
                // If shell collides with something...
                else if (currentGame.DetectCollision(xCoord, yCoord))
                {
                    // Initiates damage then removes shell.
                    selectedPlayer.ProjectileHit(xCoord, yCoord);
                    exp.Activate(xCoord, yCoord);
                    currentGame.AddAttack(exp);
                    currentGame.CancelEffect(this);
                }

                yVelocity += gForce; // Gravity takes effect.
            }
        }

        /// <summary>
        /// Draws the shell.
        ///
        /// </summary>
        /// <param name="graphics">Shell graphics.</param>
        /// <param name="size">Size of the graphics.</param>
        public override void Display(Graphics graphics, Size size)
        {
            float x = (float)this.xCoord * size.Width / Battlefield.WIDTH;
            float y = (float)this.yCoord * size.Height / Battlefield.HEIGHT;
            float s = size.Width / Battlefield.WIDTH;

            RectangleF r = new RectangleF(x - s / 2.0f, y - s / 2.0f, s, s);
            Brush b = new SolidBrush(Color.WhiteSmoke);

            graphics.FillEllipse(b, r);
        }
    }
}
