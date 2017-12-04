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
using WindowsFormsApp1;


namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        List<int> NodeNumList = new List<int>();
        List<DataStruct.Node> NodeList = new List<DataStruct.Node>(); //紀錄所有的Node
        List<DataStruct.Edge> EdgeList = new List<DataStruct.Edge>(); //紀錄所有的Edge
        List<DataStruct.Node> OutputNodeList = new List<DataStruct.Node>(); //紀錄所有的Node
        List<DataStruct.Edge> OutputEdgeList = new List<DataStruct.Edge>();
        DataStruct.Node[] TL = new DataStruct.Node[20000];
        List<DataStruct.Edge> TangentLine_List = new List<DataStruct.Edge>();
        public int FirstTime_TangentLine_List_Num = 0;
        List<DataStruct.Node> L_part_List = new List<DataStruct.Node>();
        List<DataStruct.Node> R_part_List = new List<DataStruct.Node>();
        List<PointF> HP = new List<PointF>();
        List<DataStruct.Edge> VerticalList = new List<DataStruct.Edge>();

        private int RemainDateCount = 0;//用於紀錄測資剩餘次數
        private int CurrentDataIndex = 0;//用於讀取NodeNumList中第幾個值
        Font myFont = new Font(FontFamily.GenericSansSerif, 10, FontStyle.Regular);//新增字型用
        Pen pen = new Pen(Color.Red, 1);//新增畫筆用於畫線
        Pen pen_in_tangent = new Pen(Color.Blue, 1);//新增畫筆用於畫線
        Pen pen_in_divide = new Pen(Color.Green, 1);//新增畫筆用於畫線
        Pen pen_in_hyper = new Pen(Color.Yellow, 1);//新增畫筆用於畫線
        Pen pen_in_vertical = new Pen(Color.Purple, 1);//新增畫筆用於畫線
        bool IsReadFile = false;
        int K = 1000;
        enum step { Divide, TangentLine, Hyperplane, Eliminate };
        public int whichstep = 0;
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
            whichstep = 0;
            HP.Clear();
            VerticalList.Clear();
            EdgeList.Clear();
            TangentLine_List.Clear();
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
            DataStruct.Node node = new DataStruct.Node(e.X, e.Y);//新增點結構
            NodeList.Add(node);//將點塞到list裡面
            DrawNode(NodeList.Count);//根據點數量，先畫出點
        }

        private void Run_Click(object sender, EventArgs e)
        {
            if (IsReadFile == false)
            {
                List<DataStruct.Node> tempList = new List<DataStruct.Node>();
                for (int i = 0; i < NodeList.Count; i++)
                {
                    tempList.Add(NodeList[i]);
                }
                for (int i = 0; i < 4; i++)
                {
                    DrawVerticalLine(NodeList.Count, tempList, i);
                }
            }
            else
            {
                List<DataStruct.Node> tempList = new List<DataStruct.Node>();
                for (int i = 0; i < NodeNumList[CurrentDataIndex]; i++)
                {
                    tempList.Add(NodeList[i]);
                }
                for (int i = whichstep; i < 4; i++)
                {
                    DrawVerticalLine(NodeNumList[CurrentDataIndex], tempList, i);
                }
            }
        }
        //--------------------------------------------------------------------------------------------------
        private void StepByStep_Click(object sender, EventArgs e)
        {
            if (IsReadFile == false)
            {
                List<DataStruct.Node> tempNodeList = new List<DataStruct.Node>();
                for (int i = 0; i < NodeList.Count; i++)
                {
                    tempNodeList.Add(NodeList[i]);
                }
                DrawVerticalLine(NodeList.Count, tempNodeList, whichstep);
                whichstep++;
            }
            else
            {
                List<DataStruct.Node> tempNodeList = new List<DataStruct.Node>();
                for (int i = 0; i < NodeNumList[CurrentDataIndex]; i++)
                {
                    tempNodeList.Add(NodeList[i]);
                }
                DrawVerticalLine(NodeNumList[CurrentDataIndex], tempNodeList, whichstep);
                whichstep++;
            }
        }

        //--------------------------------------------------------------------------------------------------

        private void DrawVerticalLine(int NodeCount, List<DataStruct.Node> tempNodeList, int step)
        {
            if (NodeCount == 2)
            {
                TwoNodeFun(tempNodeList);
            }
            //--------------------------------------------------------------------------------------------------
            if (NodeCount == 3) //三點時要找外心
            {
                ThreeNodeFun(tempNodeList);
            }
            //--------------------------------------------------------------------------------------------------
            if (NodeCount > 3)
            {
                Graphics g = Graphics.FromHwnd(this.panel1.Handle);
                if (step == (int)Form1.step.Divide)
                {
                    Divide(NodeCount, tempNodeList);//取得L_part_List與R_part_List
                }
                if (step == (int)Form1.step.TangentLine)
                {
                    Get_TangentLine(tempNodeList);//取得TangentLine_List
                }
                if (step == (int)Form1.step.Hyperplane)
                {
                    //畫Hyperplane
                    //DrawVerticalLine(L_part_List.Count, L_part_List);
                    //DrawVerticalLine(R_part_List.Count, R_part_List);
                    DataStruct.Node last_SG_Node = new DataStruct.Node();
                    DataStruct.Edge Down_TangentLine = new DataStruct.Edge(TangentLine_List[1].X2, TangentLine_List[1].Y2,
                        TangentLine_List[1].X1, TangentLine_List[1].Y1);
                    DataStruct.Node P_L = new DataStruct.Node(TangentLine_List[0].L_Node().X, TangentLine_List[0].L_Node().Y);
                    DataStruct.Node P_R = new DataStruct.Node(TangentLine_List[0].R_Node().X, TangentLine_List[0].R_Node().Y);
                    DataStruct.Edge SG = new DataStruct.Edge(P_L.X, P_L.Y, P_R.X, P_R.Y);
                    PointF HP_FirstNode = new PointF(SG.Vertical_top_Node().X, SG.Vertical_top_Node().Y);
                    HP.Add(HP_FirstNode);
                    do
                    {
                        g.DrawLine(Pens.OrangeRed, SG.X1, SG.Y1, SG.X2, SG.Y2);
                        List<DataStruct.Node> L_part = new List<DataStruct.Node>();
                        for (int i = 0; i < L_part_List.Count; i++)
                        {
                            if (!(L_part_List[i].X == SG.X1 && L_part_List[i].Y == SG.Y1) &&
                                !(L_part_List[i].X == last_SG_Node.X && L_part_List[i].Y == last_SG_Node.Y))
                            {
                                L_part.Add(L_part_List[i]);
                            }
                        }
                        List<DataStruct.Edge> L_lineList = new List<DataStruct.Edge>();
                        for (int i = 0; i < L_part.Count; i++)
                        {
                            DataStruct.Edge temp = new DataStruct.Edge(SG.X1, SG.Y1, L_part[i].X, L_part[i].Y);
                            L_lineList.Add(temp);
                        }
                        List<DataStruct.Node> R_part = new List<DataStruct.Node>();
                        for (int i = 0; i < R_part_List.Count; i++)
                        {
                            if (!(R_part_List[i].X == SG.X2 && R_part_List[i].Y == SG.Y2) &&
                                !(R_part_List[i].X == last_SG_Node.X && R_part_List[i].Y == last_SG_Node.Y))
                            {
                                R_part.Add(R_part_List[i]);
                            }
                        }
                        List<DataStruct.Edge> R_lineList = new List<DataStruct.Edge>();
                        for (int i = 0; i < R_part.Count; i++)
                        {
                            DataStruct.Edge temp = new DataStruct.Edge(SG.X2, SG.Y2, R_part[i].X, R_part[i].Y);
                            R_lineList.Add(temp);
                        }

                        Dictionary<PointF, int> L_IntersectionNodeList = new Dictionary<PointF, int>();
                        Dictionary<PointF, int> R_IntersectionNodeList = new Dictionary<PointF, int>();
                        //SG的中垂線
                        PointF SG_top = new PointF(SG.Vertical_top_Node().X, SG.Vertical_top_Node().Y);
                        PointF SG_down = new PointF(SG.Vertical_down_Node().X, SG.Vertical_down_Node().Y);
                        if (cmpLine(SG, Down_TangentLine) && FirstTime_TangentLine_List_Num == 3) //解決內點三角形問題
                        {
                            L_lineList.Clear();
                            R_lineList.Clear();
                        }
                        if (L_lineList.Count != 0)
                        {
                            for (int i = 0; i < L_lineList.Count; i++)
                            {
                                var top_x = L_lineList[i].Vertical_top_Node().X;
                                var top_y = L_lineList[i].Vertical_top_Node().Y;
                                var down_x = L_lineList[i].Vertical_down_Node().X;
                                var down_y = L_lineList[i].Vertical_down_Node().Y;
                                PointF top = new PointF(top_x, top_y);
                                PointF down = new PointF(down_x, down_y);
                                PointF IntersectionNode = new PointF();
                                IntersectionNode = GetIntersection(top, down, SG_top, SG_down);
                                L_IntersectionNodeList.Add(IntersectionNode, i);
                                g.DrawLine(pen_in_vertical, top_x, top_y, down_x, down_y);
                            }
                        }
                        if (R_lineList.Count != 0)
                        {
                            for (int i = 0; i < R_lineList.Count; i++)
                            {
                                var top_x = R_lineList[i].Vertical_top_Node().X;
                                var top_y = R_lineList[i].Vertical_top_Node().Y;
                                var down_x = R_lineList[i].Vertical_down_Node().X;
                                var down_y = R_lineList[i].Vertical_down_Node().Y;
                                PointF top = new PointF(top_x, top_y);
                                PointF down = new PointF(down_x, down_y);
                                PointF IntersectionNode = new PointF();
                                IntersectionNode = GetIntersection(top, down, SG_top, SG_down);
                                R_IntersectionNodeList.Add(IntersectionNode, i);
                                g.DrawLine(pen_in_vertical, top_x, top_y, down_x, down_y);
                            }
                        }
                        //從xz_IntersectionNodeList與yz_IntersectionNodeList找y值最小的點
                        var L_Y_small_Node = L_IntersectionNodeList.OrderBy(o => o.Key.Y).FirstOrDefault();
                        var R_Y_small_Node = R_IntersectionNodeList.OrderBy(o => o.Key.Y).FirstOrDefault();
                        //--------------------------------------------------------------------------------------------
                        if (R_lineList.Count == 0 && L_lineList.Count == 0)
                        {
                            SG.X1 = Down_TangentLine.X1;
                            SG.Y1 = Down_TangentLine.Y1;
                            SG.X2 = Down_TangentLine.X2;
                            SG.Y2 = Down_TangentLine.Y2;
                        }
                        else if (L_lineList.Count == 0 && R_lineList.Count != 0)
                        {
                            HP.Add(R_Y_small_Node.Key);
                            DataStruct.Edge temp = new DataStruct.Edge();
                            temp = R_lineList[R_Y_small_Node.Value].VerticalLine();
                            VerticalList.Add(temp);
                            if (FirstTime_TangentLine_List_Num == 3)
                            {
                                last_SG_Node.X = SG.X2;
                                last_SG_Node.Y = SG.Y2;
                                SG.X2 = Down_TangentLine.X2;
                                SG.Y2 = Down_TangentLine.Y2;
                            }
                            else
                            {
                                SG.X1 = Down_TangentLine.X1;
                                SG.Y1 = Down_TangentLine.Y1;
                                SG.X2 = Down_TangentLine.X2;
                                SG.Y2 = Down_TangentLine.Y2;
                            }
                        }
                        else if (L_lineList.Count != 0 && R_lineList.Count == 0)
                        {
                            HP.Add(L_Y_small_Node.Key);
                            DataStruct.Edge temp = new DataStruct.Edge();
                            temp = L_lineList[L_Y_small_Node.Value].VerticalLine();
                            VerticalList.Add(temp);
                            if (FirstTime_TangentLine_List_Num == 3)
                            {
                                last_SG_Node.X = SG.X1;
                                last_SG_Node.Y = SG.Y1;
                                SG.X1 = Down_TangentLine.X1;
                                SG.Y1 = Down_TangentLine.Y1;
                            }
                            else
                            {
                                SG.X1 = Down_TangentLine.X1;
                                SG.Y1 = Down_TangentLine.Y1;
                                SG.X2 = Down_TangentLine.X2;
                                SG.Y2 = Down_TangentLine.Y2;
                            }
                        }
                        else
                        {
                            if (L_Y_small_Node.Key.X == 0 && L_Y_small_Node.Key.Y == 0)
                            {
                                HP.Add(R_Y_small_Node.Key);
                                DataStruct.Edge temp = new DataStruct.Edge();
                                temp = R_lineList[R_Y_small_Node.Value].VerticalLine();
                                VerticalList.Add(temp);
                                last_SG_Node.X = SG.X2;
                                last_SG_Node.Y = SG.Y2;
                                if (R_lineList.Count != 0)
                                {
                                    if (R_lineList[R_Y_small_Node.Value].X1 == SG.X2 &&
                                        R_lineList[R_Y_small_Node.Value].Y1 == SG.Y2)
                                    {
                                        SG.X2 = R_lineList[R_Y_small_Node.Value].X2;
                                        SG.Y2 = R_lineList[R_Y_small_Node.Value].Y2;
                                    }
                                    else
                                    {
                                        SG.X2 = R_lineList[R_Y_small_Node.Value].X1;
                                        SG.Y2 = R_lineList[R_Y_small_Node.Value].Y1;
                                    }
                                }
                            }
                            else if (R_Y_small_Node.Key.X == 0 && R_Y_small_Node.Key.Y == 0)
                            {
                                HP.Add(L_Y_small_Node.Key);
                                DataStruct.Edge temp = new DataStruct.Edge();
                                temp = L_lineList[L_Y_small_Node.Value].VerticalLine();
                                VerticalList.Add(temp);
                                last_SG_Node.X = SG.X1;
                                last_SG_Node.Y = SG.Y1;
                                if (L_lineList.Count != 0)
                                {
                                    if (L_lineList[L_Y_small_Node.Value].X1 == last_SG_Node.X &&
                                        L_lineList[L_Y_small_Node.Value].Y1 == last_SG_Node.Y)
                                    {
                                        SG.X1 = L_lineList[L_Y_small_Node.Value].X2;
                                        SG.Y1 = L_lineList[L_Y_small_Node.Value].Y2;
                                    }
                                    else
                                    {
                                        SG.X1 = L_lineList[L_Y_small_Node.Value].X1;
                                        SG.Y1 = L_lineList[L_Y_small_Node.Value].Y1;
                                    }
                                }
                            }
                            else if (L_Y_small_Node.Key.Y > R_Y_small_Node.Key.Y)
                            {

                                HP.Add(R_Y_small_Node.Key);
                                DataStruct.Edge temp = new DataStruct.Edge();
                                temp = R_lineList[R_Y_small_Node.Value].VerticalLine();
                                VerticalList.Add(temp);
                                last_SG_Node.X = SG.X2;
                                last_SG_Node.Y = SG.Y2;
                                if (R_lineList.Count != 0)
                                {
                                    if (R_lineList[R_Y_small_Node.Value].X1 == SG.X2 &&
                                        R_lineList[R_Y_small_Node.Value].Y1 == SG.Y2)
                                    {
                                        SG.X2 = R_lineList[R_Y_small_Node.Value].X2;
                                        SG.Y2 = R_lineList[R_Y_small_Node.Value].Y2;
                                    }
                                    else
                                    {
                                        SG.X2 = R_lineList[R_Y_small_Node.Value].X1;
                                        SG.Y2 = R_lineList[R_Y_small_Node.Value].Y1;
                                    }
                                }
                            }
                            else
                            {
                                HP.Add(L_Y_small_Node.Key);
                                DataStruct.Edge temp = new DataStruct.Edge();
                                temp = L_lineList[L_Y_small_Node.Value].VerticalLine();
                                VerticalList.Add(temp);
                                last_SG_Node.X = SG.X1;
                                last_SG_Node.Y = SG.Y1;
                                if (L_lineList.Count != 0)
                                {
                                    if (L_lineList[L_Y_small_Node.Value].X1 == last_SG_Node.X &&
                                        L_lineList[L_Y_small_Node.Value].Y1 == last_SG_Node.Y)
                                    {
                                        SG.X1 = L_lineList[L_Y_small_Node.Value].X2;
                                        SG.Y1 = L_lineList[L_Y_small_Node.Value].Y2;
                                    }
                                    else
                                    {
                                        SG.X1 = L_lineList[L_Y_small_Node.Value].X1;
                                        SG.Y1 = L_lineList[L_Y_small_Node.Value].Y1;
                                    }
                                }
                            }
                        }
                        //--------------------------------------------------------------------------------------------
                        for (int i = 0; i < HP.Count; i++)
                        {
                            g.DrawImageUnscaled(get_NodeBitmap(), Convert.ToInt32(HP[i].X), Convert.ToInt32(HP[i].Y));
                            //g.DrawString($"{HP[i].X},{HP[i].X}", myFont, Brushes.Firebrick, HP[i].X, HP[i].X);
                        }
                    } while (!checkIfsameEdge(SG, Down_TangentLine));
                    for (int i = 0; i < HP.Count - 1; i++)
                    {
                        g.DrawLine(pen_in_hyper, HP[i].X, HP[i].Y, HP[i + 1].X, HP[i + 1].Y);
                    }
                    g.DrawLine(Pens.OrangeRed, SG.X1, SG.Y1, SG.X2, SG.Y2);
                    PointF HP_LastNode = new PointF(SG.Vertical_down_Node().X, SG.Vertical_down_Node().Y);
                    g.DrawLine(pen_in_hyper, HP[HP.Count - 1].X, HP[HP.Count - 1].Y, HP_LastNode.X, HP_LastNode.Y);
                    HP.Add(HP_LastNode);
                }
                if (step == (int)Form1.step.Eliminate)
                {
                    this.panel1.Refresh();
                    EdgeList.Clear();
                    OutputNodeList.Clear();
                    for (int i = 0; i < HP.Count - 1; i++)
                    {
                        g.DrawLine(pen_in_hyper, HP[i].X, HP[i].Y, HP[i + 1].X, HP[i + 1].Y);
                        DataStruct.Edge temp = new DataStruct.Edge(HP[i].X, HP[i].Y, HP[i + 1].X, HP[i + 1].Y);
                        EdgeList.Add(temp);
                    }
                    for (int i = 0; i < HP.Count; i++)
                    {
                        DataStruct.Node temp = new DataStruct.Node(HP[i].X, HP[i].Y);
                        OutputNodeList.Add(temp);
                    }
                    for (int i = 0; i < VerticalList.Count; i++)
                    {
                        //先計算HP[i]HP[i+1]，HP[i]HP[i+2]的外積，記錄下狀態為A，之後與算HP[i]與第j條VerticalList的左點z，記錄下狀態為B，
                        //與算HP[i]與第j條VerticalList的右點x，記錄下狀態為C，如果B與A相同則代表只留HP[i+1]x這條邊
                        DataStruct.Node NodeA = new DataStruct.Node(Convert.ToInt32(HP[i].X), Convert.ToInt32(HP[i].Y));
                        DataStruct.Node NodeB = new DataStruct.Node(Convert.ToInt32(HP[i + 1].X), Convert.ToInt32(HP[i + 1].Y));
                        DataStruct.Node NodeC = new DataStruct.Node(Convert.ToInt32(HP[i + 2].X), Convert.ToInt32(HP[i + 2].Y));
                        PointF AB_vecter = new PointF(NodeB.X - NodeA.X, NodeB.Y - NodeA.Y);
                        PointF AC_vectet = new PointF(NodeC.X - NodeA.X, NodeC.Y - NodeA.Y);
                        PointF AR_Node_vecter = new PointF(VerticalList[i].R_Node().X - NodeA.X, VerticalList[i].R_Node().Y - NodeA.Y);
                        PointF AL_Node_vecter = new PointF(VerticalList[i].L_Node().X - NodeA.X, VerticalList[i].L_Node().Y - NodeA.Y);
                        //oa * ob = oa.x * ob.y - oa.y * ob.x 外積公式
                        var ABAC = AB_vecter.X * AC_vectet.Y - AB_vecter.Y * AC_vectet.X;
                        ABAC = getstate(ABAC);
                        var ABAR = AB_vecter.X * AR_Node_vecter.Y - AB_vecter.Y * AR_Node_vecter.X;
                        ABAR = getstate(ABAR);
                        var ABAL = AB_vecter.X * AL_Node_vecter.Y - AB_vecter.Y * AL_Node_vecter.X;
                        ABAL = getstate(ABAL);
                        if (ABAC != 0)
                        {
                            if (ABAC == ABAR)
                            {
                                PointF aa = new PointF(HP[i + 1].X, HP[i + 1].Y);
                                PointF bb = new PointF(VerticalList[i].L_Node().X, VerticalList[i].L_Node().Y);
                                for (int j = 0; j < HP.Count; j++)
                                {
                                    var qq = Math.Round((bb.Y - aa.Y) / (bb.X - aa.X), 1);
                                    var ww = Math.Round((HP[j].Y - aa.Y) / (HP[j].X - aa.X), 1);
                                    if (qq == ww & HP[j] != aa)
                                    {
                                        bb = HP[j];
                                    }
                                }
                                DataStruct.Edge temp = new DataStruct.Edge(aa.X, aa.Y, bb.X, bb.Y);
                                g.DrawLine(pen_in_hyper, temp.X1, temp.Y1, temp.X2, temp.Y2);
                                DataStruct.Node tempbb = new DataStruct.Node(bb.X, bb.Y);
                                OutputNodeList.Add(tempbb);
                                EdgeList.Add(temp);
                            }
                            else if (ABAC == ABAL)
                            {
                                PointF aa = new PointF(HP[i + 1].X, HP[i + 1].Y);
                                PointF bb = new PointF(VerticalList[i].R_Node().X, VerticalList[i].R_Node().Y);
                                for (int j = 0; j < HP.Count; j++)
                                {
                                    var qq = Math.Round((bb.Y - aa.Y) / (bb.X - aa.X), 1);
                                    var ww = Math.Round((HP[j].Y - aa.Y) / (HP[j].X - aa.X), 1);
                                    if (qq == ww & HP[j] != aa)
                                    {
                                        bb = HP[j];
                                    }
                                }
                                DataStruct.Edge temp = new DataStruct.Edge(aa.X, aa.Y, bb.X, bb.Y);
                                g.DrawLine(pen_in_hyper, temp.X1, temp.Y1, temp.X2, temp.Y2);
                                DataStruct.Node tempbb = new DataStruct.Node(bb.X, bb.Y);
                                OutputNodeList.Add(tempbb);
                                EdgeList.Add(temp);
                            }
                        }
                        else{
                            PointF aa = new PointF(VerticalList[i].R_Node().X, VerticalList[i].R_Node().Y);
                            PointF bb = new PointF(VerticalList[i].L_Node().X, VerticalList[i].L_Node().Y);                         
                            DataStruct.Edge temp = new DataStruct.Edge(aa.X, aa.Y, bb.X, bb.Y);
                            g.DrawLine(pen_in_hyper, temp.X1, temp.Y1, temp.X2, temp.Y2);
                            DataStruct.Node tempbb = new DataStruct.Node(bb.X, bb.Y);
                            OutputNodeList.Add(tempbb);
                            EdgeList.Add(temp);
                        }
                    }
                }
            }
        }

        private int getstate(float state)
        {
            int fin_state = 0;
            if (state > 0)
            {
                fin_state = 1;
            }
            else if (state < 0)
            {
                fin_state = -1;
            }
            else
            {
                fin_state = 0;
            }
            return fin_state;
        }
        private bool checkIfsameEdge(DataStruct.Edge a, DataStruct.Edge b)
        {
            if ((a.X1 == b.X1 && a.Y1 == b.Y1) && (a.X2 == b.X2 && a.Y2 == b.Y2))
                return true;
            else if ((a.X1 == b.X2 && a.Y1 == b.Y2) && (a.X2 == b.X1 && a.Y2 == b.Y1))
                return true;
            else
                return false;
        }
        private bool cmpLine(DataStruct.Edge a, DataStruct.Edge b)
        {
            if (a.X1 == b.X1 && a.Y1 == b.Y1 && a.X2 == b.X2 && a.Y2 == b.Y2)
                return true;
            else
                return false;
        }
        public static PointF GetIntersection(PointF lineFirstStar, PointF lineFirstEnd, PointF lineSecondStar, PointF lineSecondEnd)
        {
            float a = 0, b = 0;
            int state = 0;
            if (lineFirstStar.X != lineFirstEnd.X)
            {
                a = (lineFirstEnd.Y - lineFirstStar.Y) / (lineFirstEnd.X - lineFirstStar.X);
                state |= 1;
            }
            if (lineSecondStar.X != lineSecondEnd.X)
            {
                b = (lineSecondEnd.Y - lineSecondStar.Y) / (lineSecondEnd.X - lineSecondStar.X);
                state |= 2;
            }
            switch (state)
            {
                case 0: //L1與L2都平行Y軸
                    {
                        if (lineFirstStar.X == lineSecondStar.X)
                        {
                            //throw new Exception("兩條直線互相重合，且平行於Y軸，無法計算交點。");
                            return new PointF(0, 0);
                        }
                        else
                        {
                            //throw new Exception("兩條直線互相平行，且平行於Y軸，無法計算交點。");
                            return new PointF(0, 0);
                        }
                    }
                case 1: //L1存在斜率, L2平行Y軸
                    {
                        float x = lineSecondStar.X;
                        float y = (lineFirstStar.X - x) * (-a) + lineFirstStar.Y;
                        return new PointF(Convert.ToInt32(x), Convert.ToInt32(y));
                    }
                case 2: //L1 平行Y軸，L2存在斜率
                    {
                        float x = lineFirstStar.X;
                        //網上有相似代碼的，這一處是錯誤的。你可以對比case 1 的邏輯 進行分析
                        //源code:lineSecondStar * x + lineSecondStar * lineSecondStar.X + p3.Y;
                        float y = (lineSecondStar.X - x) * (-b) + lineSecondStar.Y;
                        return new PointF(Convert.ToInt32(x), Convert.ToInt32(y));
                    }
                case 3: //L1，L2都存在斜率
                    {
                        if (a == b)
                        {
                            // throw new Exception("兩條直線平行或重合，無法計算交點。");
                            return new PointF(0, 0);
                        }
                        float x = (a * lineFirstStar.X - b * lineSecondStar.X - lineFirstStar.Y + lineSecondStar.Y) / (a - b);
                        float y = a * x - a * lineFirstStar.X + lineFirstStar.Y;
                        return new PointF(Convert.ToInt32(x), Convert.ToInt32(y));
                    }
            }
            // throw new Exception("不可能發生的情況");
            return new PointF(0, 0);
        }
        //--------------------------------------------------------------------------------------------------
        private void Get_TangentLine(List<DataStruct.Node> tempList)
        {
            Graphics g = Graphics.FromHwnd(this.panel1.Handle);
            MergeSort(0, tempList.Count - 1, tempList);
            int m = 0;
            for (int i = 0; i < tempList.Count; i++)
            {
                while (m >= 2 && cross(TL[m - 2], TL[m - 1], tempList[i]) < 0)
                {
                    m--;
                }
                TL[m++] = tempList[i];
            }
            for (int i = tempList.Count - 2, t = m + 1; i >= 0; i--)
            {
                while (m >= t && cross(TL[m - 2], TL[m - 1], tempList[i]) < 0)
                {
                    m--;
                }
                TL[m++] = tempList[i];
            }
            for (int i = 0; i < m - 1; i++)
            {
                DataStruct.Edge edge = new DataStruct.Edge(TL[i].X, TL[i].Y, TL[i + 1].X, TL[i + 1].Y);
                TangentLine_List.Add(edge);
                FirstTime_TangentLine_List_Num = TangentLine_List.Count;//用在判斷是否有內角
            }
            //找出EdgeList與TangentLine_List不同的邊，並移除原本EdgeList
            var Tangenttemp = TangentLine_List;
            for (int i = 0; i < TangentLine_List.Count; i++)
            {
                for (int j = 0; j < EdgeList.Count; j++)
                {
                    if (checkIfsameEdge(Tangenttemp[i], EdgeList[j]))
                    {
                        Tangenttemp.RemoveAt(i);
                        if(i!=0)
                            i--;
                    }
                }
            }
            Tangenttemp.OrderBy(o => o.Top_Node().Y);
            TangentLine_List = Tangenttemp;
            for (int i = 0; i < TangentLine_List.Count(); i++)
            {
                g.DrawLine(pen_in_tangent, TangentLine_List[i].X1, TangentLine_List[i].Y1, TangentLine_List[i].X2,
                    TangentLine_List[i].Y2);
                DataStruct.Edge temp = new DataStruct.Edge(TangentLine_List[i].X1, TangentLine_List[i].Y1, TangentLine_List[i].X2,
                    TangentLine_List[i].Y2);
                EdgeList.Add(temp);
            }
            if (TangentLine_List[0].Top_Node().Y > TangentLine_List[1].Top_Node().Y)
            {
                DataStruct.Edge temp = new DataStruct.Edge();
                temp = TangentLine_List[0];
                TangentLine_List[0] = TangentLine_List[1];
                TangentLine_List[1] = temp;
            }
        }
        //oa * ob = oa.x * ob.y - oa.y * ob.x 外積公式
        private int cross(DataStruct.Node o, DataStruct.Node a, DataStruct.Node b)
        {
            return (a.X - o.X) * (b.Y - o.Y) - (a.Y - o.Y) * (b.X - o.X);
        }
        private bool cmp(DataStruct.Node a, DataStruct.Node b)
        {
            return (a.Y < b.Y) || (a.Y == b.Y && a.X < b.X);
        }
        private void MergeSort(int left, int right, List<DataStruct.Node> tempList)
        {
            if (left < right)
            {
                int mid = (left + right) / 2;
                MergeSort(left, mid, tempList);
                MergeSort(mid + 1, right, tempList);
                Merge(left, mid, right, tempList);
            }
        }
        private void Merge(int left, int mid, int right, List<DataStruct.Node> tempList)
        {
            int i = left, j = mid + 1, top = 0;
            DataStruct.Node[] data = new DataStruct.Node[10000];
            while (i <= mid && j <= right)
            {
                if (cmp(tempList[i], tempList[j])) data[top++] = tempList[i++];
                else data[top++] = tempList[j++];
            }
            while (i <= mid) data[top++] = tempList[i++];
            while (j <= right) data[top++] = tempList[j++];
            for (i = 0, j = left; i < top; i++, j++)
                tempList[j] = data[i];
        }
        //--------------------------------------------------------------------------------------------------

        private void Divide(int NodeCount, List<DataStruct.Node> tempList)
        {
            Graphics g = Graphics.FromHwnd(this.panel1.Handle);
            var tempListSort = tempList.OrderBy(a => a.X).ToList();
            L_part_List = tempListSort.GetRange(0, NodeCount / 2).ToList();
            ClockwiseSortPoints(L_part_List);
            R_part_List = tempListSort.GetRange(NodeCount / 2, NodeCount - (NodeCount / 2)).ToList();
            ClockwiseSortPoints(R_part_List);

            DataStruct.Edge edge = new DataStruct.Edge();
            for (int i = 0; i < L_part_List.Count - 1; i++)
            {
                g.DrawLine(pen_in_divide, L_part_List[i].X, L_part_List[i].Y, L_part_List[i + 1].X, L_part_List[i + 1].Y);
                edge = new DataStruct.Edge(L_part_List[i].X, L_part_List[i].Y, L_part_List[i + 1].X, L_part_List[i + 1].Y);
                EdgeList.Add(edge);
            }
            if (L_part_List.Count - 1 != 1)
            {
                g.DrawLine(pen_in_divide, L_part_List[L_part_List.Count - 1].X, L_part_List[L_part_List.Count - 1].Y, L_part_List[0].X,
                    L_part_List[0].Y);
                edge = new DataStruct.Edge(L_part_List[L_part_List.Count - 1].X, L_part_List[L_part_List.Count - 1].Y,
                    L_part_List[0].X, L_part_List[0].Y);
                EdgeList.Add(edge);
            }
            for (int i = 0; i < R_part_List.Count - 1; i++)
            {
                g.DrawLine(pen_in_divide, R_part_List[i].X, R_part_List[i].Y, R_part_List[i + 1].X, R_part_List[i + 1].Y);
                edge = new DataStruct.Edge(R_part_List[i].X, R_part_List[i].Y, R_part_List[i + 1].X, R_part_List[i + 1].Y);
                EdgeList.Add(edge);
            }
            if (R_part_List.Count - 1 != 1)
            {
                g.DrawLine(pen_in_divide, R_part_List[R_part_List.Count - 1].X, R_part_List[R_part_List.Count - 1].Y, R_part_List[0].X,
                    R_part_List[0].Y);
                edge = new DataStruct.Edge(R_part_List[R_part_List.Count - 1].X, R_part_List[R_part_List.Count - 1].Y,
                    R_part_List[0].X, R_part_List[0].Y);
                EdgeList.Add(edge);
            }
        }
        //--------------------------------------------------------------------------------------------------
        private void ThreeNodeFun(List<DataStruct.Node> tempList)
        {
            Graphics g = Graphics.FromHwnd(this.panel1.Handle);

            ClockwiseSortPoints(tempList);
            DataStruct.Node Excenter = new DataStruct.Node(); //三點時要找外心
            Excenter = GetTriangleExcenter(tempList[0], tempList[1], tempList[2]);
            if (Excenter.X != 0 && Excenter.Y != 0) //有找到外心時
            {
                g.DrawImageUnscaled(get_NodeBitmap(), Excenter.X, Excenter.Y);

                //做逆時針排序，用於找每一條邊的法向量
                //以逆時針順序記錄三角形的三個頂點（三角形的三條邊變成有向邊）。這麼做的好處是，三角形依序取兩條邊計算叉積，就得到朝外的法向量
                List<DataStruct.Edge> Edge_List = new List<DataStruct.Edge>();
                for (int i = 0; i < 3; i++)
                {
                    DataStruct.Edge normal_vector = new DataStruct.Edge();
                    normal_vector.X1 = tempList[i].X;
                    normal_vector.Y1 = tempList[i].Y;
                    normal_vector.X2 = tempList[(i + 1) % 3].X;
                    normal_vector.Y2 = tempList[(i + 1) % 3].Y;
                    Edge_List.Add(normal_vector);
                }
                for (int i = 0; i < 3; i++)
                {
                    g.DrawLine(pen, Edge_List[i].Vertical_top_Node().X, Edge_List[i].Vertical_top_Node().Y, Excenter.X, Excenter.Y);
                    DataStruct.Edge edge = new DataStruct.Edge(Edge_List[i].Vertical_top_Node().X, Edge_List[i].Vertical_top_Node().Y, Excenter.X, Excenter.Y);
                    EdgeList.Add(edge);
                }
            }
            else //沒有外心時
            {
                DataStruct.Node Mid1 = new DataStruct.Node();
                DataStruct.Node Mid2 = new DataStruct.Node();
                //垂直
                if (tempList[0].X == tempList[1].X && tempList[0].X == tempList[2].X)
                {
                    List<int> tempList1 = new List<int>();
                    tempList1.Add(tempList[0].Y);
                    tempList1.Add(tempList[1].Y);
                    tempList1.Add(tempList[2].Y);
                    tempList1.Sort();
                    //找2中點做2條水平線，對y軸要排序
                    Mid1.Y = (tempList1[0] + tempList1[1]) / 2;
                    Mid2.Y = (tempList1[1] + tempList1[2]) / 2;

                    g.DrawLine(pen, 0, Mid1.Y, 600, Mid1.Y);
                    g.DrawLine(pen, 0, Mid2.Y, 600, Mid2.Y);

                    DataStruct.Edge edge1 = new DataStruct.Edge(0, Mid1.Y, 600, Mid1.Y);
                    EdgeList.Add(edge1);
                    DataStruct.Edge edge2 = new DataStruct.Edge(0, Mid2.Y, 600, Mid2.Y);
                    EdgeList.Add(edge2);
                }
                //水平
                if (tempList[0].Y == tempList[1].Y && tempList[0].Y == tempList[2].Y)
                {
                    //找2中點做2條水平線，對x軸要排序
                    List<int> tempList1 = new List<int>();
                    tempList1.Add(tempList[0].X);
                    tempList1.Add(tempList[1].X);
                    tempList1.Add(tempList[2].X);
                    tempList1.Sort();
                    Mid1.X = (tempList1[0] + tempList1[1]) / 2;
                    Mid2.X = (tempList1[1] + tempList1[2]) / 2;

                    g.DrawLine(pen, Mid1.X, 0, Mid1.X, 600);
                    g.DrawLine(pen, Mid2.X, 0, Mid2.X, 600);

                    DataStruct.Edge edge1 = new DataStruct.Edge(Mid1.X, 0, Mid1.X, 600);
                    EdgeList.Add(edge1);
                    DataStruct.Edge edge2 = new DataStruct.Edge(Mid2.X, 0, Mid2.X, 600);
                    EdgeList.Add(edge2);
                }
                //為一直線
                if ((tempList[0].Y / tempList[0].X) == (tempList[1].Y / tempList[1].X) &&
                    (tempList[0].Y / tempList[0].X) == (tempList[2].Y / tempList[2].X))
                {
                    DataStruct.Node normal_vector = new DataStruct.Node(); //得ab法向量(y,-x)
                    normal_vector.X = tempList[0].Y - tempList[1].Y;
                    normal_vector.Y = -(tempList[0].X - tempList[1].X);

                    List<DataStruct.Node> tempList3 = new List<DataStruct.Node>();
                    for (int i = 0; i < 3; i++)
                    {
                        tempList3.Add(tempList[i]);
                    }
                    var NodeList_Sort = tempList3.OrderBy(a => a.X).ToList(); //共線時，3點等比，比較x的大小，由小到大依序排序，因此可得到正確的中點
                    //例如 a(200 200)b(100 100)c(300 300) ->sort b a c 
                    Mid1.X = (NodeList_Sort[0].X + NodeList_Sort[1].X) / 2;
                    Mid1.Y = (NodeList_Sort[0].Y + NodeList_Sort[1].Y) / 2;
                    DataStruct.Node topNode1 = new DataStruct.Node();
                    topNode1.X = Mid1.X + K * normal_vector.X;
                    topNode1.Y = Mid1.Y + K * normal_vector.Y;
                    DataStruct.Node downNode1 = new DataStruct.Node();
                    downNode1.X = Mid1.X - K * normal_vector.X;
                    downNode1.Y = Mid1.Y - K * normal_vector.Y;

                    g.DrawLine(pen, topNode1.X, topNode1.Y, downNode1.X, downNode1.Y);
                    DataStruct.Edge edge1 = new DataStruct.Edge(topNode1.X, topNode1.Y, downNode1.X, downNode1.Y);
                    EdgeList.Add(edge1);

                    Mid2.X = (NodeList_Sort[2].X + NodeList_Sort[1].X) / 2;
                    Mid2.Y = (NodeList_Sort[2].Y + NodeList_Sort[1].Y) / 2;
                    DataStruct.Node topNode2 = new DataStruct.Node();
                    topNode2.X = Mid2.X + K * normal_vector.X;
                    topNode2.Y = Mid2.Y + K * normal_vector.Y;
                    DataStruct.Node downNode2 = new DataStruct.Node();
                    downNode2.X = Mid2.X - K * normal_vector.X;
                    downNode2.Y = Mid2.Y - K * normal_vector.Y;

                    g.DrawLine(pen, topNode2.X, topNode2.Y, downNode2.X, downNode2.Y);
                    DataStruct.Edge edge2 = new DataStruct.Edge(topNode2.X, topNode2.Y, downNode2.X, downNode2.Y);
                    EdgeList.Add(edge2);
                }
            }
        }

        private void TwoNodeFun(List<DataStruct.Node> tempList)
        {
            Graphics g = Graphics.FromHwnd(this.panel1.Handle);
            DataStruct.Node A = tempList[0];
            DataStruct.Node B = tempList[1];
            DataStruct.Node Mid = new DataStruct.Node();
            if (A.X == B.X && A.Y != B.Y) //垂直
            {
                Mid.Y = (A.Y + B.Y) / 2;
                g.DrawLine(pen, 0, Mid.Y, 600, Mid.Y);
                DataStruct.Edge edge = new DataStruct.Edge(0, Mid.Y, 600, Mid.Y);
                EdgeList.Add(edge);
            }
            if (A.Y == B.Y && A.X != B.X) //水平
            {
                Mid.X = (A.X + B.X) / 2;
                g.DrawLine(pen, Mid.X, 0, Mid.X, 600);
                DataStruct.Edge edge = new DataStruct.Edge(Mid.X, 0, Mid.X, 600);
                EdgeList.Add(edge);
            }
            if (A.X != B.X && A.Y != B.Y) //不同點
            {
                DataStruct.Node normal_vector = new DataStruct.Node(); //得ab法向量(y,-x)
                normal_vector.X = A.Y - B.Y;
                normal_vector.Y = -(A.X - B.X);
                Mid.X = (A.X + B.X) / 2;
                Mid.Y = (A.Y + B.Y) / 2;

                DataStruct.Node topNode = new DataStruct.Node();
                topNode.X = Mid.X + K * normal_vector.X;
                topNode.Y = Mid.Y + K * normal_vector.Y;
                DataStruct.Node downNode = new DataStruct.Node();
                downNode.X = Mid.X - K * normal_vector.X;
                downNode.Y = Mid.Y - K * normal_vector.Y;
                g.DrawLine(pen, topNode.X, topNode.Y, downNode.X, downNode.Y);
                DataStruct.Edge edge = new DataStruct.Edge(topNode.X, topNode.Y, downNode.X, downNode.Y);
                EdgeList.Add(edge);
            }
        }

        //--------------------------------------------------------------------------------------------------
        private void ClockwiseSortPoints(List<DataStruct.Node> tempList)
        {
            DataStruct.Node center;
            double x = 0, y = 0;
            for (int i = 0; i < tempList.Count; i++)
            {
                x += tempList[i].X;
                y += tempList[i].Y;
            }
            center.X = (int)x / tempList.Count;
            center.Y = (int)y / tempList.Count;
            //冒泡排序
            for (int i = 0; i < tempList.Count - 1; i++)
            {
                for (int j = 0; j < tempList.Count - i - 1; j++)
                {
                    if (PointCmp(tempList[j], tempList[j + 1], center))
                    {
                        DataStruct.Node tmp = tempList[j];
                        tempList[j] = tempList[j + 1];
                        tempList[j + 1] = tmp;
                    }
                }
            }
        }
        bool PointCmp(DataStruct.Node a, DataStruct.Node b, DataStruct.Node center)
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
            int d2 = (b.X - center.X) * (b.X - center.X) + (b.Y - center.Y) * (b.Y - center.Y);
            return d1 > d2;
        }
        //--------------------------------------------------------------------------------------------------

        private DataStruct.Node GetTriangleExcenter(DataStruct.Node A, DataStruct.Node B, DataStruct.Node C)
        {
            DataStruct.Node Excenter = new DataStruct.Node();//新增點結構
            DataStruct.Node noExcenter = new DataStruct.Node(0, 0);//用於不存在外心時回傳
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
            if (A.Y != B.Y && A.X == B.X)//判斷直角三型
            {
                DataStruct.Node temp = new DataStruct.Node();
                temp = C;
                C = B;
                B = A;
                A = temp;
            }
            double x1 = A.X, x2 = B.X, x3 = C.X, y1 = A.Y, y2 = B.Y, y3 = C.Y;
            double C1 = Math.Pow(x1, 2) + Math.Pow(y1, 2) - Math.Pow(x2, 2) - Math.Pow(y2, 2);
            double C2 = Math.Pow(x2, 2) + Math.Pow(y2, 2) - Math.Pow(x3, 2) - Math.Pow(y3, 2);
            double centery = (C1 * (x2 - x3) - C2 * (x1 - x2)) / (2 * (y1 - y2) * (x2 - x3) - 2 * (y2 - y3) * (x1 - x2));
            double centerx = (C1 - 2 * centery * (y1 - y2)) / (2 * (x1 - x2));
            Excenter = new DataStruct.Node(Convert.ToInt32(centerx), Convert.ToInt32(centery));
            return Excenter;
        }
        //--------------------------------------------------------------------------------------------------
        private static Bitmap get_NodeBitmap()
        {
            int NodeSize = 3;
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
                    if (lineArray.Length != 0 && lineArray[0] == "0")
                    {
                        break;
                    }
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
                    DataStruct.Node node = new DataStruct.Node();//新增點結構
                    node.X = Convert.ToInt32(substrings[0]);
                    node.Y = Convert.ToInt32(substrings[1]);
                    NodeList.Add(node);//塞到list裡面                   
                }
            }
            this.button_Next.Show();//show按鈕，顯示測資個數
            this.button_Next.Text = $"{NodeNumList.Count}";
            RemainDateCount = NodeNumList.Count;
        }
        private void OpenOutputFile_Click(object sender, EventArgs e)
        {
            string line;
            Graphics g = Graphics.FromHwnd(this.panel1.Handle);

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                System.IO.StreamReader file = new System.IO.StreamReader(openFileDialog1.FileName);
                while ((line = file.ReadLine()) != null)
                {
                    Char delimiter = ' ';
                    String[] substrings = line.Split(delimiter);
                    if (substrings[0] == "P")
                    {
                        g.DrawImageUnscaled(get_NodeBitmap(), Convert.ToInt32(substrings[1]), Convert.ToInt32(substrings[2]));
                        g.DrawString($"{substrings[1]},{substrings[2]}", myFont, Brushes.Firebrick, Convert.ToInt32(substrings[1]), Convert.ToInt32(substrings[2]));
                    }
                    if (substrings[0] == "E")
                    {
                        g.DrawLine(pen, Convert.ToInt32(substrings[1]), Convert.ToInt32(substrings[2]), Convert.ToInt32(substrings[3]), Convert.ToInt32(substrings[4]));
                    }
                }
                file.Close();
            }
        }
        private void Output_txt_Click(object sender, EventArgs e)
        {
            FileStream fileStream = new FileStream(@"..\..\..\OutputFile.txt", FileMode.Create);
            fileStream.Close();
            StreamWriter sw = new StreamWriter(@"..\..\..\OutputFile.txt");
            //當前點個數
            if (IsReadFile != true)
            {
                //將NodeList中的元素跟具x值排序
                var NodeList_Sort = OutputNodeList.OrderBy(a => a.X).ThenBy(b => b.Y).ToList();
                foreach (var node in NodeList_Sort)
                {
                    sw.Write("P " + node.X + " " + node.Y);
                    sw.Write(System.Environment.NewLine);
                }
            }
            else
            {
                List<DataStruct.Node> NodeListCopy = new List<DataStruct.Node>();
                for (int i = 0; i < NodeNumList[CurrentDataIndex]; i++)
                {
                    DataStruct.Node temp = new DataStruct.Node();
                    temp = OutputNodeList[i];
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
}
