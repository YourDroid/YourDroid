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
			FileStream fs;
            try
            {
                fs = new FileStream(args[0], FileMode.Open, FileAccess.ReadWrite);
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
