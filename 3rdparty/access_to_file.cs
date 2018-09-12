using System;
using System.IO;

namespace file_opener
{
    class Program
    {
        static int Main(string[] args)
        {
			if (args.Length == 0)
            return 45;
            try
            {
                StreamReader sr = File.OpenText(args[0]);
				sr.ReadLine()
            }
            catch(IOException ex)
            {
                Console.WriteLine(ex.Message);
                return 1;
            }
            fs.Close();
            return 0;
        }
    }
}
