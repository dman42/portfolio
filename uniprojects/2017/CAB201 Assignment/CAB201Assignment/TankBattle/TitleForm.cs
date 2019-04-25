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
    /// Title window of the game.
    ///
    /// </summary>
    public partial class TitleForm : Form
    {
        public TitleForm()
        {
            InitializeComponent();
        }

        /// <summary>
        /// When the new game button is pressed, opens setup screen...
        ///
        /// </summary>
        private void newGameButton_Click(object sender, EventArgs e) 
        {
            Setup setup = new Setup();
            setup.Show();
        }

        private void TitleForm_Load(object sender, EventArgs e)
        {

        }
    }
}
