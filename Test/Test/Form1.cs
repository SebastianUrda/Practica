using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;
using System.Net.Mail;
using System.Net;
using System.IO;
using System.Data.SqlClient;

namespace Test
{
    public partial class Form1 : Form
    {
        String[] ports;
        SerialPort port;
        bool isConnected = false;
        public Form1()
        {
            InitializeComponent();
            getAvailableComPorts();
            disableControls();
            foreach (string port in ports)
            {
                comboBox1.Items.Add(port);
                Console.WriteLine(port);
                if (ports[0] != null)
                {
                    comboBox1.SelectedItem = ports[0];
                }
            }
        }
        private void updateTextBox()
        {
            richTextBox1.Text += port.ReadExisting() + "\n";
            richTextBox1.ScrollToCaret();
        }
        private void connectToArduino()
        {
            isConnected = true;
            string selectedPort = comboBox1.GetItemText(comboBox1.SelectedItem);
            port = new SerialPort(selectedPort, 9600);
            port.Open();
            enableControls();
            button1.Text = "Disconnect";
            richTextBox1.AppendText("Connected " + isConnected + "\n");

        }
        private void readFromExcel()
        {
            try
            {
                Microsoft.Office.Interop.Excel.Application xlApp = new Microsoft.Office.Interop.Excel.Application();
                Microsoft.Office.Interop.Excel.Workbook xlWorkbook = xlApp.Workbooks.Open(@"" + textBox2.Text);
                Microsoft.Office.Interop.Excel._Worksheet xlWorksheet = xlWorkbook.Sheets[1];
                Microsoft.Office.Interop.Excel.Range xlRange = xlWorksheet.UsedRange;
                int rowCount = xlRange.Rows.Count;
                int colCount = xlRange.Columns.Count;
                int rows = rowCount / 3;
                var sb = new System.Text.StringBuilder();
                sb.Append(rows);
                port.WriteLine(sb.ToString());
                sb.Clear();
                for (int i = 1; i <= rowCount; i++)
                {

                    for (int j = 1; j <= colCount; j++)
                    {
                        sb = new System.Text.StringBuilder();
                        sb.Append(xlRange.Cells[i, j].Value2.ToString());
                        port.WriteLine(sb.ToString());
                        sb.Clear();
                        string feedBack = port.ReadLine();
                        Console.WriteLine(feedBack);
                    }

                }
                xlApp.Workbooks.Close();
                MessageBox.Show("Sending Done!");
                string feedBack1 = port.ReadLine();
                Console.WriteLine(feedBack1);
                feedBack1 = port.ReadLine();
                Console.WriteLine(feedBack1);
                updateTextBox();
            }
            catch { MessageBox.Show("Could not open Excel file!"); }

        }
        private void disconnectFromArduino()
        {
            isConnected = false;
            port.Close();
            disableControls();
            button1.Text = "Connect";
            richTextBox1.AppendText("Disconnected " + isConnected + "\n");
            richTextBox1.Clear();
        }
        void getAvailableComPorts()
        {
            ports = SerialPort.GetPortNames();
        }
        private void disableControls()
        {
            richTextBox1.Enabled = false;
            textBox2.Enabled = false;
            button4.Enabled = false;
        }
        private void enableControls()
        {
            richTextBox1.Enabled = true;
            textBox2.Enabled = true;
            button4.Enabled = true;
        }
        private void Form1_Load(object sender, EventArgs e)
        {

        }
        private void button1_Click(object sender, EventArgs e)
        {
            if (!isConnected)
            {
                connectToArduino();
            }
            else
            {
                disconnectFromArduino();
            }
        }
        private void button4_Click(object sender, EventArgs e)
        {
            readFromExcel();
        }
    }
}
