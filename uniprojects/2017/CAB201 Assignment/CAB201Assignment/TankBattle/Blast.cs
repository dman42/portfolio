using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TankBattle
{
    /// <summary>
    /// Class that determines the blast caused by a shell, its damage, the graphics, etc.
    ///
    /// </summary>
    public class Blast : Attack
    {
        private int damage;
        private int expRadius;
        private int destructRadius;
        private float xCoord;
        private float yCoord;
        private float lifespan;

        /// <summary>
        /// Contstructor function that sets the damage, explosion radius, and earth destruction radius caused
        /// by the blast.
        ///
        /// </summary>
        /// <param name="explosionDamage">Damage from the explosion.</param>
        /// <param name="explosionRadius">Radius where any tank is affected by the blast.</param>
        /// <param name="earthDestructionRadius">Radius where any earth is affected by the blast.</param>
        public Blast(int explosionDamage, int explosionRadius, int earthDestructionRadius)
        {
            damage = explosionDamage;
            expRadius = explosionRadius;
            destructRadius = earthDestructionRadius;
        }

        /// <summary>
        /// Activates the blast, setting its lifespan and coordinates.
        ///
        /// </summary>
        /// <param name="x">X-axis of the blast.</param>
        /// <param name="y">Y-axis of the blast.</param>
        public void Activate(float x, float y)
        {
            xCoord = x;
            yCoord = y;
            lifespan = 1.0f;
        }

        /// <summary>
        /// Decreases the lifespan, initiating damage and earth destruction
        /// after the lifespan has reached 0.
        ///
        /// </summary>
        public override void Tick()
        {
            lifespan -= 0.05f;
            if (lifespan <= 0)
            {
                currentGame.Damage(xCoord, yCoord, damage, expRadius);
                Battlefield bttl = currentGame.GetMap();
                bttl.DestroyTerrain(xCoord, yCoord, destructRadius);
                currentGame.CancelEffect(this);
            }
        }

        /// <summary>
        /// Draws the blast.
        ///
        /// </summary>
        /// <param name="graphics">Shell graphics.</param>
        /// <param name="displaySize">Size of the graphics.</param>
        public override void Display(Graphics graphics, Size displaySize)
        {
            float x = (float)this.xCoord * displaySize.Width / Battlefield.WIDTH;
            float y = (float)this.yCoord * displaySize.Height / Battlefield.HEIGHT;
            float radius = displaySize.Width * (float)((1.0 - lifespan) * expRadius * 3.0 / 2.0) / Battlefield.WIDTH;

            int alpha = 0, red = 0, green = 0, blue = 0;

            if (lifespan < 0)
            {
                lifespan = 0;
            }

            if (lifespan < 1.0 / 3.0)
            {
                red = 255;
                alpha = (int)(lifespan * 3.0 * 255);
            }
            else if (lifespan < 2.0 / 3.0)
            {
                red = 255;
                alpha = 255;
                green = (int)((lifespan * 3.0 - 1.0) * 255);
            }
            else
            {
                red = 255;
                alpha = 255;
                green = 255;
                blue = (int)((lifespan * 3.0 - 2.0) * 255);
            }

            RectangleF rect = new RectangleF(x - radius, y - radius, radius * 2, radius * 2);
            Brush b = new SolidBrush(Color.FromArgb(alpha, red, green, blue));

            graphics.FillEllipse(b, rect);
        }
    }
}
