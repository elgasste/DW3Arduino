using System.Windows.Media.Imaging;
using System.Windows.Media;
using System.Windows;

namespace DW3ArduinoEditor
{
   public static class BitmapUtils
   {
      public static Color GetBitmapPixelColor( BitmapSource bitmap, int x, int y )
      {
         var bytesPerPixel = (int)Math.Truncate( ( bitmap.Format.BitsPerPixel + 7.0 ) / 8.0 );
         int stride = bitmap.PixelWidth * bytesPerPixel;
         byte[] pixel = new byte[1];
         var rect = new Int32Rect( x, y, 1, 1 );

         bitmap.CopyPixels( rect, pixel, stride, 0 );
         return bitmap.Palette.Colors[pixel[0]];
      }

      public static ushort GetPixelColor16( BitmapSource bitmap, int x, int y ) => ColorUtils.ColorToUInt16( GetBitmapPixelColor( bitmap, x, y ) );

      public static void CheckTileTexturePoolBitmapFormat( BitmapSource bitmapSource )
      {
         if ( bitmapSource.Format != PixelFormats.Indexed8 )
         {
            throw new Exception( "Tile texture image pixel format should be Indexed8" );
         }
         else if ( bitmapSource.PixelHeight != Constants.TileSize )
         {
            throw new Exception( string.Format( "Tileset image height should be {0}", Constants.TileSize ) );
         }
         else if ( bitmapSource.PixelWidth % Constants.TileSize != 0 )
         {
            throw new Exception( string.Format( "Tileset image width should be a multiple of {0}", Constants.TileSize ) );
         }
      }
   }
}
