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
      Console.WriteLine("Hello World!");

      //DrawX();
      //return;

      //ParseImageSpace();
      //return;

      //ParseImageTriangle();
      //return;

      var port = new SerialPort("COM4", 9600, Parity.None, 8, StopBits.One);
      port.Open();

      var lines = File.ReadAllLines("C:\\triangles.txt");
      foreach (var line in lines.Where(l => l.Length > 0))
      {
        port.WriteLine(line);
        Thread.Sleep(500);
      }
    }

    private static void ParseImage()
    {
      Bitmap templateImage = null;
      using (var image = new Bitmap(@"C:\\Sketch002.bmp"))
      {
        templateImage = new Bitmap(image);

        File.AppendAllText("C:\\waves.txt", "Waves              \n");
        for (var i = 0; i < templateImage.Width; i++)
        {
          for (var j = 0; j < templateImage.Height; j++)
          {
            var pixel = templateImage.GetPixel(i, j);
            if (pixel.R > 240 && pixel.G > 240 && pixel.B > 240)
              File.AppendAllText("C:\\waves.txt", "1");
            else
              File.AppendAllText("C:\\waves.txt", "0");
          }
          File.AppendAllText("C:\\waves.txt", "\n");
        }
      }
    }

    private static void ParseImageSpace()
    {
      Bitmap templateImage = null;
      using (var image = new Bitmap(@"C:\\deep_sky.bmp"))
      {
        templateImage = new Bitmap(image);

        File.AppendAllText("C:\\space.txt", "Space              \n");
        for (var i = 0; i < templateImage.Width; i++)
        {
          for (var j = 0; j < templateImage.Height; j++)
          {
            var pixel = templateImage.GetPixel(i, j);
            if (pixel.R > 240 && pixel.G > 240 && pixel.B > 240)
              File.AppendAllText("C:\\space.txt", "1");
            else
              File.AppendAllText("C:\\space.txt", "0");
          }
          File.AppendAllText("C:\\space.txt", "\n");
        }
      }
    }

    private static void ParseImageTriangle()
    {
      Bitmap templateImage = null;
      using (var image = new Bitmap(@"C:\\triangles.bmp"))
      {
        templateImage = new Bitmap(image);

        File.AppendAllText("C:\\triangles.txt", "Triangles          \n");
        for (var i = 0; i < templateImage.Width; i++)
        {
          for (var j = 0; j < templateImage.Height; j++)
          {
            var pixel = templateImage.GetPixel(i, j);
            if (pixel.R > 240 && pixel.G > 240 && pixel.B > 240)
              File.AppendAllText("C:\\triangles.txt", "1");
            else
              File.AppendAllText("C:\\triangles.txt", "0");
          }
          File.AppendAllText("C:\\triangles.txt", "\n");
        }
      }
    }

    private static void DrawX()
    {
      File.AppendAllText("C:\\x.txt", "Cross              \n");
      for (var i = 0; i < 225; i++)
      {
        for (var j = 0; j < 225; j++)
        {
          if (j < 113 && i > j && i < 225 - j || j > 112 && i < j && i > 225 - j)
            File.AppendAllText("C:\\x.txt", "1");
          else
            File.AppendAllText("C:\\x.txt", "0");
        }
        File.AppendAllText("C:\\x.txt", "\n");
      }
    }
  }
}
