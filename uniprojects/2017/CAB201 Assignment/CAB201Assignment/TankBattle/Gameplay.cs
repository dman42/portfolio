using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Windows.Forms;

namespace TankBattle
{
    /// <summary>
    /// Class that manages the gameplay of the program including starting and finishing rounds, 
    /// players in game, positions of tanks, wind speed, list of attacks, current battlefield, etc.
    ///
    /// </summary>
    public class Gameplay
    {
                 
        private Player[] playerArray; 
        private PlayerTank[] playerTankArray; 
        private List<Attack> attackArray; 
        private int totalRounds; 
        private int currentRound; 
        private int startingPlayer; 
        private int currentPlayer; 
        private Battlefield currentBattlefield;
        private int[] playerPositions;
        private int windSpeed;

        /// <summary>
        /// Constructor of class Gameplay that takes into account the number
        /// of players and rounds the game has.
        ///
        /// </summary>
        /// <param name="numPlayers">Total number of players (between 2 and 8) the game has.</param>
        /// <param name="numRounds">Total number of rounds (between 1 and 100) the game has.</param>
        public Gameplay(int numPlayers, int numRounds) {
            if (numPlayers < 2)
            {
                numPlayers = 2;
            }
            else if (numPlayers > 8)
            {
                numPlayers = 8;
            }

            if (numRounds < 1)
            {
                numRounds = 1;
            }
            else if (numRounds > 100)
            {
                numRounds = 100;
            }

            playerArray = new Player[numPlayers];
            totalRounds = numRounds;
            attackArray = new List<Attack>(); 
        }

        /// <summary>
        /// Returns the number of players currently playing the game.
        /// 
        /// </summary>
        /// <returns>The number of players currently playing the game.</returns>
        public int NumPlayers() {
            return playerArray.Length;
        }

        /// <summary>
        /// Returns the round currently being played.
        /// 
        /// </summary>
        /// <returns>The round currently being played.</returns>
        public int CurrentRound() {
            return currentRound;
        }

        /// <summary>
        /// Returns the total amount of rounds the game has.
        /// 
        /// </summary>
        /// <returns>The total amount of rounds the game has.</returns>
        public int GetTotalRounds() {
            return totalRounds;
        }

        /// <summary>
        /// Adds a player to the game via the list of players.
        ///
        /// </summary>
        /// <param name="player">Player to be added to the game.</param>
        /// <param name="playerNum">Player's order number.</param>
        public void CreatePlayer(int playerNum, Player player) {
            playerArray[playerNum - 1] = player;
        }

        /// <summary>
        /// Summons the player stored in 'playerArray' at a given index.
        ///
        /// </summary>
        /// <param name="playerNum">Player's order number.</param>
        /// <returns>Player stored at the corresponding index in 'playerArray'.</returns>
        public Player Player(int playerNum) {
            return playerArray[playerNum - 1];
        }

        /// <summary>
        /// Summons the tank a player with a certain player number has.
        ///
        /// </summary>
        /// <param name="playerNum">Order number of the player that has the necessary tank.</param>
        /// <returns>The player's tank.</returns>
        public PlayerTank GetBattleTank(int playerNum) {
            return playerTankArray[playerNum];
        }

        /// <summary>
        /// Returns a colour based on that player's order number.
        ///
        /// </summary>
        /// <param name="playerNum">Player's order number.</param>
        /// <returns>The player's tank colour.</returns>
        public static Color GetTankColour(int playerNum) {
            if (playerNum == 1)
            {
                return Color.Red;
            }
            else if (playerNum == 2)
            {
                return Color.Green;
            }
            else if (playerNum == 3)
            {
                return Color.Orange;
            }
            else if (playerNum == 4)
            {
                return Color.Yellow;
            }
            else if (playerNum == 5)
            {
                return Color.Blue;
            }
            else if (playerNum == 6)
            {
                return Color.Black;
            }
            else if (playerNum == 7)
            {
                return Color.Purple;
            }
            else
            {
                return Color.Wheat;
            }
        }

        /// <summary>
        /// Calculates the logical position of all the tanks, based on the fact that the first and
        /// last tanks are half the distance from the ends as each tank is from each other.
        ///
        /// </summary>
        /// <param name="numPlayers">Number of players in the game.</param>
        /// <returns>An array of tank positions.</returns>
        public static int[] CalculatePlayerPositions(int numPlayers) {
            int[] positionArray = new int[numPlayers];

            // Figures out the distance between an end tank and a wall.
            int distanceUnit = (int) (Battlefield.WIDTH / (2.0 * numPlayers));

            // Correctly positions each tank by incrementing wall distance times two for each tank
            for (int i = 0; i < numPlayers; i++)
            {
                float positionFloat = (float) (2.0 * distanceUnit * i) + distanceUnit;
                positionArray[i] = (int) Math.Round(positionFloat);
            }
            return positionArray;
        }

        /// <summary>
        /// Randomly shuffles an array's contents.
        ///
        /// </summary>
        /// <param name="array">Any array of integers.</param>
        public static void Randomise(int[] array) {
            Random rnd = new Random();

            int len = array.Length;
            while (len > 1)
            {
                int a = rnd.Next(len--); // Decreases length so the function will not end up changing the end value more than once.
                int b = array[len];
                array[len] = array[a]; // Makes the last instance of an array equal to a random instance.
                array[a] = b; // Makes said random instance equal to the last instance, switching the values.
            } 
        }

        /// <summary>
        /// Begins game, setting the current round to 1, the starting player's index to 0, and calling
        /// StartRound() for the first time.
        ///
        /// </summary>
        public void StartGame() {
            currentRound = 1;
            startingPlayer = 0;
            StartRound();
        }

        /// <summary>
        /// Begins one round of gameplay. Detailed comments are given via in-line comments in the function. 
        ///
        /// </summary>
        public void StartRound() {
            Random rnd = new Random();
            currentPlayer = startingPlayer; // The starting player is the current player.
            currentBattlefield = new Battlefield(); // Sets new terrain.
            playerPositions = CalculatePlayerPositions(playerArray.Length); // Stores the player positions.
            foreach (Player player in playerArray)
            { // Runs 'NewRound()' for each player.
                player.NewRound();
            }
            Randomise(playerPositions); // Randomises the player positions so each player could be at any of the positions.
            playerTankArray = new PlayerTank[playerArray.Length];
            for (int i = 0; i < playerTankArray.Length; i++)
            { // Creates tanks for each player.
                 playerTankArray[i] = new PlayerTank(playerArray[i], playerPositions[i], currentBattlefield.TankYPosition(playerPositions[i]), this);
            }
            
            windSpeed = rnd.Next(-100, 101); // Sets random wind speed.
            SkirmishForm skirmish = new SkirmishForm(this);
            skirmish.Show(); // Opens the skirmish window.
        }

        /// <summary>
        /// Returns the battlefield being used in the current round.
        ///
        /// </summary>
        /// <returns>The battlefield being used in the current round.</returns>
        public Battlefield GetMap() {
            return currentBattlefield;
        }

        /// <summary>
        /// Draws every tank, assuming each tank exists in-game.
        ///
        /// </summary>
        /// <param name="graphics">Tank graphics.</param>
        /// <param name="displaySize">Size of the graphics.</param>
        public void DrawTanks(Graphics graphics, Size displaySize) {

            foreach (PlayerTank playertank in playerTankArray)
            {
                if (playertank.TankExists())
                {
                    playertank.Display(graphics, displaySize);
                }
            }
        }

        /// <summary>
        /// Returns the tank the current player is using.
        ///
        /// </summary>
        /// <returns>The tank the current player is using.</returns>
        public PlayerTank GetCurrentPlayerTank() {
            return playerTankArray[currentPlayer];
        }

        /// <summary>
        /// Adds an attack to the list of attacks the game keeps track of.
        ///
        /// </summary>
        /// <param name="weaponEffect">Weapon to be added.</param>
        public void AddAttack(Attack weaponEffect) {
            attackArray.Add(weaponEffect);
            weaponEffect.RecordCurrentGame(this); // Records 'game' the weapon is in for later use.
        }

        /// <summary>
        /// Runs the "tick" function of each weapon. See documentation for tick functions.
        ///
        /// </summary>
        /// <returns>Whether there was any weapon to tick.</returns>
        public bool ProcessEffects() { 
            bool ticked = false;
            for (int i = attackArray.Count - 1; i > -1; i--)
            {
                if (attackArray[i] != null)
                {
                    attackArray[i].Tick();
                    ticked = true;
                }
            }

            return ticked;
        }

        /// <summary>
        /// Draws every weapon and its effect.
        ///
        /// </summary>
        /// <param name="graphics">Weapon graphics.</param>
        /// <param name="displaySize">Size of the graphics.</param>
        public void DrawWeaponEffects(Graphics graphics, Size displaySize) {
            foreach (Attack attack in attackArray)
            {
                attack.Display(graphics, displaySize);
            }
        }

        /// <summary>
        /// Removes an attack to the list of attacks the game keeps track of.
        ///
        /// </summary>
        /// <param name="weaponEffect">Weapon to be removed.</param>
        public void CancelEffect(Attack weaponEffect) {
            attackArray.Remove(weaponEffect);
        }

        /// <summary>
        /// Checks if a projectile has hit anything.
        ///
        /// </summary>
        /// <param name="projectileX">X-position of the projectile.</param>
        /// <param name="projectileY">Y-position of the projectile.</param>
        /// <returns> Whether the projectle collided with anything or not.</returns>
        public bool DetectCollision(float projectileX, float projectileY) {
            // If the projectile leaves the battlefield altogether...
            if (projectileX < 0 || projectileX > Battlefield.WIDTH || projectileY > Battlefield.HEIGHT)
            {
                return false;
            }

            // If there is a tank where the projectile is...
            else if (currentBattlefield.TileAt((int)projectileX, (int)projectileY))
            {
                return true;
            }

            // If there is a tank where the projectile is...
            for (int i = 0; i < playerTankArray.Length; i++) {
                if (projectileX <= playerTankArray[i].GetXPos() + Tank.WIDTH
                && projectileX >= playerTankArray[i].GetXPos()
                && projectileY <= playerTankArray[i].YPos() + Tank.HEIGHT
                && projectileY >= playerTankArray[i].YPos()
                && i != currentPlayer)
                {
                    return true;
                }
            }

            // If the projectile hits nothing...
            return false;
        }

        /// <summary>
        /// Inflicts damage onto a tank based on the position, explosion damage,
        /// and radius of the explosion caused by a weapon.
        ///
        /// </summary>
        /// <param name="damageX">X-position of the projectile.</param>
        /// <param name="damageY">Y-position of the projectile.</param>
        /// <param name="explosionDamage">Damage caused by the explosion.</param>
        /// <param name="radius">Explosion radius.</param>
        public void Damage(float damageX, float damageY, float explosionDamage, float radius) {

            for (int i = 0; i < playerTankArray.Length; i++)
            {
                // For each tank that exists...
                if (playerTankArray[i].TankExists())
                {

                    float centerX = (float)(playerTankArray[i].GetXPos() + (Tank.WIDTH / 2)); // Center X-axis of the tank.
                    float centerY = (float)(playerTankArray[i].YPos() + (Tank.HEIGHT / 2)); // Center Y-axis of the tank.
                    // Difference between damage location and center of the tank.
                    float distance = (float)(Math.Sqrt(Math.Pow(centerX - damageX, 2) + Math.Pow(centerY - damageY, 2))); 

                    if (distance < radius && distance > radius / 2)
                    { // If the tank is within the outer region of the radius.
                        playerTankArray[i].Damage((int)((explosionDamage * (radius - distance)) / radius));
                    }
                    else if (distance < radius / 2)
                    { // If the tank is within the inner region of the radius.
                        playerTankArray[i].Damage((int)(explosionDamage));
                    }
                }
            }
        }

        /// <summary>
        /// Calls the 'GravityStep()' function for each tank and the battlefield.
        ///
        /// </summary>
        /// <returns>Whether anything fell.</returns>
        public bool GravityStep() {
            bool stepped = false;

            if (currentBattlefield.GravityStep())
            {
                stepped = true;
            }

            for (int i = 0; i < playerTankArray.Length; i++)
            {
                if (playerTankArray[i].GravityStep())
                {
                    stepped = true;
                }
            }

            return stepped;
        }

        /// <summary>
        /// Ends the turn.
        ///
        /// </summary>
        /// <returns>If the round is over (false if yes).</returns>
        public bool EndTurn() {
            Random rnd = new Random();
            int counter = 0;
            // Counts every tank that exists.
            for (int i = 0; i < playerTankArray.Length; i++)
            {
                if (playerTankArray[i].TankExists())
                { 
                    counter++;
                }
            }

            // If there are 2 or more tanks, round continues and the next player with an existing tank is found.
            if (counter >= 2 ) {
                do
                {
                    currentPlayer++;
                    if (currentPlayer == playerTankArray.Length)
                    {
                        currentPlayer = 0;
                    }
                } while (!playerTankArray[currentPlayer].TankExists());
                
                windSpeed += rnd.Next(-10, 11); // Changes the wind.
                return true;
            } else { // If there are less than 2 tanks, round ends and score is updated.
                ScoreWinner();
                return false;
            }
        }

        /// <summary>
        /// Gives the player with a survivng tank one point.
        ///
        /// </summary>
        public void ScoreWinner()
        {
            for (int i = 0; i < playerTankArray.Length; i++)
            {
                if (playerTankArray[i].TankExists())
                {
                    Player player = playerTankArray[i].Player();
                    player.AddPoint();
                }
            }
        }

        /// <summary>
        /// Sets up next round. If the round limit has been reached, shows the title screen.
        /// 
        /// </summary>
        public void NextRound() 
        {
            currentRound++;
            if (currentRound <= totalRounds)
            {
                startingPlayer++;
                if (startingPlayer <= NumPlayers())
                {
                    startingPlayer = 0;
                }
                StartRound();
            }
            else
            {
                TitleForm titleForm = new TitleForm();
                titleForm.Show();
            }
        }

        /// <summary>
        /// Returns the wind speed.
        ///
        /// </summary>
        /// <returns>The wind speed.</returns>
        public int Wind()
        {
            return windSpeed;
        }
    }
}
