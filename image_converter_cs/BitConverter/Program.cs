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
      DrawSlider("slider.txt", "Slider             \n");
      //ParseImage("circles.bmp", "circles.txt", "Circles            \n");
      return;

      var port = new SerialPort("COM4", 9600, Parity.None, 8, StopBits.One);
      port.Open();

      WritePattern(port, "waves.txt");
      WritePattern(port, "cross.txt");
      WritePattern(port, "space.txt");
      WritePattern(port, "triangles.txt");
    }

    private static void WritePattern(SerialPort port, string fileName)
    {
      var lines = File.ReadAllLines(fileName);
      foreach (var line in lines.Where(l => l.Length > 0))
      {
        port.WriteLine(line);
        Thread.Sleep(1000);
      }
    }

    /*private static void ParseImage(string inFile, string outFile, string name)
    {
      using (var image = new Bitmap(inFile))
      {
        File.AppendAllText(outFile, name);
        for (var i = 0; i < image.Width; i++)
        {
          for (var j = 0; j < image.Height; j++)
          {
            var pixel = image.GetPixel(i, j);
            if (pixel.R > 240 && pixel.G > 240 && pixel.B > 240)
              File.AppendAllText(outFile, "1");
            else
              File.AppendAllText(outFile, "0");
          }
          File.AppendAllText(outFile, "\n");
        }
      }
    }*/

    private static void DrawX(string outFile, string name)
    {
      File.AppendAllText(outFile, name);
      for (var i = 0; i < 225; i++)
      {
        for (var j = 0; j < 225; j++)
        {
          if (j < 113 && i > j && i < 225 - j || j > 112 && i < j && i > 225 - j)
            File.AppendAllText(outFile, "1");
          else
            File.AppendAllText(outFile, "0");
        }
        File.AppendAllText(outFile, "\n");
      }
    }

    private static void DrawSlider(string outFile, string name)
    {
      File.AppendAllText(outFile, name);
      for (var i = 0; i < 225; i++)
      {
        for (var j = 0; j < 225; j++)
        {
          if (i < 113)
          {
            if (j > 2 * i)
              File.AppendAllText(outFile, "1");
            else
              File.AppendAllText(outFile, "0");
          }
          else
          {
            if (j > 2 * (i - 113))
              File.AppendAllText(outFile, "0");
            else
              File.AppendAllText(outFile, "1");
          }
        }
        File.AppendAllText(outFile, "\n");
      }
    }
  }
}
