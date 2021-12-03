using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {

        int startflag = 0;
        int flag_sensor;
        string RxString;
        string temp = "30";
        public Form1()
        {
            InitializeComponent();
        }

        private void serial_start_Click(object sender, EventArgs e)

        {
            serialPort1.PortName = "COM4";
            serialPort1.BaudRate = 9600;

            serialPort1.Open();
            if (serialPort1.IsOpen)
            {
                textBox1.ReadOnly = false;
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
                serialPort1.Close();

            serialPort1.PortName = "COM4";
            serialPort1.BaudRate = 9600;
        }

        private void read_in_TS_channel_Click(object sender, EventArgs e)
        {
            WebClient client = new WebClient();
            label1.Text = client.DownloadString("http://api.thingspeak.com/channels/1566862/field/field1/last.text");

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            {
                if (!string.Equals(textBox1.Text, ""))
                {
                    if (serialPort1.IsOpen) serialPort1.Close();
                    try
                    {

                        const string WRITEKEY = "K5HO9RZVOU98PKL8";
                        string strUpdateBase = "http://api.thingspeak.com/update";
                        string strUpdateURI = strUpdateBase + "?api_key=" + WRITEKEY;
                        string strField1 = textBox1.Text;

                        HttpWebRequest ThingsSpeakReq;
                        HttpWebResponse ThingsSpeakResp;
                        strUpdateURI += "&field1=" + strField1;

                        flag_sensor++;
                        ThingsSpeakReq = (HttpWebRequest)WebRequest.Create(strUpdateURI);
                        ThingsSpeakResp = (HttpWebResponse)ThingsSpeakReq.GetResponse();
                        ThingsSpeakResp.Close();
                        if (!(string.Equals(ThingsSpeakResp.StatusDescription, "OK")))
                        {
                            Exception exData = new Exception(ThingsSpeakResp.StatusDescription);
                            throw exData;
                        }
                    }
                    catch (Exception ex)
                    {
                    }
                    textBox1.Text = "";
                    serialPort1.Open();
                }
            }
        }
        private void serial_stop_Click(object sender, EventArgs e)
        {
            serialPort1.Close();
            textBox1.ReadOnly = true;
        }

        private void current_data_Click(object sender, EventArgs e)
        {
            textBox1.AppendText(RxString);
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }
        private void SerialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            RxString = serialPort1.ReadExisting();
            this.Invoke(new EventHandler(current_data_Click)); /
        }
    }
}
