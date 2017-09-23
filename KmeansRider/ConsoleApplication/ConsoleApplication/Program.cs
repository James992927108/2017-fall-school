using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;

namespace ConsoleApplication
{
    class Program
    {
        static int loopCount = 1;
        static int recursiveFlag = 0;

        static void Main(string[] args)
        {
            double[,] nodes = new double[151, 151];
            int nodeNumber = 150;

            double[,] classes = new double[3, 5];
            int classNumber = 3;

            ReadFile(nodes);
            //隨機指派群集中心
            RandomAssignClusterCenter(classNumber, classes);
            //for recursive
            kMean(nodeNumber, classNumber, nodes, classes);
            double SSE = 0;
            for (int i = 0; i < classNumber; i++)
            {
                for (int j = 0; j < nodeNumber; j++)
                {
                    if (nodes[i, 4] == j)
                    {
                         SSE = SSE +  CulSSE(nodes[i, 0], nodes[i, 1], nodes[i, 2], nodes[i, 3], classes[j, 0],
                            classes[j, 1], classes[j, 2], classes[j, 3]);
                    }
                }
            }
            System.Console.WriteLine("SSE : {0}", SSE);
            System.Console.WriteLine("=================================");
            System.Console.WriteLine("finish! recusive times : {0}", loopCount);
            System.Console.WriteLine("Press any key to continue...");
            System.Console.ReadKey();
        }


        private static void ReadFile(double[,] nodes)
        {
//            string[] lines = System.IO.File.ReadAllLines(@"E:\\GitHub\\SimpleKmean\\IrisData.txt");
            string[] lines = System.IO.File.ReadAllLines(@"../../../../../IrisData.txt");
            for (int i = 0; i < lines.Length; i++)
            {
                string[] GetCoordinate = lines[i].Split(',');
                for (int j = 0; j < GetCoordinate.Length; j++)
                {
                    nodes[i, j] = Convert.ToDouble(GetCoordinate[j]);
                    //Console.WriteLine(nodes[i,j]);
                }
                //Console.WriteLine("\t" + lines[i]);
            }
        }

        private static void RandomAssignClusterCenter(int classNumber, double[,] classes)
        {
            System.Console.WriteLine("\t---------RandomAssignClusterCenter----------");
            System.Console.WriteLine("\tclass[ ]--( x , y , z , w )");
            for (int i = 0; i < classNumber; i++)
            {
                Random random = new Random(DateTime.Now.Millisecond);
                for (int j = 0; j < classNumber; j++)
                {
                    classes[i, j] = Math.Round(random.NextDouble() * 10, 1);
                    Thread.Sleep(20);
                }
                System.Console.WriteLine("\tclass[{0}]--({1},{2},{3},{4})", i, classes[i, 0], classes[i, 1], classes[i, 2],
                    classes[i, 3]);
            }
        }

        

        static double distance(double x1, double y1, double z1, double w1, double x2, double y2, double z2, double w2)
        {
            return Math.Round(
                Math.Sqrt(Math.Pow((x1 - x2), 2) + Math.Pow((y1 - y2), 2) + Math.Pow((z1 - z2), 2) +
                          Math.Pow((w1 - w2), 2)), 3);
        }
        
        static double CulSSE(double x1, double y1, double z1, double w1, double x2, double y2, double z2, double w2)
        {
            return  Math.Round(
                Math.Pow((x1 - x2), 2) + Math.Pow((y1 - y2), 2) + Math.Pow((z1 - z2), 2) +
                          Math.Pow((w1 - w2), 2), 3);
        }

        static void showResult(int nodeNumber, int classNumber, double[,] nodes, double[,] classes)
        {
//            for (int i = 0; i < classNumber; i++)
//            {
//                System.Console.WriteLine("\tclass[{0}]--({1},{2},{3},{4})", i, classes[i, 0], classes[i, 1],
//                    classes[i, 2], classes[i, 3]);
//            }
            System.Console.WriteLine("-------------------");
//            for (int i = 0; i < nodeNumber; i++)
//            {
//                System.Console.WriteLine("{5}---({0},{1},{2},{3}) --> class[{4}]", nodes[i, 0], nodes[i, 1],
//                    nodes[i, 2], nodes[i, 3], nodes[i, 4], i);
//            }
        }
        
        static void kMean(int nodeNumber, int classNumber, double[,] nodes, double[,] classes)
        {
            //產生初始群集
            InitClusterAndCulDistance(nodeNumber, classNumber, nodes, classes);

            //產生新的質量中心
            var tempClasses = CalculateNewCenterClass(nodeNumber, classNumber, nodes);
            
            CompareLastClass(nodeNumber, classNumber, nodes, classes, tempClasses);
        }

        private static void CompareLastClass(int nodeNumber, int classNumber, double[,] nodes, double[,] classes,
            double[,] tempClasses)
        {
            int k = 0;
            for (k = 0; k < classNumber; k++)
            {
                if ((tempClasses[k, 0] != classes[k, 0]) || (tempClasses[k, 1] != classes[k, 1]) ||
                    (tempClasses[k, 2] != classes[k, 2]) || (tempClasses[k, 3] != classes[k, 3]))
                {
                    recursiveFlag = 1;
                    break;
                }
            }
            if (k >= classNumber)
                recursiveFlag = 0;
            if (recursiveFlag == 1)
            {
                for (int j = 0; j < classNumber; j++)
                {
                    for (int i = 0; i < 4; i++)
                    {
                        classes[j, i] = tempClasses[j, i];
                    }
                }
                // recursive
                kMean(nodeNumber, classNumber, nodes, classes);
                loopCount++;
            }
            if (recursiveFlag == 0)
            {
                showResult(nodeNumber, classNumber, nodes, classes);
            }
        }

        private static void InitClusterAndCulDistance(int nodeNumber, int classNumber, double[,] nodes, double[,] classes)
        {
            System.Console.WriteLine("\t---------GeneratesInitialCluster----------");
            for (int i = 0; i < nodeNumber; i++)
            {
                double min = 100000;
                for (int j = 0; j < classNumber; j++)
                {
                    double mindDistance = distance(nodes[i, 0], nodes[i, 1], nodes[i, 2], nodes[i, 3], classes[j, 0],
                        classes[j, 1], classes[j, 2], classes[j, 3]);
                    //System.Console.WriteLine("i = {0} j = {1} mindDistance = {2}",i,j,mindDistance);
                    if (mindDistance < min)
                    {
                        min = mindDistance;
                        nodes[i, 4] = j;
                    }
                }
            }
        }
        
        private static double[,] CalculateNewCenterClass(int nodeNumber, int classNumber, double[,] nodes)
        {
            System.Console.WriteLine("\t---------CalculateNewCenterClass----------");
            double[,] tempClasses = new double[nodeNumber, 5];
            for (int j = 0; j < classNumber; j++)
            {
                double[] tempCoordinate = new double[4];
                for (int i = 0; i < nodeNumber; i++)
                {
                    if (nodes[i, 4] == j)
                    {
                        for (int n = 0; n < 4; n++)
                            tempCoordinate[n] = tempCoordinate[n] + nodes[i, n];
                        tempClasses[j, 4]++;
                    }
                }
                if (tempClasses[j, 4] == 0)
                    tempClasses[j, 4] = 1;
                for (int m = 0; m < 4; m++)
                    tempClasses[j, m] = Math.Round(tempCoordinate[m] / tempClasses[j, 4], 1);
                System.Console.WriteLine("class[{0}] :new cor ({1},{2},{3},{4}),count = {5},",j, tempClasses[j, 0],
                    tempClasses[j, 1], tempClasses[j, 2], tempClasses[j, 3], tempClasses[j, 4]);
            }
            return tempClasses;
        }

    }
}