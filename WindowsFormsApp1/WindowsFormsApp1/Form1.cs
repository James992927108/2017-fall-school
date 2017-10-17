using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
//1.匯入命名空間System.Drawing
using System.Drawing;
using System.IO;
using System.Linq;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        List<int> NodeNumList = new List<int>();
        List<NodeStruct> NodeList = new List<NodeStruct>(); //紀錄所有的Node
        ArrayList ReadFileArrayList = new ArrayList();//讀檔用
        private int RemainDateCount = 0;//用於紀錄測資剩餘次數
        public Form1()
        {
            InitializeComponent();
        }

        private void Run_Click(object sender, EventArgs e)
        {
            //2.建立控制項的Graphic物件，將這動作想像是開啟一個空白畫布
            Graphics gra = this.panel1.CreateGraphics();
            //3.新增Pen物件，想像他是一隻筆
            Pen myPen = new Pen(Color.Red, 1);
            //4.在控制項上繪製，想像你在空白畫布上畫東西
            //4.1繪製直線
            gra.DrawLine(myPen, 1, 1, 100, 100);
            //4.2繪製正方形
            gra.DrawRectangle(myPen, 10, 20, 80, 80);
            //4.3繪製一拋物線
            gra.DrawArc(myPen, 10, 20, 70, 80, 123, 233);
            //4.4繪製一矩形
            gra.DrawRectangle(myPen, 50, 60, 110, 120);

        }

        private void Clean_Click(object sender, EventArgs e)
        {
            clean_function();
        }

        public void clean_function()
        {
            NodeList.Clear();//清空有問題
            NodeNumList.Clear();
            this.button_Next.Hide();
            this.panel1.Refresh();
        }

        //--------------------------------------------------------------------------------------------------
        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {

        }

        private void OpenFile_Click(object sender, EventArgs e)
        {
            clean_function();

            string line;
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                System.IO.StreamReader file = new
                    System.IO.StreamReader(openFileDialog1.FileName);
                FileStream fileStream = new FileStream(@"..\aaaa.txt", FileMode.Create);
                fileStream.Close();
                //StreamWriter sw = new StreamWriter(@"..\aaaa.txt");
                while ((line = file.ReadLine()) != null)
                {
                    string[] lineArray = line.Select(o => o.ToString()).ToArray();
                    if (lineArray.Length != 0 && lineArray[0] != "#")
                    {
                        ReadFileArrayList.Add(line);
//                        sw.Write(line);
//                        sw.Write(System.Environment.NewLine);
                    }
                }
                //sw.Close();
                //MessageBox.Show(file.ReadToEnd());
                file.Close();
            }
            for (int i = 0; i < ReadFileArrayList.Count; i++)
            {
                Char delimiter = ' ';
                String[] substrings = ReadFileArrayList[i].ToString().Split(delimiter);
                if (substrings.Length ==1)
                {
                    //NodeNumList的值代表有幾個點
                    NodeNumList.Add(Convert.ToInt32(ReadFileArrayList[i]));
                }
                else//為座標放入NodeList裡面
                {  
                    NodeStruct node = new NodeStruct();//新增點結構
                    node.x = Convert.ToInt32(substrings[0]);
                    node.y = Convert.ToInt32(substrings[1]);
                    NodeList.Add(node);//塞到list裡面                   
                }
            }
            this.button_Next.Show();//show按鈕，顯示測資個數
            this.button_Next.Text = $"{NodeNumList.Count}";
            RemainDateCount = NodeNumList.Count;

        }

        private void Next_Click(object sender, EventArgs e)
        {
            RemainDateCount -- ;
            if (RemainDateCount == -1)
            {
                MessageBox.Show("已無資料");
                this.button_Next.Hide();
            }
            this.button_Next.Text = $"{RemainDateCount}";
            for (int i = 0; i < NodeNumList.Count; i++)
            {
                for (int j = 0; j < NodeNumList[0] - 1; j++)
                {
                    var bm = get_NodeBitmap();
                    //画在哪里 ( Graphics.FromHwnd(this.panel1.Handle) = this.panel1.CreateGraphics();)     
                    Graphics g = Graphics.FromHwnd(this.panel1.Handle);
//                    g.DrawImageUnscaled(bm, e.X, e.Y);       //具体坐标
                    Pen myPen = new Pen(Color.Red, 1); //新增畫筆

//                    NodeStruct node = new NodeStruct();//新增點結構
//                    node.x = e.X;
//                    node.y = e.Y;
//                    NodeList.Add(node);//塞到list裡面
//
//                    if (NodeList.Count != 1)
//                    {
                        g.DrawLine(myPen, NodeList[j].x, NodeList[j].y, NodeList[j+1].x, NodeList[j+1].y);
//                    }
                }

            }

        }

        private void Output_txt_Click(object sender, EventArgs e)
        {

        }
        

        //--------------------------------------------------------------------------------------------------
        //        private void OnPanelMouseMove(object sender, MouseEventArgs e) => Text = $"Coordinate [{e.X},{e.Y}]";
        //        private void OnPanelMouseLeave(object sender, EventArgs e) => Text = "Voronoi Homework";


        private void OnPanelMouseDown(object sender, MouseEventArgs e)
        {
            var bm = get_NodeBitmap();
            //画在哪里 ( Graphics.FromHwnd(this.panel1.Handle) = this.panel1.CreateGraphics();)     
            Graphics g = Graphics.FromHwnd(this.panel1.Handle);
            g.DrawImageUnscaled(bm, e.X, e.Y);       //具体坐标
            Pen myPen = new Pen(Color.Red, 1); //新增畫筆

            NodeStruct node = new NodeStruct();//新增點結構
            node.x = e.X;
            node.y = e.Y;
            NodeList.Add(node);//塞到list裡面

            if (NodeList.Count != 1)
            {
                g.DrawLine(myPen, NodeList[NodeList.Count - 2].x, NodeList[NodeList.Count - 2].y, e.X, e.Y);
            }
        }

        private static Bitmap get_NodeBitmap()
        {
            int NodeSize = 5;
            Bitmap bm = new Bitmap(NodeSize, NodeSize); //这里调整点的大小   
            for (int i = 0; i < NodeSize; i++)
            {
                for (int j = 0; j < NodeSize; j++)
                {
                    bm.SetPixel(i, j, Color.Black); //设置点的颜色   
                }
            }
            return bm;
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        //--------------------------------------------------------------------------------------------------

        private void TEST_Click(object sender, EventArgs e)
        {
            int a = 5;
            Bitmap bm = new Bitmap(a, a);     //这里调整点的大小   
            for (int i = 0; i < a; i++)
            {
                for (int j = 0; j < a; j++)
                {
                    bm.SetPixel(i, j, Color.Black);       //设置点的颜色   
                }
            }
            Graphics g = Graphics.FromHwnd(this.panel1.Handle);     //画在哪里     
            g.DrawImageUnscaled(bm, 100, 100);       //具体坐标
        }

       
    }

    public struct NodeStruct
    {
        public int x, y;

        public NodeStruct(int p1, int p2)
        {
            x = p1;
            y = p2;
        }
    }

}
