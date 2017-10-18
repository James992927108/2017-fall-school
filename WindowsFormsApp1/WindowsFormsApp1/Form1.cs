﻿using System;
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
            //            NodeList.Clear();//清空有問題
            //            NodeNumList.Clear();
            //            this.button_Next.Hide();
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
                //FileStream fileStream = new FileStream(@"..\aaaa.txt", FileMode.Create);
                //fileStream.Close();
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
                if (substrings.Length == 1)
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
            this.button_Next.Text = $"{NodeNumList.Count - 2}";
            RemainDateCount = NodeNumList.Count - 2;
        }

        private void Next_Click(object sender, EventArgs e)
        {
            int CurrentDataIndex = (NodeNumList.Count - 2) - RemainDateCount;

            if (RemainDateCount == -1)
            {
                MessageBox.Show("已無資料");
                this.button_Next.Hide();
            }
            this.button_Next.Text = $"{RemainDateCount}";

            var nodeBitmap = get_NodeBitmap();
            //画在哪里 ( Graphics.FromHwnd(this.panel1.Handle) = this.panel1.CreateGraphics();)     
            Graphics g = Graphics.FromHwnd(this.panel1.Handle);
            //新增字型用
            Font myFont = new Font(FontFamily.GenericSansSerif, 10, FontStyle.Regular);
            //新增畫筆用於畫線
            Pen myPen = new Pen(Color.Red, 1);

            for (int i = 0; i < NodeNumList[CurrentDataIndex] - 1; i++)
            {
                g.DrawImageUnscaled(nodeBitmap, NodeList[i].x, NodeList[i].y);
                g.DrawImageUnscaled(nodeBitmap, NodeList[i + 1].x, NodeList[i + 1].y);

                g.DrawString($"{NodeList[i].x},{NodeList[i].y}", myFont, Brushes.Firebrick, NodeList[i].x, NodeList[i].y);
                g.DrawString($"{NodeList[i + 1].x},{NodeList[i + 1].y}", myFont, Brushes.Firebrick, NodeList[i + 1].x, NodeList[i + 1].y);

                g.DrawLine(myPen, NodeList[i].x, NodeList[i].y, NodeList[i + 1].x, NodeList[i + 1].y);
            }
            //NodeList[0].x, NodeList[0].y => node放到nodelist後面，並移除nodelist的第一個，用於下一次計算
            for (int i = 0; i < NodeNumList[CurrentDataIndex]; i++)
            {
                NodeStruct node = new NodeStruct();//新增點結構
                node.x = NodeList[0].x;
                node.y = NodeList[0].y;
                NodeList.Add(node);//塞到list裡面
                NodeList.RemoveAt(0);
            }
            RemainDateCount--;
        }

        private void Output_txt_Click(object sender, EventArgs e)
        {
            FileStream fileStream = new FileStream(@"..\bbb.txt", FileMode.Create);
            fileStream.Close();
            StreamWriter sw = new StreamWriter(@"..\bbb.txt");
            //將NodeList中的元素跟具x值排序
            var NodeList_Sort = from a in NodeList 
                                orderby a.x
                                select a;

            foreach (var node in NodeList_Sort)
            {
                sw.Write("P " + node.x + " " + node.y);
                sw.Write(System.Environment.NewLine);
            }
            sw.Close();
            MessageBox.Show("完成輸出txt");
        }
        //--------------------------------------------------------------------------------------------------
        //        private void OnPanelMouseMove(object sender, MouseEventArgs e) => Text = $"Coordinate [{e.X},{e.Y}]";
        //        private void OnPanelMouseLeave(object sender, EventArgs e) => Text = "Voronoi Homework";

        private void OnPanelMouseDown(object sender, MouseEventArgs e)
        {
            var nodeBitmap = get_NodeBitmap();
            //画在哪里 ( Graphics.FromHwnd(this.panel1.Handle) = this.panel1.CreateGraphics();)     
            Graphics g = Graphics.FromHwnd(this.panel1.Handle);
            //新增字型用
            Font myFont = new Font(FontFamily.GenericSansSerif, 10, FontStyle.Regular);
            //新增畫筆用於畫線
            Pen myPen = new Pen(Color.Red, 1);
            //點座標
            g.DrawImageUnscaled(nodeBitmap, e.X, e.Y);
            // Draw the $"Coordinate [{e.X},{e.Y}]"
            g.DrawString($"{e.X},{e.Y}", myFont, Brushes.Firebrick, e.X, e.Y);

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
            Bitmap nodeBitmap = new Bitmap(NodeSize, NodeSize); //这里调整点的大小   
            for (int i = 0; i < NodeSize; i++)
            {
                for (int j = 0; j < NodeSize; j++)
                {
                    nodeBitmap.SetPixel(i, j, Color.Black); //设置点的颜色   
                }
            }
            return nodeBitmap;
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
