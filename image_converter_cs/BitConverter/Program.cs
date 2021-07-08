using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Threading;

namespace BitConverter
{
  class Program
  {
    private static string[][] _fileNames = {
      new [] { "0_0_waves.txt", "0_1_cross.txt", "0_2_space.txt", "0_3_triangles.txt" },
      new [] { "1_0_bubbles.txt", "1_1_slider.txt", "1_2_circles.txt", "1_3_labyrinth.txt" }
    };

    private const int EepromIndex = 1;

    static void Main(string[] args)
    {
      //DrawSlider("slider.txt", "Slider             \n");
      //ParsePattern("labyrinth.bmp", "labyrinth.txt", "Labyrinth          \n");
      //ParseImage("multicolor.bmp", "2_img.txt", "3_img.txt");
      //return;

      var port = new SerialPort("COM4", 9600, Parity.None, 8, StopBits.One);
      port.Open();

      //for (var i = 0; i < _fileNames[EepromIndex].Length; i++)
      //  WritePattern(port, _fileNames[EepromIndex][i]);

      WriteImage(port, "2_img.txt");
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

    private static void WriteImage(SerialPort port, string fileName)
    {
      var lines = File.ReadAllLines(fileName);
      foreach (var line in lines.Where(l => l.Length > 0))
      {
        var triples = line.Split("*", System.StringSplitOptions.RemoveEmptyEntries);
        foreach (var triple in triples)
          port.WriteLine(triple);
        Thread.Sleep(300);
      }
    }

    private static void ParsePattern(string inFile, string outFile, string name)
    {
      using (var image = new Bitmap(inFile))
      {
        File.AppendAllText(outFile, name);
        for (var i = 0; i < image.Width; i++)
        {
          for (var j = 0; j < image.Height; j++)
          {
            var pixel = image.GetPixel(i, j);
            if (pixel.R > 127 && pixel.G > 127 && pixel.B > 127)
              File.AppendAllText(outFile, "1");
            else
              File.AppendAllText(outFile, "0");
          }
          File.AppendAllText(outFile, "\n");
        }
      }
    }

    private static void ParseImage(string inFile, string outFile0, string outFile1)
    {
      using (var image = new Bitmap(inFile))
      {
        for (var i = 0; i < image.Height; i++)
        {
          var filename = i < 48 ? outFile0 : outFile1;
          for (var j = 0; j < image.Width; j++)
          {
            var pixel = image.GetPixel(j, i);
            File.AppendAllText(filename, $"{pixel.R}|{pixel.G}|{pixel.B}*");
          }
          File.AppendAllText(filename, "\n");
        }
      }
    }

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
