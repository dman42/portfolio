using System;
using System.IO;
using System.Text;

namespace CAB301ass1
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
        /// Creates a randomly shuffled array which, when ordered, goes from 0 to 1 less than the size of the array.
        /// </summary>
        /// <param name="size">Size of the array.</param>
        public static int[] IntArray(int size)
        {
            int[] array = new int[size];
            for (int i = 0; i < size; i++)
            {
                array[i] = i;
            }
            Shuffle(array);
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
        /// Returns the median value in a given array A of n numbers. This is
        /// the kth element, where k = n/2, if the array was sorted.
        /// </summary>
        /// <param name="A">Array of numbers.</param>
        /// <param name="countOperations">If set, counts the amount of basic operations performed in
        /// the function. Defaults to true.</param>
        static int BruteForceMedian(int[] A, bool countOperations = true)
        {
            int opCount = 0;

            int n = A.Length;
            double k = Math.Ceiling(n / 2.0);
            for (int i = 0; i <= n-1; i++)
            {
                int numsmaller = 0; // How many elements are smaller than A[i]
                int numequal = 0; // How many elements are equal to A[i]

                for (int j = 0; j <= n-1; j++)
                {
                    opCount++;

                    if (A[j] < A[i])
                    {
                        numsmaller = numsmaller + 1;
                    }

                    else if (A[j] == A[i])
                    {
                        numequal = numequal + 1;
                    }
                }

                if (numsmaller < k && k <= (numsmaller + numequal))
                {
                    if (countOperations)
                    {
                        Globals.operations += opCount;
                    }
                    return A[i];
                }
            }

            return A[n-1]; // This is a placeholder return statement so C# runs without any errors while not breaking
            // the specified format of the function. There is no feasable scenario where this return statement is executed.

        }

        /// <summary>
        /// Tests to see if the BruteForceMedian function works correctly.
        /// </summary>
        static bool MedianTest()
        {
            // Tests if the function can correctly handle single-entry arrays.
            int expectedValue = Globals.rand.Next(1, 100);
            int[] firstTestArray = {expectedValue};
            int actualValue = BruteForceMedian(firstTestArray, false);
            if (!(actualValue == expectedValue))
            {
                Console.WriteLine("Error with test 1, output should be " + expectedValue + " but is " + actualValue);
                return false;
            }

            // Tests if the function works with unique values on an even array.
            int[] secondTestArray = IntArray(4);
            expectedValue = 1;
            actualValue = BruteForceMedian(secondTestArray, false);
            if (!(actualValue == expectedValue))
            {
                Console.WriteLine("Error with test 2, output should be " + expectedValue + " but is " + actualValue);
                return false;
            }

            // Tests if the function works with unique values on an odd array.
            int[] thirdTestArray = IntArray(5);
            expectedValue = 2;
            actualValue = BruteForceMedian(thirdTestArray, false);
            if (!(actualValue == expectedValue))
            {
                Console.WriteLine("Error with test 3, output should be " + expectedValue + " but is " + actualValue);
                return false;
            }

            // Tests if the function works with similar values.
            int[] fourthTestArray = {1, 1, 1, 1, 5, 5, 5};
            Shuffle(fourthTestArray);
            expectedValue = 1;
            actualValue = BruteForceMedian(fourthTestArray, false);
            if (!(actualValue == expectedValue))
            {
                Console.WriteLine("Error with test 4, output should be " + expectedValue + " but is " + actualValue);
                return false;
            }

            Console.WriteLine("All tests passed.");
            return true;
        }
        static void Main(string[] args)
        {
            int PROCESS_NUM = 10; 
            int SIZE_ITERATOR = 100; 
            int INITIAL_PROBLEM_SIZE = 100; 
            int EXECS_PER_PROCESS = 500;

            double[] basicOperations = new double[PROCESS_NUM];
            double[] executionTimes = new double[PROCESS_NUM];
            int process = 0;
            int problemSize = INITIAL_PROBLEM_SIZE; 

            if (MedianTest())
            {
                Console.WriteLine("Starting processes.");

                while (process < PROCESS_NUM)
                {
                    Console.WriteLine("Executing process " + (process + 1) + " of " + PROCESS_NUM + ", size " + problemSize);

                    DateTime start = DateTime.Now;
                    for (int j = 0; j < EXECS_PER_PROCESS; j++)
                    {
                        BruteForceMedian(IntArray(problemSize));
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
                }

                // Writes the data collected into an easily graphable .csv file and exports it to a certain path.
                String path = Directory.GetCurrentDirectory(); 
                path += @"\Median.csv";
                using (FileStream fileStream = File.Create(path))
                {
                    String excelSheet = "";
                    for (int i = 0; i < PROCESS_NUM; i++)
                    {
                        excelSheet += (INITIAL_PROBLEM_SIZE + (SIZE_ITERATOR*i));
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
