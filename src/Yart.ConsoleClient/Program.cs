using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Processing;
using System.Numerics;
using Yart.ConsoleClient;
using System.Diagnostics;

const int screenWidth = 1920;
const int screenHeight = 1080;
const int subpixelCount = 4;
const int iterations = 8;

var stopwatch = new Stopwatch();
stopwatch.Start();

using (var image = new Image<Rgba32>(screenWidth, screenHeight))
{
    var pixelBuffer = new float[screenWidth * screenHeight * 4];

    unsafe
    {
        // 6.2 seconds

        //Parallel.For(0, screenHeight - 1, y =>
        for (int y = 0; y < screenHeight; y++)
        {
            fixed (float* pixelBufferPointer = pixelBuffer)
            {
                Native.TraceScene(new UIntVector2(screenWidth, screenHeight), new UIntVector2(0, (uint)y), new UIntVector2(screenWidth - 1, (uint)y), subpixelCount, iterations, pixelBufferPointer);
            }
        }//);
    }

    image.Mutate(c => c.ProcessPixelRowsAsVector4((Span<Vector4> row, Point point) =>
    {
        for (int x = 0; x < row.Length; x++)
        {
            row[x] = new Vector4(
                pixelBuffer[((point.Y * screenWidth) + x) * 4 + 0],
                pixelBuffer[((point.Y * screenWidth) + x) * 4 + 1],
                pixelBuffer[((point.Y * screenWidth) + x) * 4 + 2],
                1.0f);
        }
    }, new Rectangle(0, 0, screenWidth, screenHeight)));

    await image.SaveAsPngAsync("test-image.png");
}

stopwatch.Stop();
Console.WriteLine(stopwatch.Elapsed.TotalSeconds);