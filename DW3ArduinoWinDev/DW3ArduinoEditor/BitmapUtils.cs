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
            throw new Exception( string.Format( "Tile texture pool image height should be {0}", Constants.TileSize ) );
         }
         else if ( bitmapSource.PixelWidth % Constants.TileSize != 0 )
         {
            throw new Exception( string.Format( "Tile texture pool image width should be a multiple of {0}", Constants.TileSize ) );
         }
      }

      public static void CheckStaticSpriteTexturePoolBitmapFormat( BitmapSource bitmapSource )
      {
         if ( bitmapSource.Format != PixelFormats.Indexed8 )
         {
            throw new Exception( "Static sprite texture image pixel format should be Indexed8" );
         }
         else if ( bitmapSource.PixelHeight != Constants.StaticSpriteTextureSize )
         {
            throw new Exception( string.Format( "Static sprite pool image height should be {0}", Constants.StaticSpriteTextureSize ) );
         }
         else if ( bitmapSource.PixelWidth % Constants.StaticSpriteTextureSize != 0 )
         {
            throw new Exception( string.Format( "Static sprite pool image width should be a multiple of {0}", Constants.StaticSpriteTextureSize ) );
         }
      }

      public static void CheckActiveSpriteTexturePoolBitmapFormat( BitmapSource bitmapSource )
      {
         if ( bitmapSource.Format != PixelFormats.Indexed8 )
         {
            throw new Exception( "Active sprite texture image pixel format should be Indexed8" );
         }
         else if ( bitmapSource.PixelHeight != Constants.ActiveSpriteTextureHeight )
         {
            throw new Exception( string.Format( "Active sprite pool image height should be {0}", Constants.ActiveSpriteTextureHeight ) );
         }
         else if ( bitmapSource.PixelWidth % Constants.ActiveSpriteTextureWidth != 0 )
         {
            throw new Exception( string.Format( "Active sprite pool image width should be a multiple of {0}", Constants.ActiveSpriteTextureWidth ) );
         }
      }

      public static void CheckTextTilesBitmapFormat( BitmapSource bitmapSource )
      {
         if ( bitmapSource.Format != PixelFormats.Indexed8 )
         {
            throw new Exception( "Text tiles image pixel format should be Indexed8" );
         }
         else if ( bitmapSource.PixelWidth != ( Constants.TextTileCount * Constants.TextTileSize ) )
         {
            throw new Exception( string.Format( "Text tiles image width should be {0}", Constants.TextTileCount * Constants.TextTileSize ) );
         }
         else if ( bitmapSource.PixelHeight != Constants.TextTileSize )
         {
            throw new Exception( string.Format( "Text tiles image height should be {0}", Constants.TextTileSize ) );
         }
      }
   }
}
