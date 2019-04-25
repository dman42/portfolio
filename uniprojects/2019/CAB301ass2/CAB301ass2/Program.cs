using System;
using System.IO;
using System.Text;

namespace CAB301ass2
{
    public static class Globals
    {
        public static int operations = 0;
        public static Random rand = new Random();
    }
    class Program
    {
        /// <summary>
        /// Function for writing words into files.
        /// </summary>
        public static void AddText(FileStream fs, string value)
        {
            byte[] info = new UTF8Encoding(true).GetBytes(value);
            fs.Write(info, 0, info.Length);
        }

        /// <summary>
        /// Creates an array of random numbers.
        /// </summary>
        /// <param name="size">Size of the array.</param>
        public static int[] RandomIntArray(int size)
        {
            int[] array = new int[size];
            for (int i = 0; i < size; i++)
            {
                array[i] = Globals.rand.Next();
            }
            return array;
        }
        
        /// <summary>
        /// Performs a Fischer-Yates random shuffle on an array.
        /// </summary>
        /// <param name="array">Array of integers.</param>
        public static void Shuffle(int[] array)
        {
            int n = array.Length;
            while (n > 1)
            {
                n--;
                int k = Globals.rand.Next(n + 1);
                int value = array[k];
                array[k] = array[n];
                array[n] = value;
            }
        }

        /// <summary>
        /// Finds the distance between the two closest elements in an array of numbers.
        /// </summary>
        /// <param name="A">Array of numbers.</param>
        /// <param name="countOperations">If set, counts the amount of basic operations performed in
        /// the function. Defaults to true.</param>
        /// <returns>The minimum distance between two of its elements.</returns>
        static int MinDistance(int[] A, bool countOperations = true)
        {
            int opCount = 0;
            int n = A.Length;

            int dmin = int.MaxValue;
            for (int i = 0; i <= n-1; i++)
            {
                for (int j = 0; j <= n-1; j++)
                {
                    opCount++;
                    if ((i != j) && (Math.Abs(A[i]-A[j]) < dmin))
                    {
                        dmin = Math.Abs(A[i] - A[j]);
                    }
                }
            }

            if (countOperations)
            {
                Globals.operations += opCount;
            }
            return dmin;
        }

        /// <summary>
        /// Finds the distance between the two closest elements in an array of numbers.
        /// </summary>
        /// <param name="A">Array of numbers.</param>
        /// <param name="countOperations">If set, counts the amount of basic operations performed in
        /// the function. Defaults to true.</param>
        /// <returns>The minimum distance d between two of its elements.</returns>
        static int MinDistance2(int[] A, bool countOperations = true)
        {
            int opCount = 0;
            int n = A.Length;

            int dmin = int.MaxValue;
            for (int i = 0; i <= n - 2; i++)
            {
                for (int j = i + 1; j <= n - 1; j++)
                {
                    int temp = Math.Abs(A[i] - A[j]);

                    opCount++;
                    if (temp < dmin)
                    {
                        dmin = temp;
                    }
                }
            }

            if (countOperations)
            {
                Globals.operations += opCount;
            }
            return dmin;
        }

        /// <summary>
        /// Tests to see if both minimum distance functions work correctly (or as intended by the algorithm).
        /// </summary>
        static bool MedianTest()
        {
            // Tests if MinDistance can correctly handle single-entry arrays.
            int expectedValue = int.MaxValue;
            int[] testArray = {1};
            int actualValue = MinDistance(testArray, false);
            if (!(actualValue == expectedValue))
            {
                Console.WriteLine("Error with test 1, output should be " + expectedValue + " but is " + actualValue);
                return false;
            }

            // Tests if MinDistance works with unique values that are equally close.
            testArray = new int[]{ 5, 6, 7, 8 };
            Shuffle(testArray);
            expectedValue = 1;
            actualValue = MinDistance(testArray, false);
            if (!(actualValue == expectedValue))
            {
                Console.WriteLine("Error with test 2, output should be " + expectedValue + " but is " + actualValue);
                return false;
            }

            // Tests if MinDistance works with unique values that are not equally close.
            testArray = new int[]{1, 2, 4, 8};
            Shuffle(testArray);
            expectedValue = 1;
            actualValue = MinDistance(testArray, false);
            if (!(actualValue == expectedValue))
            {
                Console.WriteLine("Error with test 3, output should be " + expectedValue + " but is " + actualValue);
                return false;
            }

            // Tests if MinDistance works with similar values.
            testArray = new int[]{1, 1, 3, 9};
            Shuffle(testArray);
            expectedValue = 0;
            actualValue = MinDistance(testArray, false);
            if (!(actualValue == expectedValue))
            {
                Console.WriteLine("Error with test 4, output should be " + expectedValue + " but is " + actualValue);
                return false;
            }

            // Tests if MinDistance works with negative values.
            testArray = new int[]{ -4, -1, 1, 4 };
            Shuffle(testArray);
            expectedValue = 2;
            actualValue = MinDistance(testArray, false);
            if (!(actualValue == expectedValue))
            {
                Console.WriteLine("Error with test 5, output should be " + expectedValue + " but is " + actualValue);
                return false;
            }

            // Tests if MinDistance2 can correctly handle single-entry arrays.
            expectedValue = int.MaxValue;
            testArray = new int[] { 1 };
            actualValue = MinDistance2(testArray, false);
            if (!(actualValue == expectedValue))
            {
                Console.WriteLine("Error with test 6, output should be " + expectedValue + " but is " + actualValue);
                return false;
            }

            // Tests if MinDistance2 works with unique values that are equally close.
            testArray = new int[] { 5, 6, 7, 8 };
            Shuffle(testArray);
            expectedValue = 1;
            actualValue = MinDistance2(testArray, false);
            if (!(actualValue == expectedValue))
            {
                Console.WriteLine("Error with test 7, output should be " + expectedValue + " but is " + actualValue);
                return false;
            }

            // Tests if MinDistance2 works with unique values that are not equally close.
            testArray = new int[] { 1, 2, 4, 8 };
            Shuffle(testArray);
            expectedValue = 1;
            actualValue = MinDistance2(testArray, false);
            if (!(actualValue == expectedValue))
            {
                Console.WriteLine("Error with test 8, output should be " + expectedValue + " but is " + actualValue);
                return false;
            }

            // Tests if MinDistance2 works with similar values.
            testArray = new int[] { 1, 1, 3, 9 };
            Shuffle(testArray);
            expectedValue = 0;
            actualValue = MinDistance2(testArray, false);
            if (!(actualValue == expectedValue))
            {
                Console.WriteLine("Error with test 9, output should be " + expectedValue + " but is " + actualValue);
                return false;
            }

            // Tests if MinDistance2 works with negative values.
            testArray = new int[] { -4, -1, 1, 4 };
            Shuffle(testArray);
            expectedValue = 2;
            actualValue = MinDistance2(testArray, false);
            if (!(actualValue == expectedValue))
            {
                Console.WriteLine("Error with test 10, output should be " + expectedValue + " but is " + actualValue);
                return false;
            }

            Console.WriteLine("All tests passed.");
            return true;
        }
        static void Main(string[] args)
        {
            int PROCESS_NUM = 20;
            int SIZE_ITERATOR = 100;
            int INITIAL_PROBLEM_SIZE = 100;
            int EXECS_PER_PROCESS = 100;

            double[] basicOperations = new double[PROCESS_NUM];
            double[] executionTimes = new double[PROCESS_NUM];
            int process = 0;
            int problemSize = INITIAL_PROBLEM_SIZE;

            if (MedianTest())
            {
                Console.WriteLine("Starting processes.\nProcesses 1 - 10 test MinDistance, 11 - 20 test MinDistance2.");

                while (process < PROCESS_NUM)
                {
                    Console.WriteLine("Executing process " + (process + 1) + " of " + PROCESS_NUM + ", size " + problemSize);

                    DateTime start = DateTime.Now;
                    for (int i = 0; i < EXECS_PER_PROCESS; i++)
                    {
                        if (process >= (PROCESS_NUM / 2.0))
                        {
                            MinDistance2(RandomIntArray(problemSize));
                        }
                        else
                        {
                            MinDistance(RandomIntArray(problemSize));
                        }
                    }
                    DateTime end = DateTime.Now;
                    TimeSpan fullTimeDifference = end.Subtract(start);

                    double averageTime = fullTimeDifference.TotalMilliseconds;
                    averageTime = averageTime / EXECS_PER_PROCESS;

                    double averageOps = Globals.operations;
                    averageOps = averageOps / EXECS_PER_PROCESS;
                    Globals.operations = 0;

                    basicOperations[process] = averageOps;
                    executionTimes[process] = averageTime;
                    Console.WriteLine("Average basic operations performed: " + averageOps);
                    Console.WriteLine("Average time to execute: " + averageTime + "ms");
                    process++;

                    problemSize += SIZE_ITERATOR;
                    if (process == (PROCESS_NUM / 2.0)) problemSize = INITIAL_PROBLEM_SIZE;
                }

                // Writes the data collected into an easily graphable .csv file and exports it to a certain path.
                String path = Directory.GetCurrentDirectory();
                path += @"\MinDistance.csv";
                using (FileStream fileStream = File.Create(path))
                {
                    String excelSheet = "";
                    for (int i = 0; i < PROCESS_NUM; i++)
                    {
                        excelSheet += (INITIAL_PROBLEM_SIZE + (SIZE_ITERATOR * (i % ((PROCESS_NUM / 2.0)))));
                        excelSheet += ",";
                        excelSheet += (basicOperations[i]);
                        excelSheet += ",";
                        excelSheet += (executionTimes[i]);
                        excelSheet += "\n";
                    }

                    AddText(fileStream, excelSheet);
                    Console.WriteLine("An excel sheet has been successfully exported to " + path);
                } 
            }
            Console.WriteLine("Press any key to close.");
            Console.Read();
        }
    }
}

