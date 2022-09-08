using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Processing;
using System.Numerics;
using RayTracer.ConsoleClient;

const int screenWidth = 800;
const int screenHeight = 600;

using (var image = new Image<Rgba32>(screenWidth, screenHeight))
{
    var pixelBuffer = new float[screenWidth * screenHeight * 4];

    unsafe
    {
        Parallel.For(0, 600, y =>
        {
            fixed (float* pixelBufferPointer = pixelBuffer)
            {
                Native.TraceScene(screenWidth, screenHeight, 0, y, 800, y + 1, 16, 8, pixelBufferPointer);
            }
        });
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
    }, new Rectangle(0, 0, 800, 600)));

    await image.SaveAsPngAsync("test-image.png");
}