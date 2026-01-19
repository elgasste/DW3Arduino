using System.Windows.Media.Imaging;

namespace DW3ArduinoEditor.ViewModels
{
   public class TextureImageViewModel : ViewModelBase
   {
      private BitmapSource? _image;
      public BitmapSource? Image
      {
         get => _image;
         set => SetProperty( ref _image, value );
      }

      private int _width;
      public int Width
      {
         get => _width;
         set => SetProperty( ref _width, value );
      }

      private int _height;
      public int Height
      {
         get => _height;
         set => SetProperty( ref _height, value );
      }

      public TextureImageViewModel( BitmapSource? image, int width, int height )
      {
         Image = image;
         Width = width;
         Height = height;
      }
   }
}
