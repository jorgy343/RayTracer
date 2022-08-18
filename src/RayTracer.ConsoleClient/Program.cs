using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Processing;
using System.Numerics;
using RayTracer.ConsoleClient;

const int width = 800;
const int height = 600;

using (var image = new Image<Rgba32>(width, height))
{
    var pixelBuffer = new float[width * height * 4];

    unsafe
    {
        fixed (float* pixelBufferPointer = pixelBuffer)
        {
            Native.TraceScene(0, 0, 800, 600, pixelBufferPointer);
        }
    }

    image.Mutate(c => c.ProcessPixelRowsAsVector4((Span<Vector4> row, Point point) =>
    {
        for (int x = 0; x < row.Length; x++)
        {
            row[x] = new Vector4(
                pixelBuffer[((point.Y * width) + x) * 4 + 0],
                pixelBuffer[((point.Y * width) + x) * 4 + 1],
                pixelBuffer[((point.Y * width) + x) * 4 + 2],
                1.0f);
        }
    }, new Rectangle(0, 0, 800, 600)));

    await image.SaveAsPngAsync("test-image.png");
}