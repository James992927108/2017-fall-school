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
        }
        public struct Edge
        {
            public int X1, Y1, X2, Y2;
            public Edge(int p1, int p2, int p3, int p4)
            {
                X1 = p1;
                Y1 = p2;
                X2 = p3;
                Y2 = p4;
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
            public Node Vertical_line()
            {
                int K = 10000;
                int Vertical_line_x = mid_Node().X + K * normal_vector().X;
                int Vertical_line_y = mid_Node().Y + K * normal_vector().Y;
                Node Vertical_line = new Node();
                Vertical_line.X = Vertical_line_x;
                Vertical_line.Y = Vertical_line_y;
                return Vertical_line;
            }
            public Node down_Node()
            {
                int K = 10000;
                int Vertical_line_x = mid_Node().X - K * normal_vector().X;
                int Vertical_line_y = mid_Node().Y - K * normal_vector().Y;
                Node Vertical_line = new Node();
                Vertical_line.X = Vertical_line_x;
                Vertical_line.Y = Vertical_line_y;
                return Vertical_line;
            }
        }
    }
}