using System;

using System.Security.Cryptography.X509Certificates;


namespace WindowsFormsApp1
{
    public class DataStruct
    {
        public struct Node
        {
            public int X, Y;
            public Node(int p1, int p2)
            {
                X = p1;
                Y = p2;
            }
            public Node(float p1, float p2)
            {
                X = Convert.ToInt32(p1);
                Y = Convert.ToInt32(p2);
            }
        }
        public struct Edge
        {
            public int X1, Y1, X2, Y2;
            private Node _a, _b;
            public Edge(int p1, int p2, int p3, int p4)
            {
                X1 = p1;
                Y1 = p2;
                X2 = p3;
                Y2 = p4;
                _a = new Node(X1,Y1);
                _b = new Node(X2,Y2);
            }
            public Edge(float p1, float p2, float p3, float p4)
            {
                X1 = Convert.ToInt32(p1);
                Y1 = Convert.ToInt32(p2);
                X2 = Convert.ToInt32(p3);
                Y2 = Convert.ToInt32(p4);
                _a = new Node(X1, Y1);
                _b = new Node(X2, Y2);
            }
            //法向量為，
            // 若ab向量為(x,y)->法向量為(y,-x)方向向量
            public Node normal_vector()
            {
                int normal_vector_x = Y2 - Y1;
                int normal_vector_y = -(X2 - X1);
                Node normal_vector = new Node();
                normal_vector.X = normal_vector_x;
                normal_vector.Y = normal_vector_y;
                return normal_vector;
            }
            //中點
            public Node mid_Node()
            {
                int mid_Node_x = (X2 + X1) / 2;
                int mid_Node_y = (Y2 + Y1) / 2;
                Node mid_Node = new Node();
                mid_Node.X = mid_Node_x;
                mid_Node.Y = mid_Node_y;
                return mid_Node;
            }

            //中垂線
            public Node Vertical_top_Node()
            {
                int K = 1000;
                int Vertical_x = mid_Node().X + K * normal_vector().X;
                int Vertical_y = mid_Node().Y + K * normal_vector().Y;
                Node Vertical = new Node();
                Vertical.X = Vertical_x;
                Vertical.Y = Vertical_y;
                return Vertical;
            }
            public Node Vertical_down_Node()
            {
                int K = 1000;
                int Vertical_x = mid_Node().X - K * normal_vector().X;
                int Vertical_y = mid_Node().Y - K * normal_vector().Y;
                Node Vertical = new Node();
                Vertical.X = Vertical_x;
                Vertical.Y = Vertical_y;
                return Vertical;
            }
            public Edge VerticalLine()
            {
                Edge VerticalLine = new Edge(Vertical_top_Node().X, Vertical_top_Node().Y,
                Vertical_down_Node().X, Vertical_down_Node().Y);
                return VerticalLine;
            }
            public Node Top_Node()//定義y軸較小的點為起點
            {
                Node topNode = new Node();
                if (_a.X < _b.X)
                {
                    if (_a.Y < _b.Y)
                    {
                        topNode = _a;
                    }
                    else
                    {
                        topNode = _b;
                    }
                }
                else
                {
                    if (_a.Y < _b.Y)
                    {
                        topNode = _a;
                    }
                    else
                    {
                        topNode = _b;
                    }
                }
                return topNode;
            }
            public Node Down_Node()
            {
                Node downNode = new Node();
                if (_a.X < _b.X)
                {
                    if (_a.Y < _b.Y)
                    {
                        downNode = _b;
                    }
                    else
                    {
                        downNode = _a;
                    }
                }
                else
                {
                    if (_a.Y < _b.Y)
                    {
                        downNode = _b;
                    }
                    else
                    {
                        downNode = _a;
                    }
                }
                return downNode;
            }
            public Node L_Node()
            {
                Node L_Node = new Node();
                if (_a.Y < _b.Y)
                {
                    if (_a.X < _b.X)
                    {
                        L_Node = _a;
                    }
                    else
                    {
                        L_Node = _b;
                    }
                }
                else
                {
                    if (_a.X < _b.X)
                    {
                        L_Node = _a;
                    }
                    else
                    {
                        L_Node = _b;
                    }
                }
                return L_Node;
            }
            public Node R_Node()
            {
                Node R_Node = new Node();
                if (_a.Y < _b.Y)
                {
                    if (_a.X < _b.X)
                    {
                        R_Node = _b;
                    }
                    else
                    {
                        R_Node = _a;
                    }
                }
                else
                {
                    if (_a.X < _b.X)
                    {
                        R_Node = _b;
                    }
                    else
                    {
                        R_Node = _a;
                    }
                }
                return R_Node;
            }

        }
    }
}