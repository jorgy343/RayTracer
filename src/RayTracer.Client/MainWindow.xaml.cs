using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace RayTracer.Client;

public partial class MainWindow : Window
{
    public MainWindow()
    {
        UpdateBitmap();

        InitializeComponent();
    }

    public WriteableBitmap Bitmap { get; } = new WriteableBitmap(400, 400, 96, 96, PixelFormats.Bgr32, null);

    public void UpdateBitmap()
    {
        lock (Bitmap)
        {
            try
            {
                Bitmap.Lock();

                var backBufferPointer = Bitmap.BackBuffer;
                var backBufferStride = Bitmap.BackBufferStride;

                unsafe
                {
                    for (var y = 0; y < 400; y++)
                    {
                        for (var x = 0; x < 400; x++)
                        {
                            var writeAddress = backBufferPointer + (y * backBufferStride) + (x * 4);

                            var blue = 200;
                            var green = 0;
                            var red = 0;

                            var color = blue | (green << 8) | (red << 16);

                            *(int*)writeAddress = color;
                        }
                    }
                }

                Bitmap.AddDirtyRect(new Int32Rect(0, 0, 400, 400));
            }
            finally
            {
                Bitmap.Unlock();
            }
        }
    }
}