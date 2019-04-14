using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace EmployeeDetails
{
    public partial class Form1 : Form
    {
        SqlConnection con = new SqlConnection();
        DataTable dt;
        DataRow dr;
        string code;
        int flag;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // TODO: This line of code loads data into the 'hRDataSet.empdetails' table. You can move, or remove it, as needed.
            this.empdetailsTableAdapter.Fill(this.hRDataSet.empdetails);
            con.ConnectionString = "Data Source=SQLSERVER01; Initial Catalog=HR; User ID=sa; Password=niit#1234";
            con.Open();
            txtcode.Enabled = false;
            txtname.Enabled = false;
            txtaddress.Enabled = false;
            txtState.Enabled = false;
            txtCountry.Enabled = false;
            cbDesignation.Enabled = false;
            cbDepartment.Enabled = false;
            cbDesignation.Items.Add("MANAGER");
            cbDesignation.Items.Add("AUTHOR");
            cbDesignation.Items.Add("Designer");
            cbDepartment.Items.Add("MARKETING");
            cbDepartment.Items.Add("FINANCE");
            cbDepartment.Items.Add("IDD");
            cmbSave.Enabled = false;
        }

        private void cmdAdd_Click(object sender, EventArgs e)
        {
            cmbSave.Enabled = true;
            txtname.Enabled = true ;
            txtaddress.Enabled = true;
            txtState.Enabled = true;
            txtCountry.Enabled = true;
            cbDesignation.Enabled = true;
            cbDepartment.Enabled = true;
            txtname.Text = "";
            txtaddress.Text = "";
            txtState.Text = "";
            txtCountry.Text = "";
            cbDesignation.Text = "";
            cbDepartment.Text = "";
            flag = 1;
            int ctr, len;
            string codeval;
            dt = hRDataSet.Tables["empdetails"];
            len = dt.Rows.Count - 1;
            dr = dt.Rows[len];
            code = dr["ccode"].ToString();
            codeval = code.Substring(1, 3);
            ctr = Convert.ToInt32(codeval);
            if ((ctr >= 1) && (ctr < 9))
            {
                ctr = ctr + 1;
                txtcode.Text="C00"+ctr;
            }
            else if ((ctr >= 9) && (ctr < 99))
            {
                ctr = ctr + 1;
                txtcode.Text = "C0" + ctr;
            }
            else if (ctr >= 99)
            {
                ctr = ctr + 1;
                txtcode.Text = "C" + ctr;
            }
            cmdAdd.Enabled = false;
            
        }

        private void cmbSave_Click(object sender, EventArgs e)
        {
            if (flag == 1)
            {
                dt = hRDataSet.Tables["empdetails"];
                dr = dt.NewRow();
                dr[0] = txtcode.Text;
                dr[1] = txtname.Text;
                dr[2] = txtaddress.Text;
                dr[3] = txtState.Text;
                dr[4] = txtCountry.Text;
                dr[5] = cbDesignation.SelectedItem;
                dr[6] = cbDepartment.SelectedItem;
                dt.Rows.Add(dr);
                txtcode.Enabled = true;
                empdetailsTableAdapter.Update(hRDataSet);
            }
            else if (flag == 2)
            {
                dt = hRDataSet.Tables["empdetails"];
                dr = dt.Rows.Find(code);
                dr.BeginEdit();
                dr[0] = txtcode.Text;
                dr[1] = txtname.Text.Trim();
                dr[2] = txtaddress.Text.Trim();
                dr[3] = txtState.Text.Trim();
                dr[4] = txtCountry.Text.Trim();
                dr[5] = cbDesignation.SelectedItem;
                dr[6] = cbDepartment.SelectedItem;
                dr.EndEdit();
                txtcode.Enabled = true;
            }
            else if (flag == 3)
            {
                dr.Delete();
            }
            flag = 0;
            empdetailsTableAdapter.Update(hRDataSet);
            txtcode.Enabled = false;
            txtname.Enabled = false;
            txtaddress.Enabled = false;
           txtState.Enabled = false;
           txtCountry.Enabled = false;
           cbDesignation.Enabled = false;
          cbDepartment.Enabled = false;
          cmdAdd.Enabled = true;
          cmbSave.Enabled = false;
        }

        private void cmbDelete_Click(object sender, EventArgs e)
        {
            string code;
            code = txtcode.Text;
            dt = hRDataSet.Tables["empdetails"];
            dr = dt.Rows.Find(code);
            flag = 3;
            MessageBox.Show(code);
            dr.Delete();
            empdetailsTableAdapter.Update(hRDataSet);
       }

             
    }
}