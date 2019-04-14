using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Chapter8_Ex3
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            InterestCalculator simpleObj = new SimpleInterestCalculator();
            simpleObj.calculateInterest();
            
        }

      
        private void button2_Click(object sender, EventArgs e)
        {
            InterestCalculator compObj = new CompoundInterestCalculator();
            compObj.calculateInterest();
        }
    }

    class SimpleInterestCalculator : InterestCalculator
    {
        #region InterestCalculator Members

        void InterestCalculator.calculateInterest()
        {
            MessageBox.Show("Will calculate Simple Interest");
        }

        #endregion
    }

    class CompoundInterestCalculator : InterestCalculator
    {
        #region InterestCalculator Members

        void InterestCalculator.calculateInterest()
        {
            MessageBox.Show("Will calculate Compound Interest");
        }

        #endregion
    }
}