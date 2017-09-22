using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
 
namespace SimpleKmean
{
    class Program
    {
        static void Main(string[] args)
        {
            double[,] nodes = new double[151,151];
            double[,] classes = new double[3,5];
            int nodeNumber = 150;
            int classNumber = 3;
            string[] lines = System.IO.File.ReadAllLines(@"E:\\GitHub\\SimpleKmean\\SampleKmean\\IrisData.txt");
            for(int i =0 ; i <lines.Length;i++)
            {
                string[] GetCoordinate = lines[i].Split(',');
                for ( int j = 0 ; j < GetCoordinate.Length ; j++)
                {
                    nodes[i,j] = Convert.ToDouble(GetCoordinate[j]);    
                    //Console.WriteLine(nodes[i,j]);
                }
                //Console.WriteLine("\t" + lines[i]);
            }
            System.Console.WriteLine("-------------------");
            System.Console.WriteLine("\tclass[ ]--( x , y , z , w )");
            System.Console.WriteLine("\t        -------------------");
            for (int i = 0; i < classNumber; i++)
            {
                Random random = new Random(DateTime.Now.Millisecond);
                for (int j = 0; j < classNumber; j++)
                {        
                    classes[i, j] = Math.Round(random.NextDouble()*10,1);
                }
                System.Console.WriteLine("\tclass[{0}]--({1},{2},{3},{4})", i, classes[i, 0], classes[i, 1],classes[i, 2],classes[i, 3]);
            } 
            System.Console.WriteLine("-------------------");
            
            
            kMean(nodeNumber, classNumber, nodes, classes);
            System.Console.WriteLine("=================================");        
            //showResult( nodeNumber, classNumber, nodes,classes);
        }
        static void showResult( int nodeNumber, int classNumber ,double[,] nodes,double[,] classes)
        {
//            for (int i = 0; i < classNumber; i++)
//            {
//                System.Console.WriteLine("\tclass[{0}]--({1},{2},{3},{4})", i, classes[i, 0], classes[i, 1],classes[i, 2],classes[i, 3]);
//            }
            System.Console.WriteLine("-------------------");
            for (int i = 0; i < nodeNumber; i++)
            {
                System.Console.WriteLine("{5}---({0},{1},{2},{3}) --> class[{4}]", nodes[i, 0], nodes[i, 1], nodes[i, 2],nodes[i, 3],nodes[i, 4],i);
            }
        }
        
        static double distance(double x1, double y1, double z1, double w1,double x2, double y2,double z2, double w2)
        {
            return Math.Round(Math.Sqrt(Math.Pow((x1 - x2), 2) + Math.Pow((y1 - y2), 2) + Math.Pow((z1 - z2), 2) + Math.Pow((w1 - w2), 2)),1);
        }
        
         static void kMean(int nodeNumber, int classNumber, double[,] nodes, double[,] classes)
        {
            for (int i = 0; i < nodeNumber; i++)
            {
                double min = 100000;
                for (int j = 0; j < classNumber; j++)
                {
                    double mindDistance = distance(nodes[i, 0], nodes[i, 1],nodes[i, 2], nodes[i, 3], classes[j, 0], classes[j, 1],classes[j, 2], classes[j, 3]);
                    //System.Console.WriteLine("i = {0} j = {1} mindDistance = {2}",i,j,mindDistance);
                    if (mindDistance < min)
                    {
                        min = mindDistance;
                        nodes[i, 4] = j; 
                    }
                }
            }
            showResult(nodeNumber, classNumber , nodes ,classes);

            //
            // calculate the new center class
            //

//            int[,] tempClasses = new int[nodeNumber, 3];
//            for (int j = 0; j < classNumber; j++)
//            {
//                int[] tempCoordinate = new int[2];
//                for (int i = 0; i < nodeNumber; i++)
//                {
//                    if (nodes[i, 2] == j)
//                    {
//                        // new class
//                        tempCoordinate[0] += nodes[i, 0];
//                        tempCoordinate[1] += nodes[i, 1];
//                        tempClasses[j, 2]++;
//                    }
//                }
//                if (tempClasses[j, 2] == 0)
//                    tempClasses[j, 2] = 1;
//                tempClasses[j, 0] = tempCoordinate[0] / tempClasses[j, 2];
//                tempClasses[j, 1] = tempCoordinate[1] / tempClasses[j, 2];
//                System.Console.WriteLine("count = {3},class[{0}] :new cor ({1},{2})", j, tempClasses[j, 0], tempClasses[j, 1], tempClasses[j, 2]);
//
//            }
//            //System.Console.ReadKey();
//            int k = 0;
//            for (k = 0; k < classNumber; k++)
//            {
//                if ((tempClasses[k, 0] != classes[k, 0]) || (tempClasses[k, 1] != classes[k, 1]))
//                {
//                    recursiveFlag = 1;
//                    break;
//                }
//            }
//            if (k >= classNumber)
//                recursiveFlag = 0;
//            if (recursiveFlag == 1)
//            {
//                for (int j = 0; j < classNumber; j++)
//                {
//                    classes[j, 0] = tempClasses[j, 0];
//                    classes[j, 1] = tempClasses[j, 1];
//                }
//                System.Console.ReadKey();
//                // recursive
//                kMean(nodeNumber, classNumber, nodes, classes);
//                loopCount++;
//            }
//            if (recursiveFlag == 0)
//            {
//                //System.Console.WriteLine("=================================");
//                //System.Console.WriteLine("finish! recusive times : {0}",loopCount);
//                showResult(nodes, nodeNumber, classes, classNumber);
//            }
        }
    }
}