using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TankBattle
{
    /// <summary>
    /// General tank functions that apply to all tanks (which should inherit from this class).
    ///
    /// </summary>
    public abstract class Tank
    {
        public const int WIDTH = 4;
        public const int HEIGHT = 3;
        public const int NUM_TANKS = 1;

        public abstract int[,] DrawTank(float angle);

        /// <summary>
        /// Draws a line of 1's in a 2D integer array.
        ///
        /// </summary>
        /// <param name="graphic">2D integer array.</param>
        /// <param name="X1"> X-Coordinate of starting point of the line.</param>
        /// <param name="Y1"> Y-Coordinate of starting point of the line.</param>
        /// <param name="X2"> X-Coordinate of finishing point of the line.</param>
        /// <param name="Y2"> Y-Coordinate of finishing point of the line.</param>
        public static void LineDraw(int[,] graphic, int X1, int Y1, int X2, int Y2)
        {
            if (X1 == X2) // If line is vertical...
            {
                if (Y2 > Y1)
                {
                    for (int i = Y1; i <= Y2; i++)
                    {
                        graphic[i, X1] = 1;
                    }
                }
                else
                {
                    for (int i = Y1; i >= Y2; i--)
                    {
                        graphic[i, X1] = 1;
                    }
                }
            }

            else if (Y1 == Y2) // If line is horizontal...
            {
                if (X2 > X1)
                {
                    for (int i = X1; i <= X2; i++)
                    {
                        graphic[Y1, i] = 1;
                    }
                }
                else
                {
                    for (int i = X1; i >= X2; i--)
                    {
                        graphic[Y1, i] = 1;
                    }
                }
            }

            else // If line is normal...
            {
                //	Switches variables around so the line will always draw from left to right.
                if (X1 > X2)
                {
                    int switcher = X1;
                    X1 = X2;
                    X2 = switcher;
                    switcher = Y1;
                    Y1 = Y2;
                    Y2 = switcher;
                }

                // Draws line using Bresenham's logic...
                float dx = X2 - X1;
                float dy = Y2 - Y1;
                float error = 0.0f;
                float derror = Math.Abs(dy / dx);
                int y = Y1;

                if (dx > 0)
                {
                    for (int x = X1; x <= X2; x++)
                    {
                        graphic[y, x] = 1;
                        error += derror;
                        if (dy > 0)
                        {
                            while (error >= 0.5 && y <= Y2)
                            {
                                graphic[y, x] = 1;
                                y += 1;
                                error -= 1.0f;
                            }
                        }
                        else
                        {
                            while (error >= 0.5 && y >= Y2)
                            {
                                graphic[y, x] = 1;
                                if (dy < 0)
                                {
                                    y -= 1;
                                }
                                error -= 1.0f;
                            }
                        }
                    }
                } 
                else
                { 
                    for (int x = X1; x >= X2; x--)
                    {
                        graphic[y, x] = 1;
                        error += derror;
                        if (dy > 0)
                        {
                            while (error >= 0.5 && y <= Y2)
                            {
                                graphic[y, x] = 1;
                                y += 1;
                                error -= 1.0f;
                            }
                        }
                        else
                        {
                            while (error >= 0.5 && y >= Y2)
                            {
                                graphic[y, x] = 1;
                                if (dy < 0)
                                {
                                    y -= 1;
                                }
                                error -= 1.0f;
                            }
                        }
                    }
                } 
            }

        }

        /// <summary>
        /// Draws the tank's bitmap.
        ///
        /// </summary>
        /// <param name="tankColour">Colour of the tank.</param>
        /// <param name="angle"> The angle the tank's turret is at.</param>
        /// <returns>Tank Bitmap.</returns>
        public Bitmap CreateBitmap(Color tankColour, float angle)
        {
            int[,] tankGraphic = DrawTank(angle);
            int height = tankGraphic.GetLength(0);
            int width = tankGraphic.GetLength(1);

            Bitmap bmp = new Bitmap(width, height);
            Color transparent = Color.FromArgb(0, 0, 0, 0);
            Color tankOutline = Color.FromArgb(255, 0, 0, 0);

            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    if (tankGraphic[y, x] == 0)
                    {
                        bmp.SetPixel(x, y, transparent);
                    }
                    else
                    {
                        bmp.SetPixel(x, y, tankColour);
                    }
                }
            }

            // Outline each pixel
            for (int y = 1; y < height - 1; y++)
            {
                for (int x = 1; x < width - 1; x++)
                {
                    if (tankGraphic[y, x] != 0)
                    {
                        if (tankGraphic[y - 1, x] == 0)
                            bmp.SetPixel(x, y - 1, tankOutline);
                        if (tankGraphic[y + 1, x] == 0)
                            bmp.SetPixel(x, y + 1, tankOutline);
                        if (tankGraphic[y, x - 1] == 0)
                            bmp.SetPixel(x - 1, y, tankOutline);
                        if (tankGraphic[y, x + 1] == 0)
                            bmp.SetPixel(x + 1, y, tankOutline);
                    }
                }
            }

            return bmp;
        }

        public abstract int GetTankHealth();

        public abstract string[] ListWeapons();

        public abstract void ShootWeapon(int weapon, PlayerTank playerTank, Gameplay currentGame);

        /// <summary>
        /// Returns the type of tank with unique health, damage capabilities, weapons, and graphics,
        /// based on tank number.
        ///
        /// </summary>
        /// <param name="tankNumber">The number of the type of tank</param>
        /// <returns>The tank specified by the number.</returns>
        public static Tank GetTank(int tankNumber)
        {
            switch (tankNumber)
            {
                case (2): return new MyTank2();
                case (3): return new MyTank3();
                case (4): return new MyTank4();
                case (5): return new MyTank5();
                case (6): return new MyTank6();
                default : return new MyTank();
            }
        }
    }
}
