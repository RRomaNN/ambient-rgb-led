using System;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Threading;

namespace BitConverter
{
  class Program
  {
    static void Main(string[] args)
    {
      //Parse3ColorPattern("4-color-image.bmp", "4-color-image.txt");
      //ParsePattern("4-color-image.bmp", "2-color-image.txt");

      var port = new SerialPort("COM4", 9600, Parity.None, 8, StopBits.One);
      port.Open();
      //Write2ColorPattern(port, "2-color-image.txt");
      Write2ColorPattern(port, "4-color-image.txt");
    }

    private static void Write2ColorPattern(SerialPort port, string fileName)
    {
      var lines = File.ReadAllLines(fileName);
      var counter = 0;
      foreach (var line in lines.Where(l => l.Length > 0))
      {
        port.WriteLine(line);
        Thread.Sleep(1000);
        Console.WriteLine($"{counter++} / 255\n");
      }
    }

    private static void Parse3ColorPattern(string inFile, string outFile)
    {
      using (var image = new Bitmap(inFile))
      {
        for (var i = 0; i < image.Width; i++)
        {
          for (var j = 0; j < image.Height; j++)
          {
            var pixel = image.GetPixel(i, j);
            if (pixel.R > 250)
              File.AppendAllText(outFile, "1");
            else if (pixel.G > 250)
              File.AppendAllText(outFile, "2");
            else if (pixel.B > 250)
              File.AppendAllText(outFile, "3");
            else
              File.AppendAllText(outFile, "0");
          }
          File.AppendAllText(outFile, "\n");
        }
      }
    }

    private static void ParsePattern(string inFile, string outFile)
    {
      using (var image = new Bitmap(inFile))
      {
        for (var i = 0; i < image.Width; i++)
        {
          for (var j = 0; j < image.Height; j++)
          {
            var pixel = image.GetPixel(i, j);
            if (pixel.R > 250 || pixel.G > 250 || pixel.B > 250)
              File.AppendAllText(outFile, "1");
            else
              File.AppendAllText(outFile, "0");
          }
          File.AppendAllText(outFile, "\n");
        }
      }
    }
  }
}
