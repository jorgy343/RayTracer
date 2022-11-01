using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Processing;
using System.Numerics;
using Yart.ConsoleClient;
using System.Diagnostics;

const int screenWidth = 1920;
const int screenHeight = 1080;

Native.create_scene();
return;

var stopwatch = new Stopwatch();

using (var image = new Image<Rgba32>(screenWidth, screenHeight))
{
    var pixelBuffer = new float[screenWidth * screenHeight * 4];

    unsafe
    {
        void* sceneData = Native.CreateScene();

        //fixed (float* pixelBufferPointer = pixelBuffer)
        //{
        //    Native.TraceScene(new UIntVector2(screenWidth, screenHeight), new UIntVector2(566, 284), new UIntVector2(566, 284), sceneData, pixelBufferPointer);
        //}

        var patches = new List<Patch>();

        const uint patchSize = 8;
        for (uint startY = 0; ; startY += patchSize)
        {
            if (startY >= screenHeight)
            {
                break;
            }

            uint endY = startY + patchSize >= screenHeight ? screenHeight - 1 : startY + patchSize - 1;

            for (uint startX = 0; ; startX += patchSize)
            {
                if (startX >= screenWidth)
                {
                    break;
                }

                uint endX = startX + patchSize >= screenWidth ? screenWidth - 1 : startX + patchSize - 1;

                patches.Add(new Patch(new UIntVector2(startX, startY), new UIntVector2(endX, endY)));
            }
        }

        stopwatch.Start();

        uint completed = 0;

        Parallel.ForEach(patches, patch =>
        //foreach (var patch in patches)
        {
            fixed (float* pixelBufferPointer = pixelBuffer)
            {
                Native.TraceScene(new UIntVector2(screenWidth, screenHeight), patch.Start, patch.End, sceneData, pixelBufferPointer);
            }

            var newCompleted = Interlocked.Increment(ref completed);

            var random = new Random();
            if (random.NextDouble() > 0.99)
            {
                Console.WriteLine(((double)newCompleted / patches.Count).ToString("p"));
            }
        });

        stopwatch.Stop();
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

Console.WriteLine(stopwatch.Elapsed.TotalSeconds);

public record struct Patch(UIntVector2 Start, UIntVector2 End);