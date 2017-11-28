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
        }
    }
}