using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TankBattle
{
    /// <summary>
    /// The main window, where the game actually happens, including the interface as well as the display
    /// of the game and the animations.
    ///
    /// </summary>
    public partial class SkirmishForm : Form
    {
        private Color landscapeColour;
        private Random rnd = new Random();
        private Image backgroundImage = null;
        private int levelWidth = 160;
        private int levelHeight = 120;
        private Gameplay currentGame;

        private BufferedGraphics backgroundGraphics;
        private BufferedGraphics gameplayGraphics;

        /// <summary>
        /// Constructor for SkirmishForm that sets the game currently being played, background images,
        /// graphics, and starts a new turn.
        ///
        /// </summary>
        /// <param name="game">Game currently being played.</param>
        public SkirmishForm(Gameplay game)
        {
            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            SetStyle(ControlStyles.DoubleBuffer, true);
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.UserPaint, true);
            SetStyle(ControlStyles.UserPaint, true);

            currentGame = game;

            string[] imageFilenames = { "Images\\background1.jpg",
                            "Images\\background2.jpg",
                            "Images\\background3.jpg",
                            "Images\\background4.jpg"};

            Color[] landscapeColours = { Color.FromArgb(255, 0, 0, 0),
                             Color.FromArgb(255, 73, 58, 47),
                             Color.FromArgb(255, 148, 116, 93),
                             Color.FromArgb(255, 133, 119, 109) };

            backgroundImage = Image.FromFile(imageFilenames[rnd.Next(0, 4)]);
            landscapeColour = landscapeColours[rnd.Next(0, 4)];

            InitializeComponent();

            backgroundGraphics = InitDisplayBuffer();
            gameplayGraphics = InitDisplayBuffer();
            DrawBackground();
            DrawGameplay();
            NewTurn();
        }

        // From https://stackoverflow.com/questions/13999781/tearing-in-my-animation-on-winforms-c-sharp
        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                cp.ExStyle |= 0x02000000; // WS_EX_COMPOSITED
                return cp;
            }
        }

        /// <summary>
        /// Enables the tank controls, allwoing them to be used.
        ///
        /// </summary>
        public void EnableTankControls()
        {
            controlPanel.Enabled = true;
        }

        /// <summary>
        /// Sets the numeric up-down that controls the angle of the tank's shot to the value specified.
        ///
        /// </summary>
        /// <param name="angle">Angle value that sets the numeric up-down value.</param>
        public void Aim(float angle)
        {
            numericUpDown1.Value = (decimal) angle;
        }

        /// <summary>
        /// Sets the track bar that controls the power of the tank's shot to the value specified.
        ///
        /// </summary>
        /// <param name="power">Power value that sets the track bar value.</param>
        public void SetForce(int power)
        {
            trackBar1.Value = power;
        }

        /// <summary>
        /// Sets the combobox that controls the tank's weapon in use to the value specified.
        ///
        /// </summary>
        /// <param name="weapon">Index of the weapon that sets the combobox value.</param>
        public void SelectWeapon(int weapon)
        {
            comboBox1.SelectedIndex = weapon;
        }

        /// <summary>
        /// Prepares and fires the tank, as well as turning on the timer and disabling the control panel.
        ///
        /// </summary>
        public void Shoot()
        {
            PlayerTank playerTank = currentGame.GetCurrentPlayerTank();

            playerTank.Aim((float) numericUpDown1.Value);
            playerTank.SetForce(trackBar1.Value);
            playerTank.SelectWeapon(comboBox1.SelectedIndex);

            playerTank.Shoot();
            controlPanel.Enabled = false;
            timer1.Enabled = true;
        }

        /// <summary>
        /// Draws the background.
        ///
        /// </summary>
        private void DrawBackground()
        {
            Graphics graphics = backgroundGraphics.Graphics;
            Image background = backgroundImage;
            graphics.DrawImage(backgroundImage, new Rectangle(0, 0, displayPanel.Width, displayPanel.Height));

            Battlefield battlefield = currentGame.GetMap();
            Brush brush = new SolidBrush(landscapeColour);

            for (int y = 0; y < Battlefield.HEIGHT; y++)
            {
                for (int x = 0; x < Battlefield.WIDTH; x++)
                {
                    if (battlefield.TileAt(x, y))
                    {
                        int drawX1 = displayPanel.Width * x / levelWidth;
                        int drawY1 = displayPanel.Height * y / levelHeight;
                        int drawX2 = displayPanel.Width * (x + 1) / levelWidth;
                        int drawY2 = displayPanel.Height * (y + 1) / levelHeight;
                        graphics.FillRectangle(brush, drawX1, drawY1, drawX2 - drawX1, drawY2 - drawY1);
                    }
                }
            }
        }

        /// <summary>
        /// Manages the buffered graphics.
        ///
        /// </summary>
        /// <returns>Buffered graphics.</returns>
        public BufferedGraphics InitDisplayBuffer()
        {
            BufferedGraphicsContext context = BufferedGraphicsManager.Current;
            Graphics graphics = displayPanel.CreateGraphics();
            Rectangle dimensions = new Rectangle(0, 0, displayPanel.Width, displayPanel.Height);
            BufferedGraphics bufferedGraphics = context.Allocate(graphics, dimensions);
            return bufferedGraphics;
        }

        /// <summary>
        /// Renders graphics.
        ///
        /// </summary>
        private void displayPanel_Paint(object sender, PaintEventArgs e)
        {
            Graphics graphics = displayPanel.CreateGraphics();
            gameplayGraphics.Render(graphics);
        }

        /// <summary>
        /// Draws everything, including tanks and weapon effects.
        ///
        /// </summary>
        private void DrawGameplay()
        {
            backgroundGraphics.Render(gameplayGraphics.Graphics);
            currentGame.DrawTanks(gameplayGraphics.Graphics, displayPanel.Size);
            currentGame.DrawWeaponEffects(gameplayGraphics.Graphics, displayPanel.Size);
        }

        /// <summary>
        /// Starts a new turn, updating the text, the values that control the aim, power, and weapon,
        /// and starting a new turn for the player, resetting everything back to the default values.
        ///
        /// </summary>
        private void NewTurn()
        {
            PlayerTank playerTank = currentGame.GetCurrentPlayerTank();
            Player player = playerTank.Player();
            this.Text = "Tank Battle - Round " + currentGame.CurrentRound() + " of " + currentGame.GetTotalRounds();
            controlPanel.BackColor = player.PlayerColour();
            label1.Text = player.Name();
            Aim(playerTank.GetAim());
            SetForce(playerTank.GetTankPower());
            playerTank.SelectWeapon(playerTank.GetWeaponIndex());
            if (currentGame.Wind() < 0)
            {
                label3.Text = Math.Abs(currentGame.Wind()) + " W";
            }

            else
            {
                label3.Text = currentGame.Wind() + " E";
            }
            comboBox1.Items.Clear();
            Tank tank = playerTank.GetTank();
            string[] weaponList = tank.ListWeapons();
            foreach (string weapon in weaponList)
            {
                comboBox1.Items.Add(weapon);
            }
            player.NewTurn(this, currentGame);
            // Resets everything back to its default value.
            comboBox1.SelectedIndex = 0; 
            numericUpDown1.Value = 0;
            SetForce(5);
            label6.Text = "" + trackBar1.Value;
        }

        /// <summary>
        /// Whenever the combobox changes selected index, updates the selected weapon.
        ///
        /// </summary>
        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            SelectWeapon(comboBox1.SelectedIndex);
        }

        /// <summary>
        /// Whenever the bar scrolls, updates the force.
        ///
        /// </summary>
        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            SetForce(trackBar1.Value);
            label6.Text = "" + trackBar1.Value;
        }

        /// <summary>
        /// Whenever the numeric up-down value changes, updates the aim and the image of the tank turret.
        ///
        /// </summary>
        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            Aim((float) numericUpDown1.Value);
            DrawGameplay();
            displayPanel.Invalidate();
        }
        
        /// <summary>
        /// Tank shoots when the button "Fire!" is pressed.
        ///
        /// </summary>
        private void button1_Click(object sender, EventArgs e)
        {
            Shoot();
        }

        /// <summary>
        /// Whenever the timer ticks, updates the weapon effects. If the effects have finished, 
        /// game updates the gravity. If the gravity effects have finished, timer turns off and the turn ends.
        /// If it's the end of the turn, the game starts a new turn, otherwise the game starts a new round.
        ///
        /// </summary>
        private void timer1_Tick(object sender, EventArgs e)
        {
            bool ongoingEffects = currentGame.ProcessEffects();
            if (!ongoingEffects)
            {
                bool fell = currentGame.GravityStep();
                DrawBackground();
                DrawGameplay();
                displayPanel.Invalidate();
                if (!fell)
                {
                    timer1.Enabled = false;
                    bool end = currentGame.EndTurn();
                    if (end)
                    {
                        NewTurn();
                    }

                    else
                    {
                        Dispose();
                        currentGame.NextRound();
                    }
                }
            }

            else
            {
                DrawGameplay();
                displayPanel.Invalidate();
            }
        }
        
        // Useless leftover functions.
        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void label7_Click(object sender, EventArgs e)
        {

        }

        private void label6_Click(object sender, EventArgs e)
        {

        }

        private void SkirmishForm_Load(object sender, EventArgs e)
        {

        }
    }
}
