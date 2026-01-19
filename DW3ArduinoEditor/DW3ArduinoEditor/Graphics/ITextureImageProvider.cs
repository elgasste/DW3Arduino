using System.Windows.Media.Imaging;

namespace DW3ArduinoEditor.Graphics
{
   public interface ITextureImageProvider
   {
      BitmapSource GetImageFromIndex( uint index );
      Sprite GetSpriteFromIndex( uint index );
   }
}
