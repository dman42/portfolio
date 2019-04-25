using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TankBattle
{
    /// <summary>
    /// Window that enables user to choose players, name them, give them a specific tank,
    /// and choose whether they are computer or human.
    ///
    /// </summary>
    public partial class Setup : Form
    {
        private int playerCounter;

        /// <summary>
        /// Constructor of the setup window that also initialises the default amount of players, 
        /// and the tank types that can be chosen.
        ///
        /// </summary>
        public Setup()
        {
            playerCounter = 2;
            string[] tankTypes = { "Normal", "Half health, double damage", "Half damage, double health",
            "Bit less health, bit more damage", "Bit more health, bit less damage", "Bit less health, double triple trouble"};
            
            InitializeComponent();

            for (int i = 0; i < 6; i++)
            {
                comboBox1.Items.Add(tankTypes[i]);
            }

            for (int i = 0; i < 6; i++)
            {
                comboBox2.Items.Add(tankTypes[i]);
            }

            for (int i = 0; i < 6; i++)
            {
                comboBox3.Items.Add(tankTypes[i]);
            }

            for (int i = 0; i < 6; i++)
            {
                comboBox4.Items.Add(tankTypes[i]);
            }

            for (int i = 0; i < 6; i++)
            {
                comboBox5.Items.Add(tankTypes[i]);
            }

            for (int i = 0; i < 6; i++)
            {
                comboBox6.Items.Add(tankTypes[i]);
            }

            for (int i = 0; i < 6; i++)
            {
                comboBox7.Items.Add(tankTypes[i]);
            }

            for (int i = 0; i < 6; i++)
            {
                comboBox8.Items.Add(tankTypes[i]);
            }
        }

        /// <summary>
        /// When the button is pressed, sets up gameplay, with the amount of players selected, and for 
        /// each player ticked to be playing, records the name, type of tank, tank colour, and whether the 
        /// player is an AI or not, and creates the player, putting the player into the game. 
        /// The game then starts.
        ///
        /// </summary>
        private void button1_Click(object sender, EventArgs e)
        {
            Gameplay game = new Gameplay(playerCounter, (int) numericUpDown1.Value);

            Player player1;
            if (checkBox1.Checked)
            {
                player1 = new ComputerPlayer(textBox1.Text, Tank.GetTank(comboBox1.SelectedIndex + 1), Gameplay.GetTankColour(1));
            }
            else
            {
                player1 = new PlayerController(textBox1.Text, Tank.GetTank(comboBox1.SelectedIndex + 1), Gameplay.GetTankColour(1));
            }
            game.CreatePlayer(1, player1);

            Player player2;
            if (checkBox3.Checked)
            {
                player2 = new ComputerPlayer(textBox2.Text, Tank.GetTank(comboBox2.SelectedIndex + 1), Gameplay.GetTankColour(2));
            }
            else
            {
                player2 = new PlayerController(textBox2.Text, Tank.GetTank(comboBox2.SelectedIndex + 1), Gameplay.GetTankColour(2));
            }
            game.CreatePlayer(2, player2);
            if (playerCounter >= 3)
            {
                Player player3;
                if (checkBox5.Checked)
                {
                    player3 = new ComputerPlayer(textBox3.Text, Tank.GetTank(comboBox3.SelectedIndex + 1), Gameplay.GetTankColour(3));
                }
                else
                {
                    player3 = new PlayerController(textBox3.Text, Tank.GetTank(comboBox3.SelectedIndex + 1), Gameplay.GetTankColour(3));
                }
                game.CreatePlayer(3, player3);
                if (playerCounter >= 4)
                {
                    Player player4;
                    if (checkBox7.Checked)
                    {
                        player4 = new ComputerPlayer(textBox4.Text, Tank.GetTank(comboBox4.SelectedIndex + 1), Gameplay.GetTankColour(4));
                    }
                    else
                    {
                        player4 = new PlayerController(textBox4.Text, Tank.GetTank(comboBox4.SelectedIndex + 1), Gameplay.GetTankColour(4));
                    }
                    game.CreatePlayer(4, player4);
                    if (playerCounter >= 5)
                    {
                        Player player5;
                        if (checkBox9.Checked)
                        {
                            player5 = new ComputerPlayer(textBox5.Text, Tank.GetTank(comboBox5.SelectedIndex + 1), Gameplay.GetTankColour(5));
                        }
                        else
                        {
                            player5 = new PlayerController(textBox5.Text, Tank.GetTank(comboBox5.SelectedIndex + 1), Gameplay.GetTankColour(5));
                        }
                        game.CreatePlayer(5, player5);
                        if (playerCounter >= 6)
                        {
                            Player player6;
                            if (checkBox11.Checked)
                            {
                                player6 = new ComputerPlayer(textBox6.Text, Tank.GetTank(comboBox6.SelectedIndex + 1), Gameplay.GetTankColour(6));
                            }
                            else
                            {
                                player6 = new PlayerController(textBox6.Text, Tank.GetTank(comboBox6.SelectedIndex + 1), Gameplay.GetTankColour(6));
                            }
                            game.CreatePlayer(6, player6);
                            if (playerCounter >= 7)
                            {
                                Player player7;
                                if (checkBox13.Checked)
                                {
                                    player7 = new ComputerPlayer(textBox7.Text, Tank.GetTank(comboBox7.SelectedIndex + 1), Gameplay.GetTankColour(7));
                                }
                                else
                                {
                                    player7 = new PlayerController(textBox7.Text, Tank.GetTank(comboBox7.SelectedIndex + 1), Gameplay.GetTankColour(7));
                                }
                                game.CreatePlayer(7, player7);
                                if (playerCounter >= 8)
                                {
                                    Player player8;
                                    if (checkBox15.Checked)
                                    {
                                        player8 = new ComputerPlayer(textBox8.Text, Tank.GetTank(comboBox8.SelectedIndex + 1), Gameplay.GetTankColour(8));
                                    }
                                    else
                                    {
                                        player8 = new PlayerController(textBox8.Text, Tank.GetTank(comboBox8.SelectedIndex + 1), Gameplay.GetTankColour(8));
                                    }
                                    game.CreatePlayer(8, player8);
                                }
                            }
                        }
                    }
                }
            }
            
            game.StartGame();
            this.Close();
        }

        /// <summary>
        /// Whenever this checkbox is checked, enables the items on the same row as the checkbox.
        /// If applicable, enables the checkbox directly below.
        /// If applicable, disables the checkbox above (unless it is the checkbox second from above)
        /// If the checkbox is unchecked, the opposite happens.
        ///
        /// </summary>
        private void checkBox6_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox6.Checked)
            {
                playerCounter++;
                checkBox5.Enabled = true;
                textBox3.Enabled = true;
                comboBox3.Enabled = true;
                checkBox8.Enabled = true;
            }

            else
            {
                playerCounter--;
                checkBox5.Enabled = false;
                textBox3.Enabled = false;
                comboBox3.Enabled = false;
                checkBox8.Enabled = false;
            }
        }

        /// <summary>
        /// Whenever this checkbox is checked, enables the items on the same row as the checkbox.
        /// If applicable, enables the checkbox directly below.
        /// If applicable, disables the checkbox above (unless it is the checkbox second from above)
        /// If the checkbox is unchecked, the opposite happens.
        ///
        /// </summary>
        private void checkBox8_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox8.Checked)
            {
                playerCounter++;
                checkBox7.Enabled = true;
                textBox4.Enabled = true;
                comboBox4.Enabled = true;
                checkBox10.Enabled = true;
                checkBox6.Enabled = false;
            }

            else
            {
                playerCounter--;
                checkBox7.Enabled = false;
                textBox4.Enabled = false;
                comboBox4.Enabled = false;
                checkBox10.Enabled = false;
                checkBox6.Enabled = true;
            }
        }

        /// <summary>
        /// Whenever this checkbox is checked, enables the items on the same row as the checkbox.
        /// If applicable, enables the checkbox directly below.
        /// If applicable, disables the checkbox above (unless it is the checkbox second from above)
        /// If the checkbox is unchecked, the opposite happens.
        ///
        /// </summary>
        private void checkBox10_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox10.Checked)
            {
                playerCounter++;
                checkBox9.Enabled = true;
                textBox5.Enabled = true;
                comboBox5.Enabled = true;
                checkBox12.Enabled = true;
                checkBox8.Enabled = false;
            }

            else
            {
                playerCounter--;
                checkBox9.Enabled = false;
                textBox5.Enabled = false;
                comboBox5.Enabled = false;
                checkBox12.Enabled = false;
                checkBox8.Enabled = true;
            }
        }

        /// <summary>
        /// Whenever this checkbox is checked, enables the items on the same row as the checkbox.
        /// If applicable, enables the checkbox directly below.
        /// If applicable, disables the checkbox above (unless it is the checkbox second from above)
        /// If the checkbox is unchecked, the opposite happens.
        ///
        /// </summary>
        private void checkBox12_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox12.Checked)
            {
                playerCounter++;
                checkBox11.Enabled = true;
                textBox6.Enabled = true;
                comboBox6.Enabled = true;
                checkBox14.Enabled = true;
                checkBox10.Enabled = false;
            }

            else
            {
                playerCounter--;
                checkBox11.Enabled = false;
                textBox6.Enabled = false;
                comboBox6.Enabled = false;
                checkBox14.Enabled = false;
                checkBox10.Enabled = true;
            }
        }

        /// <summary>
        /// Whenever this checkbox is checked, enables the items on the same row as the checkbox.
        /// If applicable, enables the checkbox directly below.
        /// If applicable, disables the checkbox above (unless it is the checkbox second from above)
        /// If the checkbox is unchecked, the opposite happens.
        ///
        /// </summary>
        private void checkBox14_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox14.Checked)
            {
                playerCounter++;
                checkBox13.Enabled = true;
                textBox7.Enabled = true;
                comboBox7.Enabled = true;
                checkBox16.Enabled = true;
                checkBox12.Enabled = false;
            }

            else
            {
                playerCounter--;
                checkBox13.Enabled = false;
                textBox7.Enabled = false;
                comboBox7.Enabled = false;
                checkBox16.Enabled = false;
                checkBox12.Enabled = true;
            }
        }

        /// <summary>
        /// Whenever this checkbox is checked, enables the items on the same row as the checkbox.
        /// If applicable, enables the checkbox directly below.
        /// If applicable, disables the checkbox above (unless it is the checkbox second from above)
        /// If the checkbox is unchecked, the opposite happens.
        ///
        /// </summary>
        private void checkBox16_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox16.Checked)
            {
                playerCounter++;
                checkBox15.Enabled = true;
                textBox8.Enabled = true;
                comboBox8.Enabled = true;
                checkBox14.Enabled = false;
            }

            else
            {
                playerCounter--;
                checkBox15.Enabled = false;
                textBox8.Enabled = false;
                comboBox8.Enabled = false;
                checkBox14.Enabled = true;
            }
        }


        // Useless leftover functions.
        private void label1_Click(object sender, EventArgs e)
        {

        }
        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void label6_Click(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void label7_Click(object sender, EventArgs e)
        {

        }
        
        private void checkBox9_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void textBox5_TextChanged(object sender, EventArgs e)
        {

        }

        private void comboBox5_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void label9_Click(object sender, EventArgs e)
        {

        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {

        }
        
    }
}
