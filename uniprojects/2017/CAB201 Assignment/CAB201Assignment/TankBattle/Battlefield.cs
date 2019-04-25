using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TankBattle
{
    /// <summary>
    /// Class that manages the terrain and how the tanks, shells, and players can interact with it.
    ///
    /// </summary>
    public class Battlefield
    {
        public const int WIDTH = 160;
        public const int HEIGHT = 120;
        private bool[,] terrain;

        /// <summary>
        /// Randomly creates the terrain of the game, by taking a 2D boolean array and marking 'true' 
        /// where the tiles will be positioned. Follows specifications that are detailed within the function.
        ///
        /// </summary>        
        public Battlefield()
        {
            Random rnd = new Random();
            terrain = new bool[HEIGHT, WIDTH];
            for (int i = 0; i < WIDTH; i++) // The entire bottom row contains tiles.
            {
                terrain[(HEIGHT - 1), i] = true;              
            }

            for (int i = HEIGHT - 2; i > Tank.HEIGHT; i--)
            {
                for (int j = 0; j < WIDTH; j++)
                {
                    // If there is a tile underneath, there is a 299/300 chance of a tile spawn.
                    if (rnd.Next(0, 301) >= 1 && (terrain[i + 1, j])) 
                    {
                        // On top of that, either there are tiles to the lower left or right, or the tile is spawning
                        // at the edges of the map, for a guranteed tile spawn.
                        if ((j == 0) || (j == WIDTH - 1) || (terrain[i + 1, j - 1]) && (terrain[i + 1, j + 1])) {
                            terrain[i, j] = true; // Create a tile at position (j, i).
                        }
                        
                    }

                    else
                    {
                        terrain[i, j] = false;
                    }
                }
            } 
        }

        /// <summary>
        /// Checks to see if there is a tile at a certain position.
        ///
        /// </summary>
        /// <param name="x">X-axis of the terrain to check.</param>
        /// <param name="y">Y-axis of the terrain to check.</param>
        /// <returns>Whether the tile is at the coordinates or not.</returns>
        public bool TileAt(int x, int y)
        {
            if (x < 0 || x > Battlefield.WIDTH || y < 0 || y > Battlefield.HEIGHT)
            {
                return false;
            }

            else
            {
                return terrain[y, x];
            }
        }

        /// <summary>
        /// Checks to see if a tank would fit at a certain position without colliding with a tile.
        ///
        /// </summary>
        /// <param name="x">X-axis of the top-left corner of the tank.</param>
        /// <param name="y">Y-axis of the top-left corner of the tank.</param>
        /// <returns>Whether the tank is able to fit.</returns>
        public bool CheckTankCollide(int x, int y)
        {
            if (x < 0 || x > (Battlefield.WIDTH - Tank.WIDTH) || y < 0 || y > (Battlefield.HEIGHT - Tank.HEIGHT))
            {
                return false;
            }

            for(int j = 0; j < Tank.HEIGHT; j++) {

                for (int i = 0; i < Tank.WIDTH; i++)
                {
                    if (TileAt(x + i, y + j))
                    {
                        return true;
                    }
                }
            }

            return false;
            
        }

        /// <summary>
        /// Finds the lowest possible y-position for a tank on a certain x-axis to be on a terrain.
        ///
        /// </summary>
        /// <param name="x">X-axis of the top-left corner of the tank.</param>
        /// <returns>The lowest possible y-position for a tank to be on without colliding.</returns>
        public int TankYPosition(int x)
        {
            if (x < 0 || x > (Battlefield.WIDTH - Tank.WIDTH)) {
                return -1;
            }

            int yLow = 0;
            while (true)
            {
                if (CheckTankCollide(x, yLow)) {
                    return (yLow - 1);
                }
                yLow++;
            }
        }

        /// <summary>
        /// Sets the terrain within a certain radius to false, thereby 'destroying' the tiles in the radius.
        ///
        /// </summary>
        /// <param name="destroyX">X-axis of the center of destruction.</param>
        /// <param name="destroyY">Y-axis of the center of destruction.</param>
        /// <param name="radius">Length of radius of destruction, center point being (destroyX, destroyY).</param>
        public void DestroyTerrain(float destroyX, float destroyY, float radius)
        {  
            for (int j = 0; j < HEIGHT; j++)
            {
                for (int i = 0; i < WIDTH; i++)
                {
                    if (Math.Sqrt(Math.Pow(i - destroyX, 2) + Math.Pow(j - destroyY, 2)) <= radius)
                    {
                        terrain[j, i] = false;
                    }
                }
            } 
            
        }

        /// <summary>
        /// Checks to see if the tile can fall depending on if there is a tile underneath it. 
        /// If yes, 'pushes' the tile down by setting the space below it to 'true' and the previous space to 'false'.
        /// </summary>
        /// <returns>Whether the tile can fall or not.</returns>
        public bool GravityStep()
        {
            bool gravityStepped = false;
            for (int j = HEIGHT - 2; j > 0; j--)
            {
                for (int i = 0; i < WIDTH; i++)
                {
                    if (terrain[j, i] && terrain[j + 1, i] == false)
                    {
                        terrain[j + 1, i] = true;
                        terrain[j, i] = false;
                        gravityStepped = true;
                    }
                }
            }

            return gravityStepped;
        }
    }
}
