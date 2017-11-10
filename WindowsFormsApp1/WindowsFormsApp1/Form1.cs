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
using System.Windows;
using System.Drawing;
using System.IO;
using System.Linq;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        List<int> NodeNumList = new List<int>();
        List<NodeStruct> NodeList = new List<NodeStruct>(); //紀錄所有的Node
        List<EdgeStruct> EdgeList = new List<EdgeStruct>(); //紀錄所有的Edge

        private int RemainDateCount = 0;//用於紀錄測資剩餘次數
        private int CurrentDataIndex = 0;//用於讀取NodeNumList中第幾個值
        Font myFont = new Font(FontFamily.GenericSansSerif, 10, FontStyle.Regular);//新增字型用
        Pen myPen = new Pen(Color.Red, 1);//新增畫筆用於畫線
        bool IsReadFile = false;
        public Form1()
        {
            InitializeComponent();
        }

        private void Clean_Click(object sender, EventArgs e)
        {
            Clean_function();
        }

        public void Clean_function()//清除當前邊與點
        {
            if (EdgeList.Count() != 0)
            {
                int temp = EdgeList.Count();
                for (int i = 0; i < temp; i++)
                {
                    EdgeList.RemoveAt(0);
                }
            }
            if (IsReadFile == true)
            {
                if (CurrentDataIndex != 0) //第一次不須移除
                {
                    for (int i = 0; i < NodeNumList[CurrentDataIndex - 1]; i++)
                    {
                        NodeList.RemoveAt(0);
                    }
                }
            }
            else
            {
                int temp = NodeList.Count();

                for (int i = 0; i < temp; i++)
                {
                    NodeList.RemoveAt(0);
                }
            }
            this.panel1.Refresh();
        }

        private void Next_Click(object sender, EventArgs e)
        {
            CurrentDataIndex = NodeNumList.Count - RemainDateCount;//總數-剩餘次數即，為當前index
            //每次要先清除上次的
            Clean_function();
            if (RemainDateCount == 0)
            {
                MessageBox.Show("已無資料");
                this.button_Next.Hide();
            }
            else
            {
                RemainDateCount--;
                this.button_Next.Text = $"{RemainDateCount}";
                DrawNode(NodeNumList[CurrentDataIndex]);//根據點數量，先畫出點
            }
        }
        //--------------------------------------------------------------------------------------------------
        private void OnPanelMouseDown(object sender, MouseEventArgs e)
        {
            IsReadFile = false;
            NodeStruct node = new NodeStruct(e.X, e.Y);//新增點結構
            NodeList.Add(node);//將點塞到list裡面
            DrawNode(NodeList.Count);//根據點數量，先畫出點
        }

        private void Run_Click(object sender, EventArgs e)
        {
            if (IsReadFile == false)
            {
                DrawVerticalLine(NodeList.Count);
            }
            else
            {
                DrawVerticalLine(NodeNumList[CurrentDataIndex]);
            }
        }
        //--------------------------------------------------------------------------------------------------

        private void DrawVerticalLine(int NodeCount)
        {
            Graphics g = Graphics.FromHwnd(this.panel1.Handle);
            if (NodeCount == 2)
            {
                NodeStruct A = NodeList[0];
                NodeStruct B = NodeList[1];
                NodeStruct Mid = new NodeStruct();
                if (A.X == B.X && A.Y != B.Y)//垂直
                {
                    Mid.Y = (A.Y + B.Y) / 2;
                    g.DrawLine(myPen, 0, Mid.Y, 600, Mid.Y);
                    EdgeStruct edge = new EdgeStruct(0, Mid.Y, 600, Mid.Y);
                    EdgeList.Add(edge);
                }
                if (A.Y == B.Y && A.X != B.X)//水平
                {
                    Mid.X = (A.X + B.X) / 2;
                    g.DrawLine(myPen, Mid.X, 0, Mid.X, 600);
                    EdgeStruct edge = new EdgeStruct(Mid.X, 0, Mid.X, 600);
                    EdgeList.Add(edge);
                }
                if (A.X != B.X && A.Y != B.Y)//不同點
                {
                    NodeStruct normal_vector = new NodeStruct();//得ab法向量(y,-x)
                    normal_vector.X = A.Y - B.Y;
                    normal_vector.Y = -(A.X - B.X);
                    Mid.X = (A.X + B.X) / 2;
                    Mid.Y = (A.Y + B.Y) / 2;
                    int K = 10000;
                    NodeStruct topNode = new NodeStruct();
                    topNode.X = Mid.X + K * normal_vector.X;
                    topNode.Y = Mid.Y + K * normal_vector.Y;
                    NodeStruct downNode = new NodeStruct();
                    downNode.X = Mid.X - K * normal_vector.X;
                    downNode.Y = Mid.Y - K * normal_vector.Y;
                    g.DrawLine(myPen, topNode.X, topNode.Y, downNode.X, downNode.Y);
                    EdgeStruct edge = new EdgeStruct(topNode.X, topNode.Y, downNode.X, downNode.Y);
                    EdgeList.Add(edge);
                }
            }
            if (NodeCount == 3) //三點時要找外心
            {
                ClockwiseSortPoints();

                NodeStruct Excenter = new NodeStruct();//三點時要找外心
                Excenter = GetTriangleExcenter(NodeList[0], NodeList[1], NodeList[2]);
                if (Excenter.X != 0 && Excenter.Y != 0)//有找到外心時
                {
                    g.DrawImageUnscaled(get_NodeBitmap(), Excenter.X, Excenter.Y);

                    //做逆時針排序，用於找每一條邊的法向量
                    //以逆時針順序記錄三角形的三個頂點（三角形的三條邊變成有向邊）。這麼做的好處是，三角形依序取兩條邊計算叉積，就得到朝外的法向量

                    List<NodeStruct> normal_vector_List = new List<NodeStruct>();

                    //法向量為，若ab向量為(x,y)->法向量為(y,-x)

                    for (int i = 0; i < 3; i++)
                    {
                        NodeStruct normal_vector = new NodeStruct();
                        normal_vector.X = NodeList[(i + 1) % 3].Y - NodeList[i].Y;
                        normal_vector.Y = -(NodeList[(i + 1) % 3].X - NodeList[i].X);
                        normal_vector_List.Add(normal_vector);
                    }

                    //取的法向量後，找出每邊依法向量方向過每邊中點到邊界的點，預設找一點*k倍法向量
                    List<NodeStruct> MidNode_List = new List<NodeStruct>();
                    for (int i = 0; i < 3; i++)
                    {
                        NodeStruct MidNode = new NodeStruct();
                        MidNode.X = (NodeList[(i + 1) % 3].X + NodeList[i].X) / 2;
                        MidNode.Y = (NodeList[(i + 1) % 3].Y + NodeList[i].Y) / 2;
                        MidNode_List.Add(MidNode);
                    }

                    List<NodeStruct> Vertical_line_List = new List<NodeStruct>();
                    int K = 10000;
                    for (int i = 0; i < 3; i++)
                    {
                        NodeStruct Vertical_line = new NodeStruct();
                        Vertical_line.X = MidNode_List[i].X + K * normal_vector_List[i].X;
                        Vertical_line.Y = MidNode_List[i].Y + K * normal_vector_List[i].Y;
                        Vertical_line_List.Add(Vertical_line);
                    }
                    for (int i = 0; i < 3; i++)
                    {
                        g.DrawLine(myPen, Vertical_line_List[i].X, Vertical_line_List[i].Y, Excenter.X, Excenter.Y);
                        EdgeStruct edge = new EdgeStruct(Vertical_line_List[i].X, Vertical_line_List[i].Y, Excenter.X, Excenter.Y);
                        EdgeList.Add(edge);
                    }
                }
                else//沒有外心時
                {
                    NodeStruct Mid1 = new NodeStruct();
                    NodeStruct Mid2 = new NodeStruct();
                    if (NodeList[0].X == NodeList[1].X && NodeList[0].X == NodeList[2].X)//垂直
                    {
                        List<int> tempList = new List<int>();
                        tempList.Add(NodeList[0].Y);
                        tempList.Add(NodeList[1].Y);
                        tempList.Add(NodeList[2].Y);
                        tempList.Sort();
                        //找2中點做2條水平線，對y軸要排序
                        Mid1.Y = (tempList[0] + tempList[1]) / 2;
                        Mid2.Y = (tempList[1] + tempList[2]) / 2;

                        g.DrawLine(myPen, 0, Mid1.Y, 600, Mid1.Y);
                        g.DrawLine(myPen, 0, Mid2.Y, 600, Mid2.Y);

                        EdgeStruct edge1 = new EdgeStruct(0, Mid1.Y, 600, Mid1.Y);
                        EdgeList.Add(edge1);
                        EdgeStruct edge2 = new EdgeStruct(0, Mid2.Y, 600, Mid2.Y);
                        EdgeList.Add(edge2);
                    }
                    if (NodeList[0].Y == NodeList[1].Y && NodeList[0].Y == NodeList[2].Y)//水平
                    {
                        //找2中點做2條水平線，對x軸要排序
                        List<int> tempList = new List<int>();
                        tempList.Add(NodeList[0].X);
                        tempList.Add(NodeList[1].X);
                        tempList.Add(NodeList[2].X);
                        tempList.Sort();
                        Mid1.X = (tempList[0] + tempList[1]) / 2;
                        Mid2.X = (tempList[1] + tempList[2]) / 2;

                        g.DrawLine(myPen, Mid1.X, 0, Mid1.X, 600);
                        g.DrawLine(myPen, Mid2.X, 0, Mid2.X, 600);

                        EdgeStruct edge1 = new EdgeStruct(Mid1.X, 0, Mid1.X, 600);
                        EdgeList.Add(edge1);
                        EdgeStruct edge2 = new EdgeStruct(Mid2.X, 0, Mid2.X, 600);
                        EdgeList.Add(edge2);
                    }
                    if ((NodeList[0].Y / NodeList[0].X) == (NodeList[1].Y / NodeList[1].X) && (NodeList[0].Y / NodeList[0].X) == (NodeList[2].Y / NodeList[2].X))//為一直線
                    {
                        NodeStruct normal_vector = new NodeStruct();//得ab法向量(y,-x)
                        normal_vector.X = NodeList[0].Y - NodeList[1].Y;
                        normal_vector.Y = -(NodeList[0].X - NodeList[1].X);
                        int K = 10000;

                        Mid1.X = (NodeList[0].X + NodeList[1].X) / 2;
                        Mid1.Y = (NodeList[0].Y + NodeList[1].Y) / 2;
                        NodeStruct topNode1 = new NodeStruct();
                        topNode1.X = Mid1.X + K * normal_vector.X;
                        topNode1.Y = Mid1.Y + K * normal_vector.Y;
                        NodeStruct downNode1 = new NodeStruct();
                        downNode1.X = Mid1.X - K * normal_vector.X;
                        downNode1.Y = Mid1.Y - K * normal_vector.Y;

                        g.DrawLine(myPen, topNode1.X, topNode1.Y, downNode1.X, downNode1.Y);
                        EdgeStruct edge1 = new EdgeStruct(topNode1.X, topNode1.Y, downNode1.X, downNode1.Y);
                        EdgeList.Add(edge1);

                        Mid2.X = (NodeList[2].X + NodeList[1].X) / 2;
                        Mid2.Y = (NodeList[2].Y + NodeList[1].Y) / 2;
                        NodeStruct topNode2 = new NodeStruct();
                        topNode2.X = Mid2.X + K * normal_vector.X;
                        topNode2.Y = Mid2.Y + K * normal_vector.Y;
                        NodeStruct downNode2 = new NodeStruct();
                        downNode2.X = Mid2.X - K * normal_vector.X;
                        downNode2.Y = Mid2.Y - K * normal_vector.Y;

                        g.DrawLine(myPen, topNode2.X, topNode2.Y, downNode2.X, downNode2.Y);
                        EdgeStruct edge2 = new EdgeStruct(topNode2.X, topNode2.Y, downNode2.X, downNode2.Y);
                        EdgeList.Add(edge2);
                    }
                }
            }
        }
        //--------------------------------------------------------------------------------------------------
        private void ClockwiseSortPoints()
        {
            NodeStruct center;
            double x = 0, y = 0;
            for (int i = 0; i < 3; i++)
            {
                x += NodeList[i].X;
                y += NodeList[i].Y;
            }
            center.X = (int)x / 3;
            center.Y = (int)y / 3;
            //冒泡排序
            for (int i = 0; i < 3 - 1; i++)
            {
                for (int j = 0; j < 3 - i - 1; j++)
                {
                    if (PointCmp(NodeList[j], NodeList[j + 1], center))
                    {
                        NodeStruct tmp = NodeList[j];
                        NodeList[j] = NodeList[j + 1];
                        NodeList[j + 1] = tmp;
                    }
                }
            }
        }
        bool PointCmp(NodeStruct a, NodeStruct b, NodeStruct center)
        {
            if (a.X >= 0 && b.X < 0)
                return true;
            if (a.X == 0 && b.X == 0)
                return a.Y > b.Y;
            //向量OA和向量OB的叉积
            int det = (a.X - center.X) * (b.Y - center.Y) - (b.X - center.X) * (a.Y - center.Y);
            if (det < 0)
                return true;
            if (det > 0)
                return false;
            //向量OA和向量OB共线，以距离判断大小
            int d1 = (a.X - center.X) * (a.X - center.X) + (a.Y - center.Y) * (a.Y - center.Y);
            int d2 = (b.X - center.X) * (b.X - center.Y) + (b.Y - center.Y) * (b.Y - center.Y);
            return d1 > d2;
        }
        //--------------------------------------------------------------------------------------------------

        private NodeStruct GetTriangleExcenter(NodeStruct A, NodeStruct B, NodeStruct C)
        {
            NodeStruct Excenter = new NodeStruct();//新增點結構
            NodeStruct noExcenter = new NodeStruct(0, 0);//用於不存在外心時回傳
            //same point
            if (A.X == B.X && A.Y == B.Y && A.X == C.X && A.Y == C.Y)
            {
                Excenter = A;
                return Excenter;
            }
            //三點共線，利用面積，判断 (ax-cx)*(by-cy) == (bx-cx)*(ay-cy)
            if ((A.X - C.X) * (B.Y - C.Y) == (B.X - C.X) * (A.Y - C.Y))
            {
                return noExcenter;
            }
            double x1 = A.X, x2 = B.X, x3 = C.X, y1 = A.Y, y2 = B.Y, y3 = C.Y;
            double C1 = Math.Pow(x1, 2) + Math.Pow(y1, 2) - Math.Pow(x2, 2) - Math.Pow(y2, 2);
            double C2 = Math.Pow(x2, 2) + Math.Pow(y2, 2) - Math.Pow(x3, 2) - Math.Pow(y3, 2);
            double centery = (C1 * (x2 - x3) - C2 * (x1 - x2)) / (2 * (y1 - y2) * (x2 - x3) - 2 * (y2 - y3) * (x1 - x2));
            double centerx = (C1 - 2 * centery * (y1 - y2)) / (2 * (x1 - x2));
            Excenter = new NodeStruct(Convert.ToInt32(centerx), Convert.ToInt32(centery));
            return Excenter;
        }
        //--------------------------------------------------------------------------------------------------
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
        //--------------------------------------------------------------------------------------------------
        private void DrawNode(int NodeCount)
        {
            Graphics g = Graphics.FromHwnd(this.panel1.Handle);
            for (int i = 0; i < NodeCount; i++)
            {
                g.DrawImageUnscaled(get_NodeBitmap(), NodeList[i].X, NodeList[i].Y);
                g.DrawString($"{NodeList[i].X},{NodeList[i].Y}", myFont, Brushes.Firebrick, NodeList[i].X, NodeList[i].Y);
            }
        }
        //--------------------------------------------------------------------------------------------------
        private void OnPanelMouseMove(object sender, MouseEventArgs e) => Text = $"Coordinate [{e.X},{e.Y}]";
        private void OnPanelMouseLeave(object sender, EventArgs e) => Text = "Voronoi Homework";

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void StepByStep_Click(object sender, EventArgs e)
        {

        }
        //--------------------------------------------------------------------------------------------------
        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {

        }
        private void OpenFile_Click(object sender, EventArgs e)
        {
            IsReadFile = true;//用於輸出
            ArrayList ReadFileArrayList = new ArrayList();//讀檔用
            string line;
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                System.IO.StreamReader file = new System.IO.StreamReader(openFileDialog1.FileName);
                while ((line = file.ReadLine()) != null)
                {
                    string[] lineArray = line.Select(o => o.ToString()).ToArray();
                    if (lineArray.Length != 0 && lineArray[0] != "#")
                    {
                        ReadFileArrayList.Add(line);
                    }
                }
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
                    node.X = Convert.ToInt32(substrings[0]);
                    node.Y = Convert.ToInt32(substrings[1]);
                    NodeList.Add(node);//塞到list裡面                   
                }
            }
            this.button_Next.Show();//show按鈕，顯示測資個數
            this.button_Next.Text = $"{NodeNumList.Count}";
            RemainDateCount = NodeNumList.Count;
        }

        private void Output_txt_Click(object sender, EventArgs e)
        {
            FileStream fileStream = new FileStream(@"..\..\..\bbb.txt", FileMode.Create);
            fileStream.Close();
            StreamWriter sw = new StreamWriter(@"..\..\..\bbb.txt");
            //當前點個數

            if (IsReadFile != true)
            {
                //將NodeList中的元素跟具x值排序
                var NodeList_Sort = NodeList.OrderBy(a => a.X).ThenBy(b => b.Y).ToList();
                foreach (var node in NodeList_Sort)
                {
                    sw.Write("P " + node.X + " " + node.Y);
                    sw.Write(System.Environment.NewLine);
                }
            }
            else
            {
                List<NodeStruct> NodeListCopy = new List<NodeStruct>();
                for (int i = 0; i < NodeNumList[CurrentDataIndex]; i++)
                {
                    NodeStruct temp = new NodeStruct();
                    temp = NodeList[i];
                    NodeListCopy.Add(temp);
                }
                var NodeList_Sort = NodeListCopy.OrderBy(a => a.X).ThenBy(b => b.Y).ToList();
                foreach (var node in NodeList_Sort)
                {
                    sw.Write("P " + node.X + " " + node.Y);
                    sw.Write(System.Environment.NewLine);
                }
            }
            var EdgeList_Sort = EdgeList.OrderBy(a => a.X1).ThenBy(b => b.Y1).ThenBy(c => c.X2).ThenBy(d => d.Y2).ToList();
            foreach (var edge in EdgeList_Sort)
            {
                sw.Write("E " + edge.X1 + " " + edge.Y1 + " " + edge.X2 + " " + edge.Y2);
                sw.Write(System.Environment.NewLine);
            }
            sw.Close();
            MessageBox.Show("完成輸出txt");
        }

        private void tableLayoutPanel1_Paint(object sender, PaintEventArgs e)
        {

        }
        //--------------------------------------------------------------------------------------------------
    }

    public struct NodeStruct
    {
        public int X, Y;
        public NodeStruct(int p1, int p2)
        {
            X = p1;
            Y = p2;
        }
    }
    public struct EdgeStruct
    {
        public int X1, Y1, X2, Y2;
        public EdgeStruct(int p1, int p2, int p3, int p4)
        {
            X1 = p1;
            Y1 = p2;
            X2 = p3;
            Y2 = p4;
        }
    }
}
